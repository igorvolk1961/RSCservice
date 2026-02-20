#if !defined(__linux__)
#include "stdafx.h"
#else
#include <string>
#endif

#include <QtXml>
#include "rsctosvg.h"
#include "svgimage.h"
#include "imlservex.h"
#include "imlserv0.h"
#include "rsc.h"
//#include "semutils.h"
//#include "exception.h"
#include <cstring>
#include <climits>
#include <pthread.h>
#include "converterhandler.h"
#include "debug.h"
#include "myexception.h"
#include "postbuilder.h"

#define SVG_BUILER_PARAMS_INIT_SIZE 32

int DEBUG_MODE = 0;

class SvgBuilderParm{
  public:
    SvgBuilderParm(){}
    SvgBuilderParm(const char* rscFilePath,
                   HRSC hrsc,
                   double baseLineLength,
                   double baseLineLength_px,
                   ConverterHandler* converterHandler,
                   int encoding){
        this->rscFilePath = new char[strlen(rscFilePath) + 1],
        strcpy(this->rscFilePath, rscFilePath);
        this->hrsc = hrsc;
        TRsc* rsc = (TRsc*)(void*)hrsc;
        rsc->setBaseLineLength(baseLineLength);
        rsc->setBaseLineLength_px(baseLineLength_px);
        this->baseLineLength = baseLineLength;
        this->baseLineLength_px = baseLineLength_px;
        this->converterHandler = converterHandler;
        this->encoding = encoding;
    }
    ~SvgBuilderParm(){
        delete[] this->rscFilePath;
    }

    char* rscFilePath;
    HRSC hrsc;
    double baseLineLength;
    double baseLineLength_px;
    ConverterHandler* converterHandler;
    int encoding;
};

#if defined(__linux__)
  pthread_mutex_t mutex;
#else
  CRITICAL_SECTION mutex;
#endif

void synchronized_begin(){
#if defined(__linux__)
      pthread_mutex_lock(&mutex);
#else
      EnterCriticalSection(&mutex);
#endif
}

void synchronized_end(){
#if defined(__linux__)
      pthread_mutex_unlock(&mutex);
#else
      LeaveCriticalSection(&mutex);
#endif
}

SvgBuilderParm** _svgBuilderParms = NULL;
int _svgBuilderParms_length;
int _svgBuilderParms_capacity;

void __attribute__ ((constructor)) on_lib_load(){
  printf("on_lib_load\n");
#if defined(__linux__)
  pthread_mutex_init(&mutex, NULL);
#else
  InitializeCriticalSection(&mutex);
#endif
}

void __attribute__ ((destructor)) on_lib_unload(){
  printf("on_lib_unload\n");
#if defined(__linux__)
  pthread_mutex_destroy(&mutex);
#else
  DeleteCriticalSection(&mutex);
#endif

  if (_svgBuilderParms) {
    for (int j = 0; j < _svgBuilderParms_length; ++j) {
      RscToSvg::deleteSvgBuilder(j);
    }
    delete[] _svgBuilderParms;
  }
}

//namespace pugi {

//class xml_string_writer : public xml_writer
//{
//public:
//    std::string result;
//    size_t total_size;
//    xml_string_writer():xml_writer(){
//        result = "";
//        total_size = 0;
//    }
//    ~xml_string_writer(){
//    }

//	virtual void write(const void* data, size_t size)
//	{
//        char* s = new char[size + 1];
////        memset(s, 0, size + 1);
//        s[size] = 0;
//        memcpy(s, data, size);
//        result += std::string(s);
//        delete[] s;
//    }
//};

//} // namespace

//char* node_to_string(QDomElement node)
//{
//    pugi::xml_string_writer writer;
//    node.print(writer);

//    const char* xml_str = writer.result.c_str();
//    char* result = new char[strlen(xml_str) + 1];
//    strcpy(result, xml_str);
//    return result;
//}



RscToSvg::RscToSvg(){

}

