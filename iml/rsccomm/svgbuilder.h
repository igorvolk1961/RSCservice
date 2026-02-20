#ifndef SVGBUILDER_H
#define SVGBUILDER_H

#undef __STRICT_ANSI__
#include <QtXml>

#ifndef MAPGDI_H
#include "mapgdi.h"
#endif

#include "imgborder.h"
#include "imlservex.h"
#include "debug.h"

#ifndef MM_IN_MKM
  #define MM_IN_MKM 0.001
#endif

class SVGBuilder{
public:
    SVGBuilder(
               QDomElement* parent_node,
               const POLYDATAEX * data,
               const char * parm,
               const IMGCONTEXT * context,
            VECTORMIX* mixing);
	virtual ~SVGBuilder();

	virtual void build();
public:
    ImgBorder* getImgBorder();
public:
    static void buildImage(const char* parm, unsigned short type,
                           const POLYDATAEX * data,
                           const IMGCONTEXT * context,
                           VECTORMIX* mixing);
protected:
    VECTORMIX*   getMixing();
protected: 
    double posH;
    double posV;

    double ve_v_factor_x;
    double ve_v_factor_y;

    double baseLineLength_px;

    double scale_coeff_x;
    double scale_coeff_y;
    double scale_coeff_thick;

	double dx;
    double dy;

    HRSC hrsc;
    const IMGCONTEXT * context;
    QDomDocument* doc;
    QDomElement* svg_node;
    QDomElement* parent_node;
    QDomElement* current_node;
    const POLYDATAEX * data;
    const char * parm;
    VECTORMIX*   mixing;
};

#endif
