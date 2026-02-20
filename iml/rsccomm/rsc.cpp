#if !defined(__linux__)
#include "stdafx.h"
#endif

#include <string.h>
#include <wchar.h>
#include <stdio.h>
#include <sys/stat.h>
#include "rsc.h"
#include <QDebug>

const int LOCAL_COUNT = 6;

TRsc::TRsc(const char* rscFilePath){
    struct stat fi;
    stat(rscFilePath, &fi);
    size_t fileSize = fi.st_size;

    char* data = new char[fileSize];
    FILE* rscFile = fopen(rscFilePath, "rb");
    if (!rscFile) {
        char message[255];
        sprintf(message, "File %s not found", rscFilePath);
        throw MyException(message);
    }
    size_t fileSize1 = fread(data, 1, fileSize, rscFile);
    fclose(rscFile);
    if (fileSize1 != fileSize) {
        char message[255];
        sprintf(message, "Loading of file %s failed", rscFilePath);
        throw MyException(message);
    }

    header = (RSCHEADER*) data;

    segments = new SEGMENTHEADER*[header->SegmentCount];
    char* p = data + header->SegmentOffset;
    for (int j = 0; j< header->SegmentCount; ++j){
        SEGMENTHEADER* segHeader = (SEGMENTHEADER*) p;
        segments[j] = segHeader;
        p += segHeader->RecordLength;
        for (int m = 0; m < LOCAL_COUNT; ++m){
            segmentLocalObjectCount_Map.insert(SegmentLocal_ObjectCount_Pair(SegmentLocal(j, m), 0));
        }
    }

    objects = new OBJECTHEADER*[header->ObjectCount];
    p = data + header->ObjectOffset;
    for (int j = 0; j< header->ObjectCount; ++j){
        OBJECTHEADER* objHeader = (OBJECTHEADER*) p;
        objects[j] = objHeader;
        p += objHeader->RecordLength;
        if (objHeader->Excode == 0) {
            int xxx = 0;
        }

        excodeLocalObjectMap.insert(ExcodeLocal_Object_Pair(ExcodeLocal((int)objHeader->Excode, (int)objHeader->Local), objHeader));
        std::string key(objHeader->ShortName);
        keyObjectMap.insert(Key_Object_Pair(key, objHeader));

        SegmentLocal sl(objHeader->Segment, objHeader->Local);
        int objectCount = segmentLocalObjectCount_Map[sl];
        segmentLocalObjectCount_Map[sl] = objectCount + 1;
    }

    params = new PARAMHEADER*[header->ParameterCount];
    p = data + header->ParameterOffset;
    for (int j = 0; j< header->ParameterCount; ++j){
        PARAMHEADER* paramHeader = (PARAMHEADER*) p;
        params[j] = paramHeader;
        p += paramHeader->RecordLength;
    }

    p = data + header->PaletteOffset;
    palettes = (PALETTE*) p;

    p = data + header->FontOffset;
    fonts = (FONT*) p;

    p = data + header->LibraryOffset;
    libraries = (LIBRARY*) p;

    semanticClassCount = 0;
    simpleSemanticCount = 0;
    p = data + header->SemanticOffset;
    SEMANTICHEADER* semanticHeader = (SEMANTICHEADER*) p;
    for (int j = 0; j < header->SemanticCount; ++j) {
        semCodeSemanticMap.insert(SemCode_Semantic_Pair(semanticHeader[j].Code, &semanticHeader[j]));
        SEMANTICCLASS* semClass = (SEMANTICCLASS*)(data + semanticHeader[j].ClassifierOffset);
        int semClassCount = semanticHeader[j].ClassifierCount;
        if (semClassCount > 0) {
          Class_SemClass_Map* class_SemClass_Map = new Class_SemClass_Map();
          semCodeClassSemClassMapMap.insert(SemCode_Class_SemClass_Map_Pair(semanticHeader[j].Code, class_SemClass_Map));
          for (int m = 0; m < semClassCount; ++m){
            class_SemClass_Map->insert(Class_SemClass_Pair(semClass->Value, semClass));
            semClass++;
          }
          semanticClassCount++;
        } else {
            if (!semanticHeader[j].Service) {
              simpleSemanticCount++;
            }
        }
        DEFAULTVALUE* defaultValue = (DEFAULTVALUE*)(data + semanticHeader[j].DefaultValueOffset);
        if (semanticHeader[j].DefaultValueCount) {
          semCodeDefaultValueMap.insert(SemCode_DefaultValue_Pair(semanticHeader[j].Code, defaultValue));
        }
    }

    p = data + header->DefaultValueOffset;
    DEFAULTVALUE* defaultValue = (DEFAULTVALUE*) p;
    for (int j = 0; j < header->DefaultValueCount; ++j){
       int incode =  defaultValue[j].Incode;       
       if (incode) {
          SemCode_DefaultValue_Map* semCodeDefaultValueMap_ = incodeDefaultValueMapMap[incode];
          if (!semCodeDefaultValueMap_){
              semCodeDefaultValueMap_ = new SemCode_DefaultValue_Map();
              incodeDefaultValueMapMap.insert(Incode_DefaultValue_Map_Pair(incode, semCodeDefaultValueMap_));
          }
          semCodeDefaultValueMap_->insert(SemCode_DefaultValue_Pair(defaultValue[j].Code, &defaultValue[j]));
       }
    }

    p = data + header->ObjectSemanticOffset;
    for (int j = 0; j < header->ObjectSemanticCount; ++j){
        OBJECTSEMANTIC* objectSemantic = (OBJECTSEMANTIC*) p;
        ExcodeLocal excodeLocal((int)objectSemantic->Excode, (int)objectSemantic->Local);
        excodeLocalObjectSemanticsMap.insert(ExcodeLocal_ObjectSemantics_Pair(excodeLocal, objectSemantic));
        if (objectSemantic->ObligatorySemanticCount) {
            int* objectObligatorySemantics = (int*) (p + sizeof(OBJECTSEMANTIC));
            excodeLocalObligatorySemanticsMap.insert(ExcodeLocal_ObligatorySemantics_Pair(excodeLocal, objectObligatorySemantics));
        }
        if (objectSemantic->OptionalSemanticCount) {
            int* objectOptionalSemantics = (int*) (p + sizeof(OBJECTSEMANTIC) + 4 * objectSemantic->ObligatorySemanticCount);
            excodeLocalOptionalSemanticsMap.insert(ExcodeLocal_OptionalSemantics_Pair(excodeLocal, objectOptionalSemantics));
        }
        p += objectSemantic->RecordLength;
    }

    p = data + header->ValueBoundaryOffset;
    for (int j=0; j < header->ValueBoundaryCount; ++j){
        VALUEBOUNDARY* valueBoundary = (VALUEBOUNDARY*)p;
        excodeLocalValueBoundaryMap.insert(ExcodeLocal_ValueBoundary_Pair(ExcodeLocal((int)valueBoundary->Excode, (int)valueBoundary->Local),
                                           valueBoundary));
        p += valueBoundary->RecordLength;
    }


    // Заполняем структуру, используемую для получения объектов слоя с определенной геометрией (локализацией)
        for (int j = 0; j< header->SegmentCount; ++j){
            for (int m = 0; m < LOCAL_COUNT; ++m){
                int segment = j;
                SegmentLocal sl(segment , m);
                int objectCount = segmentLocalObjectCount_Map[sl];
                segmentLocalObjectIncodes_Map[sl] = new int[objectCount];
                segmentLocalObjectCount_Map[sl] = 0;
            }
        }
        p = data + header->ObjectOffset;
        for (int j = 0; j< header->ObjectCount; ++j){
            OBJECTHEADER* objHeader = (OBJECTHEADER*) p;
            objects[j] = objHeader;
            p += objHeader->RecordLength;
            if (objHeader->Excode == 0) {
                int xxx = 0;
            }

            SegmentLocal sl(objHeader->Segment, objHeader->Local);
            int objectCount = segmentLocalObjectCount_Map[sl];
            int* objectIncodes = segmentLocalObjectIncodes_Map[sl];
            objectIncodes[objectCount] = objHeader->Incode;

            segmentLocalObjectCount_Map[sl] = objectCount + 1;
        }


        semanticClassCodes = new int[semanticClassCount];
        simpleSemanticCodes = new int[simpleSemanticCount];
        p = data + header->SemanticOffset;
        semanticHeader = (SEMANTICHEADER*) p;
        semanticClassCount = 0;
        simpleSemanticCount = 0;
        for (int j = 0; j < header->SemanticCount; ++j) {
            SEMANTICCLASS* semClass = (SEMANTICCLASS*)(data + semanticHeader[j].ClassifierOffset);
            int semClassCount = semanticHeader[j].ClassifierCount;
            int semCode = semanticHeader[j].Code;
            if (semClassCount > 0) {
                semanticClassCodes[semanticClassCount] = semCode;
                semanticClassCount++;
            } else
            if (!semanticHeader[j].Service){
                simpleSemanticCodes[simpleSemanticCount] = semCode;
                simpleSemanticCount++;
            }
        }

#if defined(__linux__)
    iconv_cd_cp1251 = 0;
    iconv_cd_koi8_r = 0;
    iconv_cd_dos = 0;
    iconv_cd_wchar_t = 0;
#endif
}

