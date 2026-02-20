#ifndef SVGBUILDERDRAWABSTR_H
#define SVGBUILDERDRAWABSTR_H

#include "svgbuilder.h"
#include "imgtype.h"
#include "maptype.h"
#include "mapgdi.h"
#include "debug.h"

class SVGBuilderDrawAbstr: public SVGBuilder{
public:
    SVGBuilderDrawAbstr(
        QDomElement* parent_node,
        const POLYDATAEX * data,
        const char * parm,
        const IMGCONTEXT * context,
            VECTORMIX* mixing);

    virtual void build();
private:
    void buildImage(char* parm, unsigned short type);
protected:
    IMGDRAW* imgDraw;
};

#endif // SVGBUILDERDRAWABSTR_H
