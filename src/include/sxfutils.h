#ifndef SXF_UTILS_H
#define SXF_UTILS_H

#include "mapcreat.h"

class QString;

const int PSEUDOMERCATOR = -2;

HMAP transformToWebMercator(HMAP hmap, bool createNewIfExists);
HMAP loadSxf(const char* c_sxfFilePath, const char* c_mapFilePath, const char* c_rscFilePath,
             bool transformToPseudoMercator);
HMAP loadDir(const char* c_dirFilePath, const char* c_mapFilePath,
             bool transformToPseudoMercator);
void loadMap(const char* c_mapFilePath,
             bool transformToPseudoMercator);
void clearDir(const QString &dirPath);


int getZone(LISTREGISTER* list, int scale);
void error(const QString &message);
void warning(const QString &message);
void debug(const QString &message);
void info(const QString &message);
void log(const QString &message, int logLevel);
int bl2PseudoMercator(double b, double l, double* x, double* y);
bool equalMaps(MAPREGISTEREX* m0, MAPREGISTEREX* m1);
void transform(double* x, double* y, HMAP hmap, MAPREGISTEREX* m_src, MAPREGISTEREX* m_dst,
               bool transformToPseudoMercator);
void transformToGeo(double xx, double yy, double& bb, double& ll, HMAP hmap, MAPREGISTEREX* m_src);
void transformFromGeo(double& b, double& l, double& x, double& y, HMAP hmap, MAPREGISTEREX* m_src, MAPREGISTEREX* m_dst,
               bool transformToPseudoMercator);
void buildMercatorMap(MAPREGISTEREX* map);
long int calcEPSG(MAPREGISTEREX* mapreg);

#endif // SXF_UTILS_H