TRsc::~TRsc(){
    for (auto it =  semCodeClassSemClassMapMap.begin(); it != semCodeClassSemClassMapMap.end(); ++it){
        Class_SemClass_Map* record = it->second;
        delete record;
    }
    delete[] header;
    delete[] segments;
    delete[] objects;
    delete[] params;
    delete[] semanticClassCodes;
    delete[] simpleSemanticCodes;

#if defined(__linux__)
    if (iconv_cd_cp1251) {
      iconv_close(iconv_cd_cp1251);
    }
    if (iconv_cd_koi8_r){
      iconv_close(iconv_cd_koi8_r);
    }
    if (iconv_cd_dos){
      iconv_close(iconv_cd_dos);
    }
    if (iconv_cd_wchar_t){
      iconv_close(iconv_cd_wchar_t);
    }
#endif
}


RSCHEADER* TRsc::getHeader(){
    return header;
}

SEGMENTHEADER* TRsc::getSegment(int incode){
    if (incode < 0) {
        char message[255];
        sprintf(message, "TRsc::getSegment:  incode < 0");
        throw MyException(message);
    }
    return segments[incode];
}

PARAMHEADER* TRsc::getParams(int incode){
    if (incode == 0) {
        return NULL;
    } else {
      return params[incode - 1];
    }
}

