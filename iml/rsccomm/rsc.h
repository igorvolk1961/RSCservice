#ifndef RSC_H
#define RSC_H

#if defined(__linux__)
#include <iconv.h>
#endif

#include <string>
#include <unordered_map>
#include "rscheader.h"
#include "myexception.h"

struct ExcodeLocal {
    int excode;
    int local;

    ExcodeLocal(int excode,int /*local*/){
      this->excode = excode;
        //this->local = local;
        this->local = 4;  // Есть проблемы
    }
    bool operator==(const ExcodeLocal &other) const{
        return (excode == other.excode) && (local == other.local);
    }
};

struct SegmentLocal {
    int segment;
    int local;

    SegmentLocal(int segment,int local){
      this->segment = segment;
      this->local = local;
    }
    bool operator==(const SegmentLocal &other) const{
        return (segment == other.segment) && (local == other.local);
    }
};

namespace std {
template <>
struct hash<ExcodeLocal> {
  std::size_t operator()(const ExcodeLocal &k) const {
      using std::size_t;
      using std::hash;
      using std::string;
      return hash<int>()(k.excode) ^
            (hash<int>()(k.local) << 1);
  }
};
template <>
struct hash<SegmentLocal> {
  std::size_t operator()(const SegmentLocal &k) const {
      using std::size_t;
      using std::hash;
      using std::string;
      return hash<int>()(k.segment) ^
            (hash<int>()(k.local) << 1);
  }
};
}

typedef std::unordered_map<int, SEGMENTHEADER*> Incode_Segment_Map;
typedef std::pair<int, SEGMENTHEADER*> Incode_Segment_Pair;

typedef std::unordered_map<std::string, OBJECTHEADER*> Key_Object_Map;
typedef std::pair<std::string, OBJECTHEADER*> Key_Object_Pair;

typedef std::unordered_map<ExcodeLocal, OBJECTHEADER*> ExcodeLocal_Object_Map;
typedef std::pair<ExcodeLocal, OBJECTHEADER*> ExcodeLocal_Object_Pair;

typedef std::unordered_map<int, SEMANTICHEADER*> SemCode_Semantic_Map;
typedef std::pair<int, SEMANTICHEADER*> SemCode_Semantic_Pair;

typedef std::unordered_map<int, SEMANTICCLASS*> Class_SemClass_Map;
typedef std::pair<int, SEMANTICCLASS*> Class_SemClass_Pair;

typedef std::unordered_map<int, Class_SemClass_Map*> SemCode_Class_SemClass_Map_Map;
typedef std::pair<int, Class_SemClass_Map*> SemCode_Class_SemClass_Map_Pair;

typedef std::unordered_map<int, DEFAULTVALUE*> SemCode_DefaultValue_Map;
typedef std::pair<int, DEFAULTVALUE*> SemCode_DefaultValue_Pair;

typedef std::unordered_map<int, SemCode_DefaultValue_Map*> Incode_DefaultValue_Map_Map;
typedef std::pair<int, SemCode_DefaultValue_Map*> Incode_DefaultValue_Map_Pair;

typedef std::unordered_map<ExcodeLocal, int*> ExcodeLocal_OptionalSemantics_Map;
typedef std::pair<ExcodeLocal, int*> ExcodeLocal_OptionalSemantics_Pair;

typedef std::unordered_map<ExcodeLocal, int*> ExcodeLocal_ObligatorySemantics_Map;
typedef std::pair<ExcodeLocal, int*> ExcodeLocal_ObligatorySemantics_Pair;

typedef std::unordered_map<ExcodeLocal, OBJECTSEMANTIC*> ExcodeLocal_ObjectSemantics_Map;
typedef std::pair<ExcodeLocal, OBJECTSEMANTIC*> ExcodeLocal_ObjectSemantics_Pair;

typedef std::unordered_map<ExcodeLocal, VALUEBOUNDARY*> ExcodeLocal_ValueBoundary_Map;
typedef std::pair<ExcodeLocal, VALUEBOUNDARY*> ExcodeLocal_ValueBoundary_Pair;

