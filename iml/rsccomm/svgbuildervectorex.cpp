#if !defined(__linux__)
#include "stdafx.h"
#endif

#include "svgbuildervectorex.h"

SVGBuilderVectorEx::SVGBuilderVectorEx(
    QDomElement* parent_node,
    const POLYDATAEX * data,
	const char * parm,
    const IMGCONTEXT * context,
        VECTORMIX* mixing) :SVGBuilderVectorAbstr(parent_node, data, parm, context, mixing){

    imgvectorex = (IMGVECTOREX*)parm;
    init();
}

