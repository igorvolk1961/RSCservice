#ifndef SVGBUILDERVECTORABSTR_H
#define SVGBUILDERVECTORABSTR_H

#include "imgtype.h"
#include "svgbuilder.h"

class SVGBuilderVectorAbstr: public SVGBuilder{
public:
    SVGBuilderVectorAbstr(
        QDomElement* parent_node,
        const POLYDATAEX * data,
        const char * parm,
        const IMGCONTEXT * context,
            VECTORMIX* mixing);

    virtual void build();
protected:
    void init();
    void addShape(IMGPOLYDESC* desc);
protected:
    virtual const char* getSymbolTag();
protected:
    IMGVECTOREX* imgvectorex;
    double rotation;
};

#endif // SVGBUILDERVECTORABSTR_H
