#ifndef GISAPI_H
#define GISAPI_H

#include "mapsyst.h"
#include "vecexapi.h"

typedef long int  (WINAPI * GET_INFO_SXF)(char*, INFOSXF*);
GET_INFO_SXF _GetInfoSxf;
typedef long int  (WINAPI * MAP_GET_SXF_INFO_BY_NAME)(const char*, MAPREGISTER*, LISTREGISTER*);
MAP_GET_SXF_INFO_BY_NAME _mapGetSxfInfoByName;

#endif // GISAPI_H