long RscToSvg::createSvgBuilder(const char* rscFilePath, double baseLineLength, double baseLineLength_px,
                                int debug_mode, int encoding){
//    printf("RscToSvg  begin\n");
    DEBUG_MODE = debug_mode;

    synchronized_begin();
    if (!_svgBuilderParms) {
          _svgBuilderParms_length = 0;
          _svgBuilderParms_capacity = SVG_BUILER_PARAMS_INIT_SIZE;
          _svgBuilderParms = new SvgBuilderParm*[_svgBuilderParms_capacity];
      }
    synchronized_end();
      int j = 0;
      while (j < _svgBuilderParms_length){
          SvgBuilderParm* sbp = _svgBuilderParms[j];
          if ((strcmp(sbp->rscFilePath, rscFilePath) == 0) &&
              (fabs(sbp->baseLineLength - baseLineLength) < baseLineLength * 0.01) &&
              (fabs(sbp->baseLineLength_px - baseLineLength_px) < baseLineLength_px * 0.01) &&
              (sbp->encoding == encoding)) {
              break;
          }
          ++j;
      }
      long svgBuilderIndex;
      if (j < _svgBuilderParms_length){
        svgBuilderIndex = j;
      } else {
        synchronized_begin();
        if (_svgBuilderParms_length == SVG_BUILER_PARAMS_INIT_SIZE) {
          _svgBuilderParms_capacity += SVG_BUILER_PARAMS_INIT_SIZE;
          SvgBuilderParm** svgBuilderParms_new = new SvgBuilderParm*[_svgBuilderParms_capacity];
          memcpy(svgBuilderParms_new, _svgBuilderParms, _svgBuilderParms_length);
          delete[] _svgBuilderParms;
          _svgBuilderParms = svgBuilderParms_new;
      }
      synchronized_end();
        if (FILE *file = fopen(rscFilePath, "r")) {
          fclose(file);
  //        printf("createSvgBuilder file %s found", rscFilePath);
      } else {
         char* message = new char[255];
         sprintf(message,  "createSvgBuilder file %s not found", rscFilePath);
         throw MyException(message);
      }

      HRSC hrsc = mapOpenRsc(rscFilePath);

      if (hrsc == 0) {
        char* message = new char[255];
        sprintf(message,  "createSvgBuilder failed loading rsc %s", rscFilePath);
        throw MyException(message);
      } else {
        TRsc* rsc = (TRsc*)(void*)hrsc;
        RSCHEADER* header = rsc->getHeader();
        int enc = header->Encoding;
        printf("createSvgBuilder rsc loaded\n");
        ConverterHandler* converterHandler = new ConverterHandler();
        svgBuilderIndex = _svgBuilderParms_length;
        _svgBuilderParms[svgBuilderIndex] = new SvgBuilderParm(rscFilePath, hrsc, baseLineLength, baseLineLength_px,
                                                                       converterHandler, encoding);
        _svgBuilderParms_length++;
      }
    }
//    printf("RscToSvg end\n");
    return svgBuilderIndex;
}

HRSC RscToSvg::getHrsc(long svgBuilderIndex){
    synchronized_begin();
    if (!_svgBuilderParms) {
        char* message = new char[255];
        sprintf(message,  "RscToSvg error: svgBuilderParms is NULL");
        synchronized_end();
        throw MyException(message);
    }
    if (svgBuilderIndex >= _svgBuilderParms_length) {
        char* message = new char[255];
        sprintf(message,  "RscToSvg error: svgBuilderIndex >= svgBuilderParms_length");
        synchronized_end();
        throw MyException(message);
    }
    SvgBuilderParm* svgBuilderParm = _svgBuilderParms[svgBuilderIndex];
    synchronized_end();
    HRSC hrsc = svgBuilderParm->hrsc;
    return hrsc;
}