PALETTE* TRsc::getPalette(int index){
    if (index == 0) {
        char message[255];
        sprintf(message, "TRsc::getPalette:  index == 0");
        throw MyException(message);
    }
    return &palettes[index - 1];
}

FONT* TRsc::getFont(int index){
    if (index == 0) {
        char message[255];
        sprintf(message, "TRsc::getFont:  index == 0");
        throw MyException(message);
    }
    return &fonts[index - 1];
}

LIBRARY* TRsc::getLibrary(int index){
    if (index == 0) {
        char message[255];
        sprintf(message, "TRsc::getLibrary:  index == 0");
        throw MyException(message);
    }
    return &libraries[index - 1];
}

OBJECTHEADER* TRsc::getObjectByIncode(int incode){
    if (incode == 0) {
        char message[255];
        sprintf(message, "TRsc::getObjectByIncode:  incode == 0");
        throw MyException(message);
    }
    return objects[incode - 1];
}

OBJECTHEADER* TRsc::getObjectByKey(const char* key){
    if (keyObjectMap.find(std::string(key)) == keyObjectMap.end()){
        return NULL;
    } else {
        return keyObjectMap[std::string(key)];
    }
}

OBJECTHEADER* TRsc::getObjectByExcode(int excode, int local){
    ExcodeLocal excodeLocal = ExcodeLocal(excode, local);
    if (excodeLocalObjectMap.find(excodeLocal) == excodeLocalObjectMap.end()){
      return NULL;
    } else {
      return excodeLocalObjectMap[excodeLocal];
    }
}

