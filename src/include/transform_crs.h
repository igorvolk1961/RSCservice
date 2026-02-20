#ifndef TRANSFORM_CRS_H
#define TRANSFORM_CRS_H


class CRSTransformParams{
public:
    int type;
    double dx;
    double dy;
    double dz;
    double ex;
    double ey;
    double ez;
    double ppm;
    double a;
    double rf;
public:
    CRSTransformParams(int type, double dx, double dy, double dz,
                                 double ex, double ey, double ez,
                                 double ppm, double a, double rf)
    {
        this->type = type;
        this->dx = dx;
        this->dy = dy;
        this->dz = dz;
        this->ex = ex;
        this->ey = ey;
        this->ez = ez;
        this->ppm = ppm;
        this->a = a;
        this->rf = rf;
    };
};

void transformCRS(double& b, double& l, double& h, int ellipsoide_src, int ellipsoide_dst);

const int _KRASOVSKY42 = 1;
const int _WGS_76 = 2;
const int _HEFORD = 3;
const int _CLARKE_80 = 4;
const int _CLARKE_66 = 5;
const int _EVEREST_57 = 6;
const int _BESSEL = 7;
const int _AIRY = 8;
const int _WGS_84 = 9;

#endif // TRANSFORM_CRS_H