void RscToSvg::deleteSvgBuilder(long svgBuilderIndex){
    synchronized_begin();
    if (!_svgBuilderParms) {
        char* message = new char[255];
        sprintf(message,  "RscToSvg error: svgBuilderParms is NULL");
        synchronized_end();
        throw MyException(message);
    }
    if (svgBuilderIndex >= _svgBuilderParms_length) {
        char* message = new char[255];
        sprintf(message,  "RscToSvg error: svgBuilderIndex >= svgBuilderParms_length");
        synchronized_end();
        throw MyException(message);
    }
    SvgBuilderParm* svgBuilderParm = _svgBuilderParms[svgBuilderIndex];
    HRSC hrsc = svgBuilderParm->hrsc;

    mapCloseRsc(hrsc);

    svgBuilderParm->converterHandler->closeAll();
    delete svgBuilderParm->converterHandler;

    delete svgBuilderParm;

    synchronized_end();

}


long RscToSvg::getIncodeForExcode(long svgBuilderIndex, long excode, int local){
    HRSC hrsc = getHrsc(svgBuilderIndex);

    long int incode = imlGetRscObjectCodeByNumber(hrsc, excode, local);
    if (!incode) {
        char* message = new char[255];
        sprintf(message,  "RscToSvg::getIncodeForExcode error in imlGetRscObjectCodeByNumber");
        throw MyException(message);
    }
    else {
//        printf("RscToSvg::getIncodeForExcode incode=%ld\n", incode);
    }
    return incode;
}

long RscToSvg::getIncodeForKey(long svgBuilderIndex, const char* key){
    HRSC hrsc = getHrsc(svgBuilderIndex);

    long int incode = imlGetRscObjectCodeByKey(hrsc, key);
    if (!incode) {
        return 0;
//        char* message = new char[255];
//        sprintf(message,  "RscToSvg::getIncodeForKey error in imlGetRscObjectCodeByKey");
//        throw MyException(message);
    }
    else {
//        printf("RscToSvg::getIncodeForKey incode=%ld\n", incode);
    }
    return incode;
}

long RscToSvg::getSvgImageByKey(long svgBuilderIndex, const char* key, double** metricCoords, int* metricLengths, int metricCount,
                                      char* semantics, char** texts, SvgImage* svgImage, int symbolType,
                                double rotation, double symbolSizeFactor){
    long incode = getIncodeForKey (svgBuilderIndex, key);
    return getSvgImageByIncode(svgBuilderIndex, incode, metricCoords, metricLengths, metricCount,
                                              semantics, texts, svgImage, symbolType,
                                              rotation, symbolSizeFactor);
}

