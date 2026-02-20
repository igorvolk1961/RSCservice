#ifndef SVGBUILDERLIBRARY_H
#define SVGBUILDERLIBRARY_H

#include "imgtype.h"
#include "svgbuilder.h"

class SVGBuilderLibrary: public SVGBuilder{
public:
    SVGBuilderLibrary(
        QDomElement* parent_node,
        const POLYDATAEX * data,
        const char * parm,
        const IMGCONTEXT * context,
            VECTORMIX* mixing);

    virtual void build();
private:
    IMGLIBRARY* imglibrary;
};

#endif // SVGBUILDERLIBRARY_H
