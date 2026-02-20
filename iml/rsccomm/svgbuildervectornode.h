#ifndef SVGBUILDERVECTORNODE_H
#define SVGBUILDERVECTORNODE_H

#include "svgbuildertype.h"
#include "imgtype.h"
#include "imgsection.h"

class SVGBuilderVectorNode: public SVGBuilderType{
public:
    SVGBuilderVectorNode(
        QDomElement* parent_node,
        const POLYDATAEX * data,
        const char * parm,
        const IMGCONTEXT * context,
            VECTORMIX* mixing);
    ~SVGBuilderVectorNode();

    void build();
protected:
    ImgType* createImgType(IMGPOLYDESC* desc, HRSC hrsc, const POLYDATAEX * data, unsigned char imageFunc,
                           int index, const IMGCONTEXT* context);
private:
};

#endif // SVGBUILDERVECTORNODE_H
