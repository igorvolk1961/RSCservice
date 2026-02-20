#ifndef RSC_TO_SVG
#define RSC_TO_SVG

#include <stddef.h>
#include "rsc.h"
#include "rscheader.h"
#include "maptype.h"

class RSCOBJECT;
class RSCSEGMENT;
class IMGCONTEXT;
class SvgImage;

class RscToSvg{
public:
    RscToSvg();
public:
    static long createSvgBuilder(const char* rscFilePath, double baseLineLength, double baseLineLength_px,
                                 int debug_mode, int encoding);
    static HRSC getHrsc(long svgBuilderIndex);
    static void deleteSvgBuilder(long svgBuilderIndex);
    static long getIncodeForKey(long svgBuilderIndex, const char* key);
    static long getIncodeForExcode(long svgBuilderIndex, long excode, int local);
    static long getSvgImageByKey(long svgBuilderIndex, const char* key, double** metricCoords, int* metricLengths, int metricCount,
                                 char* semantics, char** texts, SvgImage* svgImage, int symbolType,
                                 double rotation, double symbolSizeFactor);
    static long getSvgImageByIncode(long svgBuilderIndex, long incode, double** metricCoords, int* metricLengths, int metricCount,
                                    char* semantics, char** texts, SvgImage* svgImage, int symbolType,
                                    double rotation, double symbolSizeFactor);
    static long getSegmentCount(long svgBuilderIndex);
    static long getObjectCount(long svgBuilderIndex);
    static long getObject(long svgBuilderIndex, long incode, RSCOBJECT* rscObject);
    static OBJECTHEADER* getObjectHeader(long svgBuilderIndex, long incode);
    static long getSegment(long svgBuilderIndex, RSCSEGMENT* rscSegment, long incode);
    static long getSegmentLocalCount(long svgBuilderIndex, int* count, long incode, long local);
    static long getSegmentLocalIncodes(long svgBuilderIndex, int** incodes, long incode, long local);
    static const char* getSegmentName(long svgBuilderIndex, long incode);
    static const char* getSegmentShortName(long svgBuilderIndex, long incode);
    static const char* getObjectKey(long svgBuilderIndex, long incode);
    static long getSemanticClassCount(long svgBuilderIndex);
    static Class_SemClass_Map* getSemanticClassByIndex(long svgBuilderIndex, long index);
    static Class_SemClass_Map* getSemanticClassByCode(long svgBuilderIndex, int semCode);
    static SEMANTICHEADER* getSemanticByClassIndex(long svgBuilderIndex, long index);
    static long getSimpleSemanticCount(long svgBuilderIndex);
    static SEMANTICHEADER* getSimpleSemanticByIndex(long svgBuilderIndex, long index);
    static char* convertToLocal(long svgBuilderIndex, const char* text,  char* buffer, size_t len_buffer);
    static char* convertToUtf8(long svgBuilderIndex, const char* text,  char* buffer, size_t len_buffer);
    static void beforeAddViewBox(int symbolType, IMGCONTEXT* context, double rotation);

    static SEMANTICHEADER* getSemantic(long svgBuilderIndex, int semCode);
    static OBJECTSEMANTIC* getObjectSemantics(long svgBuilderIndex, long excode, int local);
    static int* getObligatorySemantics(long svgBuilderIndex, long excode, int local);
    static int* getOptionalSemantics(long svgBuilderIndex, long excode, int local);
    static DEFAULTVALUE* getSemanticDefaultValue(long svgBuilderIndex, long incode, int semCode);
    static DEFAULTVALUE* getSemanticDefaultValue(long svgBuilderIndex, int semCode);
};

#endif
