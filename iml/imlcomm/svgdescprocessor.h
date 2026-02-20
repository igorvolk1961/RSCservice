#ifndef SVGDESCPROCESSOR_H
#define SVGDESCPROCESSOR_H

#include "mapgdiex.h"

extern "C"
{
_IMLIMP void _IMLAPI svgDescAppendImg(IMGCONTEXT * context, long int type);
_IMLIMP void _IMLAPI svgDescLeaveNode(IMGCONTEXT * context, long int type);
}

#endif // SVGDESCPROCESSOR_H