long RscToSvg::getSvgImageByIncode(long svgBuilderIndex, long incode, double** metricCoords, int* metricLengths, int metricCount,
                                          char* semantics, char** texts, SvgImage* svgImage, int symbolType,
                                   double rotation, double symbolSizeFactor){
    synchronized_begin();
    if (!_svgBuilderParms) {
        char* message = new char[255];
        sprintf(message,  "RscToSvg error: svgBuilderParms is NULL");
        synchronized_end();
        throw MyException(message);
    }
    if (svgBuilderIndex >= _svgBuilderParms_length) {
        char* message = new char[255];
        sprintf(message,  "RscToSvg error: svgBuilderIndex >= svgBuilderParms_length");
        synchronized_end();
        throw MyException(message);
    }
    SvgBuilderParm* svgBuilderParm = _svgBuilderParms[svgBuilderIndex];
    synchronized_end();
    HRSC hrsc = svgBuilderParm->hrsc;

    int pointCount = 0;
#ifdef GIS_OPERATOR
    int * PolyCounts = new int[metricCount];    // Адрес массива количества точек
#else
    long * PolyCounts = new long[metricCount];
#endif
    for (int j =0; j < metricCount; ++j) {
        if (metricLengths[j] < 2){
            char* message = new char[255];
            sprintf(message,  "RscToSvg::getSvgImage error: metricLengths[j] < 2");
            throw MyException(message);
        }
        if (metricLengths[j] % 2 != 0){
            char* message = new char[255];
            sprintf(message,  "RscToSvg::getSvgImage error: metricLengths[j] % 2 != 0");
            throw MyException(message);
        }

      PolyCounts[j] = metricLengths[j] / 2;
      pointCount += PolyCounts[j];
    }

    double x0 = metricCoords[0][0];
    double y0 = metricCoords[0][1];
    DRAWPOINT * Points = new DRAWPOINT[pointCount];  // Адрес массива объектов типа POINT
    size_t n = 0;
    for (int j = 0; j < metricCount; ++j) {
      for (int m = 0; m < PolyCounts[j]; ++m){
        Points[n].x = metricCoords[j][2 * m] - x0;
        Points[n].y = metricCoords[j][2 * m + 1] - y0;
        ++n;
      }
    }

    if (DEBUG_MODE > 4) {
       printf("\n");
       printf("RscToSvg::getSvgImage metricCount=%u\n", metricCount);
       int n=0;
       for (int m=0; m<metricCount; ++m){
           printf("RscToSvg::getSvgImage PolyCounts[m]=%u\n", PolyCounts[m]);
           for (int i=0; i<PolyCounts[m]; ++i){
               printf("RscToSvg::getSvgImage Points[n].x=%u Points[n].y=%u\n",  Points[n].x, Points[n].y);
               ++n;
           }
      }
      printf("\n");
    }

    POLYDATAEX* data = new POLYDATAEX();
    data->Points = Points;  // Адрес массива объектов типа POINT
    data->PolyCounts = PolyCounts;    // Адрес массива количества точек
    data->Count = metricCount;    // Число ломаных (многоугольников, текстов)
    data->Text = texts;   // Указатель на массив адресов текстов
    data->AlignCode = 0;   // Код выравнивания для отображения текста
    data->Flags = 0;   // Флаги обработки метрики - равно 0
    data->Semantic = semantics;   // Указатель на семантику объекта (SEMANTIC *)
//	BOX        Border;   // Прямоугольник габаритов объекта в пикселах относительно области рисования
    data->ShowScale = 100000;   // Текущий масштаб отображения электронной карты
    data->MapRsc = hrsc;   // Классификатор карты, содержащей отображаемый объект
    data->Height = 0.;   // Высота первой точки объекта (если 3D-координаты) или 0
    if (pointCount > 1) {
      data->DX = Points[1].X - Points[0].X;       // Приращение от первой точки до второй на карте
      data->DY = Points[1].Y - Points[0].Y;       // в единицах карты - для ориентирования векторных объектов
    } else {
      data->DX = 0.;
      data->DY = 0.;
}

    int type = imlGetRscObjectFunction(hrsc, incode);
    char * parm = imlGetRscObjectParameters(hrsc, incode);

    IMGCONTEXT * context = new IMGCONTEXT();

    QDomDocument doc;
    QDomElement svg_node = doc.appendChild(doc.createElement("svg")).toElement();
    svg_node.setAttribute("xmlns", "http://www.w3.org/2000/svg");
    svg_node.setAttribute("xmlns:rsc", "http://int.spb.ru/rsc");
    QDomElement g_node = svg_node.appendChild(doc.createElement("g")).toElement();
    QDomElement desc_node = svg_node.appendChild(doc.createElement("desc")).toElement();

    context->ViewScale = 1;
    context->Scale = symbolSizeFactor;
    context->hrsc = hrsc;
    context->encoding = svgBuilderParm->encoding;
    context->baseLineLength_px = svgBuilderParm->baseLineLength_px;
    context->Length = sizeof(IMGCONTEXT);
    ImgBorder imgBorder;
    context->imgBorder = &imgBorder;
    SvgDesc svgDesc(&svg_node, &g_node, &desc_node);
    context->svg_desc = &svgDesc;
    context->Size.cx = INT_MAX;
    context->Size.cy = INT_MAX;
    context->PixelX = 1./MKMINPIX;
    context->PixelY = 1./MKMINPIX;
    //context->PixelX = context->baseLineLength_px / context->baseLineLength;
    //context->PixelY = context->baseLineLength_px / context->baseLineLength;
    context->inImgVector = false;

    FRAME * border = new FRAME();

#if defined(__linux__)
    setlocale(LC_ALL, "POSIX");
#else
    setlocale(LC_ALL, "C");
#endif
    int res;
    try{
        res = 0;
        res = imgPrepareImage(data, type, parm,
          context, 0, border);
    } catch (MyException myException){
        delete border;
        delete[] PolyCounts;
        delete[] Points;
        delete data;
        delete context;
        throw MyException(myException);
    } catch (const char* message){
        delete border;
        delete[] PolyCounts;
        delete[] Points;
        delete data;
        delete context;
        throw MyException(message);
    }

    delete border;
    delete[] PolyCounts;
    delete[] Points;
    delete data;

    if (!res) {
        char* message = new char[255];
        sprintf(message,  "RscToSvg::getSvgImage error in imgPrepareImage");
        throw MyException(message);
    }

    beforeAddViewBox(symbolType, context, rotation);

    OBJECTHEADER* objectHeader = getObjectHeader(svgBuilderIndex, incode);
    double relUnitsToPix = 1.;
    if (objectHeader->Local != (int)LOCAL_LINE &&
        objectHeader->Local != (int)LOCAL_SQUARE&&
        objectHeader->Local != (int)LOCAL_TITLE) {
       relUnitsToPix = context->baseLineLength_px / 100.; // В нашем SVG все координты в процентом отношении к baseLineLength_px
    }

    imgBorder.addViewBox(&svg_node, relUnitsToPix, false);

    setlocale(LC_ALL, "");

    QString doc_s = doc.toString();
    QByteArray ba = doc_s.toLocal8Bit();
    const char* svg = ba.data();
    svgImage->svg = new char[strlen(svg) + 1];
    strcpy(svgImage->svg, svg);

    imgBorder.setSvgImageSize(svgImage, relUnitsToPix);
    delete context;

if (DEBUG_MODE > 2) printf("RscToSvg::getSvgImage svg=\n%s\n", svgImage->svg);
if (DEBUG_MODE  >  2) printf("RscToSvg::getSvgImage  end\n");
    return 1;
}