SEMANTICHEADER* TRsc::getSemantic(int semCode){
    if (semCodeSemanticMap.find(semCode) == semCodeSemanticMap.end()) {
      return NULL;
    } else {
      return semCodeSemanticMap[semCode];
    }
}


Class_SemClass_Map* TRsc::getSemanticClassByCode(int semCode){
    if (semCodeClassSemClassMapMap.find(semCode) == semCodeClassSemClassMapMap.end()){
      return NULL;
    } else {
      return semCodeClassSemClassMapMap[semCode];
    }
}

DEFAULTVALUE* TRsc::getSemanticDefaultValue(int semCode){
    if (semCodeDefaultValueMap.find(semCode) == semCodeDefaultValueMap.end()) {
      return NULL;
    } else {
      return semCodeDefaultValueMap.at(semCode);
    }
}

DEFAULTVALUE* TRsc::getSemanticDefaultValue(long incode, int semCode){
    if (incodeDefaultValueMapMap.find(incode) == incodeDefaultValueMapMap.end()) {
        return NULL;
    }
    SemCode_DefaultValue_Map* defaultValue_Map = incodeDefaultValueMapMap.at(incode);
    if (defaultValue_Map->find(semCode) == defaultValue_Map->end()) {
      return defaultValue_Map->at(semCode);
    } else{
      return NULL;
    }
}

int* TRsc::getObligatorySemantics(int excode, int local){
    ExcodeLocal excodeLocal = ExcodeLocal(excode, local);
    if (excodeLocalObligatorySemanticsMap.find(excodeLocal) == excodeLocalObligatorySemanticsMap.end()){
        return NULL;
    } else {
      return excodeLocalObligatorySemanticsMap[excodeLocal];
    }
}

int* TRsc::getOptionalSemantics(int excode, int local){
    ExcodeLocal excodeLocal = ExcodeLocal(excode, local);
    if (excodeLocalOptionalSemanticsMap.find(excodeLocal) == excodeLocalOptionalSemanticsMap.end()){
      return NULL;
    } else {
      return excodeLocalOptionalSemanticsMap[excodeLocal];
    }
}

VALUEBOUNDARY* TRsc::getValueBoundary(int excode, int local){
    ExcodeLocal excodeLocal = ExcodeLocal(excode, local);
    return excodeLocalValueBoundaryMap[ExcodeLocal(excode, local)];
}

