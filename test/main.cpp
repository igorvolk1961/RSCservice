#if !defined(__linux__)
#include "stdafx.h"
#endif

#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#if !defined(__linux__)
#include <direct.h>
#endif
#include "mapgdi.h"
#include "imlserv0.h"
#include "rsctosvg.h"
#include "svgimage.h"
#include "rsc.h"
//#include "semantic.h"
#include "semblockinfo.h"
#include "myexception.h"


std::string do_replace(std::string const &in_s, char from, char to){
  const char* in_c = in_s.c_str();
  char out[strlen(in_c) + 1];
  strcpy(out, in_c);
  char* p = out;
  while (*p) {
      if (*p == from){
        *p = to;
      }
      ++p;
  }
  std::string out_s(out);
  return out_s;
}

using namespace std;

typedef std::unordered_map<int, RSCSEGMENT*> IncodeSegmentMap;
typedef std::pair<int, RSCSEGMENT*> IncodeSegmentPair;

int main(int argc, char** arg)
{    
    int debugMode = 0;
    int incode1;
    int incode_last;
    if (argc > 1) {
      int n = atoi(arg[1]);
      incode1 = n;
      incode_last = n;
    } else {
        incode1 = 1;
        incode_last = -1;
//        incode1 = 99;
//        incode_last = 99;
    }
    int encoding = TRsc::CONV_CP1251;
//    int encoding = TRsc::CONV_KOI8_R;

#if defined(__linux__)
    const char* rscFilePath_c = "/usr/Operator14/operator.rsc";
    const char* svgDirPath_c = "/usr/local/src/oper_svg";
//    const char* rscFilePath_c = "/usr/GIS-9.20.0/res/violit2016.rsc";
//    const char* svgDirPath_c = "/usr/local/src/violit_svg";
#else
    const char* rscFilePath_c = "c:/Program Files/Panorama/Operator13/operator.rsc";
    const char* svgDirPath_c = "d:/tmp/oper_svg";
#endif


//    const char* rscFilePath_c = "/media/sf_common/rsc/mgk_kps.rsc";
//    const char* svgDirPath_c = "/usr/local/src/mgk_svg";

//    const char* rscFilePath_c = "/media/sf_common/rsc/violit2003.rsc";
    //const char* svgDirPath_c = "/usr/local/src/violit_svg";

    std::string svgDirPath_s(svgDirPath_c);

    char* rscFilePath = new char[strlen(rscFilePath_c) + 1];
    strcpy(rscFilePath, rscFilePath_c);

    double baseVEFactor = 2.1;
    double baseLineLength = 7500 * baseVEFactor;
    double baseLineLength_px = 100;

    long svgBuilderIndex;
    try {
      svgBuilderIndex = RscToSvg::createSvgBuilder(rscFilePath, baseLineLength, baseLineLength_px,
                                                   debugMode, encoding);
      delete[] rscFilePath;
    } catch (MyException myException) {
        printf("%s\n", myException.d);
        return 0;
    } catch (const char* message) {
        printf("%s\n", message);
        return 0;
    } catch (...) {
        printf("%s\n", "Unknown excption");
        return 0;
    }

    size_t semanticSize = sizeof(SEMANTIC);

    SemBlockInfo* semBlockInfo_1 = SemBlockInfo::buildSemBlock(4, 1100, "02");
    semanticSize += semBlockInfo_1->semBlockSize;
    SemBlockInfo* semBlockInfo_2 = SemBlockInfo::buildSemBlock(4, 1300, "06");
    semanticSize += semBlockInfo_2->semBlockSize;
    SemBlockInfo* semBlockInfo_3 = SemBlockInfo::buildSemBlock(4, 5253, "09");
    semanticSize += semBlockInfo_3->semBlockSize;
    SemBlockInfo* semBlockInfo_4 = SemBlockInfo::buildSemBlock(4, 5400, "01");
    semanticSize += semBlockInfo_4->semBlockSize;
//        SemBlockInfo* semBlockInfo_5 = SemBlockInfo::buildSemBlock(4, 4100, "03");
    SemBlockInfo* semBlockInfo_5 = SemBlockInfo::buildSemBlock(4, 3200, "01");
    semanticSize += semBlockInfo_5->semBlockSize;

    char* semantic_c = new char[semanticSize];
    SEMANTIC* semantic = (SEMANTIC*) semantic_c;
    semantic->Clear();
    semantic->SetLength(semanticSize);

    char* p = semantic_c;
    p += sizeof(SEMANTIC);
    memcpy(p, semBlockInfo_1->semBlock, semBlockInfo_1->semBlockSize);
    p += semBlockInfo_1->semBlockSize;
    delete semBlockInfo_1;
    memcpy(p, semBlockInfo_2->semBlock, semBlockInfo_2->semBlockSize);
    p += semBlockInfo_2->semBlockSize;
    delete semBlockInfo_2;
    memcpy(p, semBlockInfo_3->semBlock, semBlockInfo_3->semBlockSize);
    p += semBlockInfo_3->semBlockSize;
    delete semBlockInfo_3;
    memcpy(p, semBlockInfo_4->semBlock, semBlockInfo_4->semBlockSize);
    p += semBlockInfo_4->semBlockSize;
    delete semBlockInfo_4;
    memcpy(p, semBlockInfo_5->semBlock, semBlockInfo_5->semBlockSize);
    p += semBlockInfo_5->semBlockSize;
    delete semBlockInfo_5;

     int metricCount_point = 1;
     int metricLengths_point[] = {4};
     double** metricCoords_point = new double*[1];
     metricCoords_point[0] = new double[4];
     metricCoords_point[0][0] = 0.;
     metricCoords_point[0][1] = 0.;

     metricCoords_point[0][2] = 100.;
     metricCoords_point[0][3] = 0.;

     int metricCount_line = 1;
     int metricLengths_line[] = {4};
     double** metricCoords_line = new double*[1];
     metricCoords_line[0] = new double[4];
     metricCoords_line[0][0] = 0;
     metricCoords_line[0][1] = 0;

     metricCoords_line[0][2] = 200;
     metricCoords_line[0][3] = 0;

//     metricCoords_line[0][4] = 40;
//     metricCoords_line[0][5] = -200;

//     metricCoords_line[0][6] = 100;
//     metricCoords_line[0][7] = -300;

     int metricCount_square = 1;
     int metricLengths_square[] = {10};
     double** metricCoords_square = new double*[1];
     metricCoords_square[0] = new double[10];
     metricCoords_square[0][0] = 0.,
     metricCoords_square[0][1] = 0.;

     metricCoords_square[0][2] = 0.;
     metricCoords_square[0][3] = -54.;

     metricCoords_square[0][4] = 108.;
     metricCoords_square[0][5] = -54.;

     metricCoords_square[0][6] = 108.;
     metricCoords_square[0][7] = 0.;

     metricCoords_square[0][8] = 0.;
     metricCoords_square[0][9] = 0.;
//     metricCoords_square[0][0] = 0.,
//     metricCoords_square[0][1] = 0.;

//     metricCoords_square[0][2] = 400.;
//     metricCoords_square[0][3] = 0.;

//     metricCoords_square[0][4] = 400.;
//     metricCoords_square[0][5] = 200.;

//     metricCoords_square[0][6] = 0.;
//     metricCoords_square[0][7] = 200.;

//     metricCoords_squimgRealDistanceare[0][8] = 0.;
//     metricCoords_square[0][9] = 0.;

//     int metricCount_text = 2;
     int metricCount_text = 1;
     int metricLengths_text[] = {4,4};
     double** metricCoords_text = new double*[2];
     metricCoords_text[0] = new double[4];
     metricCoords_text[1] = new double[4];
     metricCoords_text[0][0] = 0.;
     metricCoords_text[0][1] = 0.;
     metricCoords_text[0][2] = 100.;
     metricCoords_text[0][3] = 0.;
     metricCoords_text[1][2] = 0.;
     metricCoords_text[1][2] = 50.;
     metricCoords_text[1][2] = 100.;
     metricCoords_text[1][2] = 50.;

     char** texts = new char*[1];
     texts[0] = "Проба пера";


    long objectCount = RscToSvg::getObjectCount(svgBuilderIndex);

    IncodeSegmentMap segments;

    struct stat di;
    if (stat(svgDirPath_c, &di)) {
#if !defined(__linux__)
    int res = mkdir(svgDirPath_c);
#else
    int res = mkdir(svgDirPath_c, 0777);
#endif
    if (res == -1){
        printf("mkdir %s faild \n", svgDirPath_c);
        return 0;
      }
    }

    if (incode_last < 1) {
      if (incode1 != 1) {
        incode_last = incode1;
      } else {
        incode_last = objectCount;
      }
    }

    int bufferLen = 1024;
    char buffer[bufferLen];

    for (long incode = incode1; incode <= incode_last; ++incode){
      int res;
//    if (true) {
//      const char* key = "713226003024";
//      long incode = RscToSvg::getIncodeForKey(NULL, svgBuilderIndex, key);

//        if (incode == 62){
//             printf("oops\n");
//        }

      RSCOBJECT rscObject;
      RscToSvg::getObject(svgBuilderIndex, incode, (RSCOBJECT*)&rscObject);
      char* objectName_c = RscToSvg::convertToLocal(svgBuilderIndex, rscObject.Name, buffer, bufferLen);

      printf("%u %s\n", incode, objectName_c);

      std::string objectName_s(objectName_c);

      objectName_s = do_replace(objectName_s, '.', '_');
      objectName_s = do_replace(objectName_s, '/', '_');

      RSCSEGMENT* segment = segments[rscObject.Segment];
      if (!segment) {
          segment = new RSCSEGMENT();
          res = RscToSvg::getSegment(svgBuilderIndex, segment, rscObject.Segment-1);
          if (res == -1) {
              continue;
          }
          const char* segmentName = (char*)segment->Name;
          std::string segmentName_s(segmentName);
          segmentName_s = do_replace(segmentName_s, '.', '_');
          segmentName_s = do_replace(segmentName_s, '/', '_');
          strcpy((char*)segment->Name, segmentName_s.data());
          segments[rscObject.Segment] = segment;
      }
      char* segmentName_c = RscToSvg::convertToLocal(svgBuilderIndex, (char*)segment->Name, buffer, bufferLen);
      std::string segmentDirPath_s = svgDirPath_s + "/" +std::string(segmentName_c);

      if (stat(segmentDirPath_s.data(), &di)) {
#if !defined(__linux__)
        res = mkdir(segmentDirPath_s.data());
#else
        res = mkdir(segmentDirPath_s.data(), 0777);
#endif
        if (res == -1){
          printf("mkdir %s faild \n", segmentDirPath_s.data());
          return 0;
        }
      }
      const char* objectKey = RscToSvg::getObjectKey(svgBuilderIndex, incode);
      char* objectKey_c = RscToSvg::convertToLocal(svgBuilderIndex, objectKey, buffer, bufferLen);
      std::string fileName_s =  (objectKey_c) ? std::string(objectKey_c) + "_": "";

      if (fileName_s.substr(0,12) == "111045000133"){
          printf("yee \n");
      }

      fileName_s += objectName_s + ".svg";
      std::string svgFilePath = segmentDirPath_s + "/" + fileName_s;

      double** metricCoords;
      int* metricLengths;
      int metricCount;
      switch (rscObject.Local) {
      case LOCAL_LINE:{
          metricCoords = (double**)metricCoords_line;
          metricLengths = metricLengths_line;
          metricCount = metricCount_line;
          break;
      }
      case LOCAL_SQUARE:{
          metricCoords = (double**)metricCoords_square;
          metricLengths = metricLengths_square;
          metricCount = metricCount_square;
          break;
      }
      case LOCAL_POINT:{
          metricCoords = (double**)metricCoords_point;
          metricLengths = metricLengths_point;
          metricCount = metricCount_point;
          break;
      }
      case LOCAL_TITLE:{
          metricCoords = (double**)metricCoords_text;
          metricLengths = metricLengths_text;
          metricCount = metricCount_text;
          break;
      }
      case LOCAL_VECTOR:{
          metricCoords = (double**)metricCoords_point;
          metricLengths = metricLengths_point;
          metricCount = metricCount_point;
          break;
      }
      case LOCAL_MIXED:{
          metricCoords = (double**)metricCoords_point;
          metricLengths = metricLengths_point;
          metricCount = metricCount_point;
          break;
      }
      }

      try {
        try {
          SvgImage svgImage;
          RscToSvg::getSvgImageByKey(svgBuilderIndex, objectKey, metricCoords, metricLengths, metricCount,
                                semantic_c, texts, &svgImage, -1, 0., 1);
          const char* svg = svgImage.svg;
          const char* svgFilePath_c = svgFilePath.c_str();
          if (debugMode > 0) {
              printf("svgFilePath=%s\n", svgFilePath_c);
          }
          if (!svg) {
              printf("Oops\n");
          }
          FILE* svgFile = fopen(svgFilePath_c, "w");
          fwrite(svg, strlen(svg), 1, svgFile);
          fclose(svgFile);
          } catch (MyException myException) {
              throw MyException(myException);
          } catch (const char* message) {
              throw MyException(message);
          } catch (...) {
              throw MyException("UNKNOWN exception");
          }
      } catch (MyException myException) {
          printf("%s\n", myException.d);
          std::string error_s(myException.d);
          if (error_s.find("nknown") == -1) {
              printf("Program abnormally trerminated\n");
              delete[] metricCoords_point[0];
              delete[] metricCoords_point;
              delete[] metricCoords_line[0];
              delete[] metricCoords_line;
              delete[] metricCoords_square[0];
              delete[] metricCoords_square;
              delete[] metricCoords_text[0];
              delete[] metricCoords_text[1];
              delete[] metricCoords_text;
              for (auto it =  segments.begin(); it != segments.end(); ++it){
                  RSCSEGMENT* seg = it->second;
                  delete seg;
              }
              return 0;
          }
      }
    }

    delete[] metricCoords_point[0];
    delete[] metricCoords_point;
    delete[] metricCoords_line[0];
    delete[] metricCoords_line;
    delete[] metricCoords_square[0];
    delete[] metricCoords_square;
    delete[] metricCoords_text[0];
    delete[] metricCoords_text[1];
    delete[] metricCoords_text;

    delete[] semantic_c;

    for (auto it =  segments.begin(); it != segments.end(); ++it){
        RSCSEGMENT* seg = it->second;
        delete seg;
    }

    cout << "Done!" << endl;
    return 0;
}