void RscToSvg::beforeAddViewBox(int symbolType,
                                IMGCONTEXT * context, double rotation){
    PostBuilder::process(symbolType, context, rotation);
}

long RscToSvg::getSegmentCount(long svgBuilderIndex){
    HRSC hrsc = getHrsc(svgBuilderIndex);

    long int result = imlGetRscSegmentCount(hrsc);

    return result;
}

long RscToSvg::getObjectCount(long svgBuilderIndex){
    HRSC hrsc = getHrsc(svgBuilderIndex);

    long int result = imlGetRscObjectCount(hrsc);

    return result;
}

long RscToSvg::getObject(long svgBuilderIndex, long incode, RSCOBJECT* rscObject){
    HRSC hrsc = getHrsc(svgBuilderIndex);

    long int result = imlGetRscObject(hrsc, incode, rscObject);

    return result;
}

OBJECTHEADER* RscToSvg::getObjectHeader(long svgBuilderIndex, long incode){
    HRSC hrsc = getHrsc(svgBuilderIndex);
    TRsc* rsc = (TRsc*)(void*)hrsc;
    OBJECTHEADER* result = rsc->getObjectByIncode(incode);

    return result;
}

long RscToSvg::getSegment(long svgBuilderIndex, RSCSEGMENT* rscSegment, long incode){
    HRSC hrsc = getHrsc(svgBuilderIndex);

    long int result = imlGetRscSegment(hrsc, rscSegment, incode);

    return result;
}

long RscToSvg::getSegmentLocalCount(long svgBuilderIndex, int* count, long incode, long local){
    HRSC hrsc = getHrsc(svgBuilderIndex);

    long int result = imlGetRscSegmentLocalCount(hrsc, count, incode, local);

    return result;
}