char* TRsc::convertTo(const char* text, char* buffer, size_t len_buffer, int fromEncoding, int toCharset){
#if defined(__linux__)
    iconv_t iconv_cd;
    switch (fromEncoding) {
    case CONV_DOS: {
        if (iconv_cd_dos == 0){
            iconv_cd_dos = iconv_open("UTF-8", "866");
        }
        iconv_cd = iconv_cd_dos;
        break;
    }
    case CONV_KOI8_R: {
        if (iconv_cd_koi8_r == 0){
            iconv_cd_koi8_r = iconv_open("UTF-8", "KOI8-R");
        }
        iconv_cd = iconv_cd_koi8_r;
        break;
    }
    case CONV_CP1251: {
        if (iconv_cd_cp1251 == 0){
            iconv_cd_cp1251 = iconv_open("UTF-8", "cp1251");
        }
        iconv_cd = iconv_cd_cp1251;
        break;
    }
    case CONV_WCHAR_T: {
        if (iconv_cd_wchar_t == 0){
            iconv_cd_wchar_t = iconv_open("UTF-8", "WCHAR_T");
        }
        iconv_cd = iconv_cd_wchar_t;
        break;
    }
    default: iconv_cd = 0;
    }

    size_t len_src;
    if (fromEncoding == CONV_WCHAR_T) {
      len_src = wcslen((wchar_t*)text) * sizeof(wchar_t);
    } else {
      len_src = strlen(text);
    }
    size_t len_dst = len_buffer;
    memset(buffer, 0, len_buffer);
    if (iconv_cd == 0) {
        strcpy(buffer, text);
        return buffer;
    }

    char* in = const_cast<char*>(text);
    char* out = buffer;
    size_t res = iconv(iconv_cd, &in, &len_src,
                 &out, &len_dst);
    if (res == -1){
        const char* message;
        switch (errno){
        case E2BIG:
          message = "Error in semantic convertion to koi8: errno=E2BIG";
          break;
        case EILSEQ:
          message = "Error in semantic convertion to koi8: errno=EILSEQ";
            break;
        case EINVAL:
          message = "Error in semantic convertion to koi8: errno=EINVAL";
            break;
        default:
          message = "Error in semantic convertion to koi8: errno=%2";
        }
        throw MyException(message);
    }
#else
    int formCharset;
    switch (fromEncoding) {
    case CONV_DOS: {
        formCharset = 866 ;
        break;
    }
    case CONV_KOI8_R: {
        formCharset = 20866;
        break;
    }
    case CONV_CP1251: {
        if(toCharset == 1251) {
          strcpy(buffer, text);
          return buffer;
        } else {
          formCharset = 1251;
          break;
        }
    }
    case CONV_UTF8: {
        formCharset = CP_UTF8;
        break;
    }
    default: formCharset = 1251;
    }
    int result_u;
    result_u = MultiByteToWideChar(formCharset, 0, text, -1, 0, 0);
    if (!result_u) {
       throw MyException("unknown_encoding");
    }
    wchar_t *ures = new wchar_t[result_u];
    if (!MultiByteToWideChar(formCharset, 0, text, -1, ures, result_u)) {
       delete[] ures;
       throw MyException("error_in_encoding_conversion");
    }
    int result_c;
    result_c = WideCharToMultiByte(toCharset, 0, ures, -1, 0, 0, 0, 0);
    if (!result_c) {
      delete[] ures;
      throw MyException("unknown_encoding1");
    }
    int result_b = WideCharToMultiByte(toCharset, 0, ures, -1, buffer, len_buffer, 0, 0);
    delete[] ures;
    if (!result_b) {
       throw MyException("error_in_encoding_conversion1");
    }
#endif
    return buffer;
}


double TRsc::getBaseLineLength(){
    return baseLineLength;
}

void TRsc::setBaseLineLength(double baseLineLength){
    this->baseLineLength = baseLineLength;
}

double TRsc::getBaseLineLength_px(){
    return baseLineLength_px;
}

void TRsc::setBaseLineLength_px(double baseLineLength_px){
    this->baseLineLength_px = baseLineLength_px;
}

int TRsc::getSegmentLocalObjectCount(int segment, int local){
    SegmentLocal segmentLocal = SegmentLocal(segment, local);
    if (segmentLocalObjectCount_Map.find(segmentLocal) == segmentLocalObjectCount_Map.end()) {
        return NULL;
    } else {
        return segmentLocalObjectCount_Map[segmentLocal];
    }
}

int* TRsc::getSegmentLocalObjectIncodes(int segment, int local){
    SegmentLocal segmentLocal = SegmentLocal(segment, local);
    if (segmentLocalObjectIncodes_Map.find(segmentLocal) == segmentLocalObjectIncodes_Map.end()) {
        return NULL;
    } else {
        return segmentLocalObjectIncodes_Map[segmentLocal];
    }
}

int TRsc::getSemanticClassCount(){
    return semanticClassCount;
}

int TRsc::getSemanticClassCodeByIndex(int index){
    return semanticClassCodes[index];
}

int TRsc::getSimpleSemanticCount(){
    return simpleSemanticCount;
}

int TRsc::getSimpleSemanticCodeByIndex(int index){
    return simpleSemanticCodes[index];
}

OBJECTSEMANTIC* TRsc::getObjectSemantics(int excode, int local){
    ExcodeLocal excodeLocal = ExcodeLocal(excode, local);
    if (excodeLocalObjectSemanticsMap.find(excodeLocal) == excodeLocalObjectSemanticsMap.end()) {
        return NULL;
    } else {
        return excodeLocalObjectSemanticsMap[excodeLocal];
    }
}