typedef std::unordered_map<SegmentLocal, int*> SegmentLocal_ObjectIncodes_Map;
typedef std::pair<SegmentLocal, int*> SegmentLocal_ObjectIncodes_Pair;

typedef std::unordered_map<SegmentLocal, int> SegmentLocal_ObjectCount_Map;
typedef std::pair<SegmentLocal, int> SegmentLocal_ObjectCount_Pair;

#ifndef MIN
  #define MIN(x, y) ((x)<(y))?(x):(y)
#endif

class TRsc{
public:
  TRsc(const char* rscFilePath);
  ~TRsc();

  RSCHEADER* getHeader();
  SEGMENTHEADER* getSegment(int index);
  PARAMHEADER* getParams(int incode);
  PALETTE* getPalette(int index);
  FONT* getFont(int index);
  LIBRARY* getLibrary(int index);

  OBJECTHEADER* getObjectByIncode(int incode);
  OBJECTHEADER* getObjectByKey(const char* key);
  OBJECTHEADER* getObjectByExcode(int excode, int local);
  SEMANTICHEADER* getSemantic(int semCode);
  Class_SemClass_Map* getSemanticClassByCode(int semCode);
  DEFAULTVALUE* getSemanticDefaultValue(long incode, int semCode);
  DEFAULTVALUE* getSemanticDefaultValue(int semCode);
  int* getObligatorySemantics(int excode, int local);
  int* getOptionalSemantics(int excode, int local);
  int* getSemantics(int excode, int local);
  VALUEBOUNDARY* getValueBoundary(int excode, int local);

  char* convertTo(const char* text, char* buffer, size_t len_buffer, int fromEncoding, int toCharset);

  double getBaseLineLength();
  void setBaseLineLength(double baseLineLength);
  double getBaseLineLength_px();
  void setBaseLineLength_px(double baseLineLength_px);

  int getSegmentLocalObjectCount(int segment, int local);
  int* getSegmentLocalObjectIncodes(int segment, int local);
  int getSemanticClassCount();
  int getSemanticClassCodeByIndex(int index);
  int getSimpleSemanticCount();
  int getSimpleSemanticCodeByIndex(int index);
  OBJECTSEMANTIC* getObjectSemantics(int excode, int local);

public:
  static const int CONV_DOS = 0;
  static const int CONV_KOI8_R = 1;
  static const int CONV_CP1251 = 126;
  static const int CONV_WCHAR_T = 127;
private:
  RSCHEADER* header;
  SEGMENTHEADER** segments;
  OBJECTHEADER** objects;
  PARAMHEADER** params;

  PALETTE* palettes;
  FONT* fonts;
  LIBRARY* libraries;

  Key_Object_Map keyObjectMap;
  ExcodeLocal_Object_Map excodeLocalObjectMap;
  SemCode_Semantic_Map semCodeSemanticMap;
  SemCode_Class_SemClass_Map_Map semCodeClassSemClassMapMap;
  SemCode_DefaultValue_Map semCodeDefaultValueMap;
  Incode_DefaultValue_Map_Map incodeDefaultValueMapMap;
  ExcodeLocal_ObjectSemantics_Map excodeLocalObjectSemanticsMap;
  ExcodeLocal_ObligatorySemantics_Map excodeLocalObligatorySemanticsMap;
  ExcodeLocal_OptionalSemantics_Map excodeLocalOptionalSemanticsMap;
  ExcodeLocal_ValueBoundary_Map excodeLocalValueBoundaryMap;

  SegmentLocal_ObjectCount_Map segmentLocalObjectCount_Map;
  SegmentLocal_ObjectIncodes_Map segmentLocalObjectIncodes_Map;
  int semanticClassCount = 0;
  int* semanticClassCodes;
  int simpleSemanticCount = 0;
  int* simpleSemanticCodes;


#if defined(__linux__)
  iconv_t iconv_cd_cp1251;
  iconv_t iconv_cd_koi8_r;
  iconv_t iconv_cd_dos;
  iconv_t iconv_cd_wchar_t;
#endif
  double baseLineLength;
  double baseLineLength_px;
};

#endif // RSC_H