long RscToSvg::getSegmentLocalIncodes(long svgBuilderIndex, int** incodes, long incode, long local){
    HRSC hrsc = getHrsc(svgBuilderIndex);

    long int result = imlGetRscSegmentLocalIncodes(hrsc, incodes, incode, local);

    return result;
}

const char* RscToSvg::getSegmentName(long svgBuilderIndex, long incode){
    HRSC hrsc = getHrsc(svgBuilderIndex);

    const char* segmentName = imlGetRscSegmentName(hrsc, incode);
    return segmentName;
}

const char* RscToSvg::getSegmentShortName(long svgBuilderIndex, long incode){
    HRSC hrsc = getHrsc(svgBuilderIndex);

    const char* segmentShortName = imlGetRscSegmentShortName(hrsc, incode);
    return segmentShortName;
}

const char* RscToSvg::getObjectKey(long svgBuilderIndex, long incode){
    HRSC hrsc = getHrsc(svgBuilderIndex);

    const char* objectKey = imlGetRscObjectKey(hrsc, incode);
    return objectKey;
}


long RscToSvg::getSemanticClassCount(long svgBuilderIndex){
    HRSC hrsc = getHrsc(svgBuilderIndex);
    TRsc* rsc = (TRsc*)(void*)hrsc;
    int count = rsc->getSemanticClassCount();
    return count;
}

Class_SemClass_Map* RscToSvg::getSemanticClassByIndex(long svgBuilderIndex, long index){
    HRSC hrsc = getHrsc(svgBuilderIndex);
    TRsc* rsc = (TRsc*)(void*)hrsc;
    int code = rsc->getSemanticClassCodeByIndex(index);
    Class_SemClass_Map* semanticClass = rsc->getSemanticClassByCode(code);
    return semanticClass;
}

Class_SemClass_Map* RscToSvg::getSemanticClassByCode(long svgBuilderIndex, int semCode){
    HRSC hrsc = getHrsc(svgBuilderIndex);
    TRsc* rsc = (TRsc*)(void*)hrsc;
    Class_SemClass_Map* semanticClass = rsc->getSemanticClassByCode(semCode);
    return semanticClass;
}

long RscToSvg::getSimpleSemanticCount(long svgBuilderIndex){
    HRSC hrsc = getHrsc(svgBuilderIndex);
    TRsc* rsc = (TRsc*)(void*)hrsc;
    int count = rsc->getSimpleSemanticCount();
    return count;
}

SEMANTICHEADER* RscToSvg::getSimpleSemanticByIndex(long svgBuilderIndex, long index){
    HRSC hrsc = getHrsc(svgBuilderIndex);
    TRsc* rsc = (TRsc*)(void*)hrsc;
    int code = rsc->getSimpleSemanticCodeByIndex(index);
    SEMANTICHEADER* semHeader = rsc->getSemantic(code);
    return semHeader;
}


SEMANTICHEADER* RscToSvg::getSemanticByClassIndex(long svgBuilderIndex, long index){
    HRSC hrsc = getHrsc(svgBuilderIndex);
    TRsc* rsc = (TRsc*)(void*)hrsc;
    int code = rsc->getSemanticClassCodeByIndex(index);
    SEMANTICHEADER* semantic = rsc->getSemantic(code);
    return semantic;
}

SEMANTICHEADER* RscToSvg::getSemantic(long svgBuilderIndex, int semCode){
    HRSC hrsc = getHrsc(svgBuilderIndex);
    TRsc* rsc = (TRsc*)(void*)hrsc;
    SEMANTICHEADER* result = rsc->getSemantic(semCode);
    return result;
}

OBJECTSEMANTIC* RscToSvg::getObjectSemantics(long svgBuilderIndex, long excode, int local){
    HRSC hrsc = getHrsc(svgBuilderIndex);
    TRsc* rsc = (TRsc*)(void*)hrsc;
    OBJECTSEMANTIC* result = rsc->getObjectSemantics(excode, local);
    return result;
}

