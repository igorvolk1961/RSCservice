#if !defined(__linux__)
#include "stdafx.h"
#endif

#include <QtXml>
#include "postbuilder.h"
#include <stdio.h>
#include <string>
#include <cmath>

void PostBuilder::process(int symbolType,
                          IMGCONTEXT* context, double rotation) {
    if (fabs(rotation) > 0.001) {
        char transform[256];
        sprintf(transform, "rotate(%0.3f)",rotation);
        QDomElement* g_node = context->svg_desc->getG_node();
        g_node->setAttribute("transform", transform);

        context->imgBorder->rotate(rotation, 0, 0);
    }
}