int* RscToSvg::getObligatorySemantics(long svgBuilderIndex, long excode, int local){
    HRSC hrsc = getHrsc(svgBuilderIndex);
    TRsc* rsc = (TRsc*)(void*)hrsc;
    int* result = rsc->getObligatorySemantics(excode, local);
    return result;
}

int* RscToSvg::getOptionalSemantics(long svgBuilderIndex, long excode, int local){
    HRSC hrsc = getHrsc(svgBuilderIndex);
    TRsc* rsc = (TRsc*)(void*)hrsc;
    int* result = rsc->getOptionalSemantics(excode, local);
    return result;
}

DEFAULTVALUE* RscToSvg::getSemanticDefaultValue(long svgBuilderIndex, long incode, int semCode){
    HRSC hrsc = getHrsc(svgBuilderIndex);
    TRsc* rsc = (TRsc*)(void*)hrsc;
    DEFAULTVALUE* result = rsc->getSemanticDefaultValue(incode, semCode);
    return result;
}

DEFAULTVALUE* RscToSvg::getSemanticDefaultValue(long svgBuilderIndex, int semCode){
    HRSC hrsc = getHrsc(svgBuilderIndex);
    TRsc* rsc = (TRsc*)(void*)hrsc;
    DEFAULTVALUE* result = rsc->getSemanticDefaultValue(semCode);
    return result;
}


char* RscToSvg::convertToLocal(long svgBuilderIndex, const char* text, char* buffer, size_t len_buffer){
    synchronized_begin();
    if (!_svgBuilderParms) {
        char* message = new char[255];
        sprintf(message,  "RscToSvg::mapGetRscObjectKey error: svgBuilderParms is NULL");
        synchronized_end();
        throw MyException(message);
    }
    if (svgBuilderIndex >= _svgBuilderParms_length) {
        char* message = new char[255];
        sprintf(message,  "RscToSvg::mapGetRscObjectKey error: svgBuilderIndex >= svgBuilderParms_length");
        synchronized_end();
        throw MyException(message);
    }
    SvgBuilderParm* svgBuilderParm = _svgBuilderParms[svgBuilderIndex];
    synchronized_end();

    HRSC hrsc = svgBuilderParm->hrsc;

    int encoding = svgBuilderParm->encoding;

    if (encoding == 0) {
      strcpy(buffer, text);
    } else {
      TRsc* rsc = (TRsc*)(void*)hrsc;
      #if defined(__linux__)
        int CP_UTF8 = 0; // внутри не используется
        char* buff =  rsc->convertTo(text, buffer, len_buffer, encoding, CP_UTF8);
      #else
        char* buff =  rsc->convertTo(text, buffer, len_buffer, encoding, 1251);
      #endif
      buffer = buff;
    }
    return buffer;
}

char* RscToSvg::convertToUtf8(long svgBuilderIndex, const char* text, char* buffer, size_t len_buffer){
    synchronized_begin();
    if (!_svgBuilderParms) {
        char* message = new char[255];
        sprintf(message,  "RscToSvg::mapGetRscObjectKey error: svgBuilderParms is NULL");
        synchronized_end();
        throw MyException(message);
    }
    if (svgBuilderIndex >= _svgBuilderParms_length) {
        char* message = new char[255];
        sprintf(message,  "RscToSvg::mapGetRscObjectKey error: svgBuilderIndex >= svgBuilderParms_length");
        synchronized_end();
        throw MyException(message);
    }
    SvgBuilderParm* svgBuilderParm = _svgBuilderParms[svgBuilderIndex];
    synchronized_end();

    HRSC hrsc = svgBuilderParm->hrsc;

    int encoding = svgBuilderParm->encoding;

    if (encoding == 0) {
      strcpy(buffer, text);
    } else {
      TRsc* rsc = (TRsc*)(void*)hrsc;
      #if defined(__linux__)
        int CP_UTF8 = 0; // внутри не используется
      #endif
      char* buff =  rsc->convertTo(text, buffer, len_buffer, encoding, CP_UTF8);
      buffer = buff;
    }
    return buffer;
}
