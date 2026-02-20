#ifndef IMGBORDER_H
#define IMGBORDER_H

//#include <QtXml>

class SvgImage;
class QDomElement;

class ImgBorder {
public:
    ImgBorder();
    double minX;
    double minY;
    double maxX;
    double maxY;
    double maxThick;
    bool withTexts;
    void init();
    void update(double x, double y);
    void extend(double value);
    void expand(ImgBorder* imgBorder);
    void translate(double dx, double dy);
    void rotate(double a, double dx, double dy);
    void addViewBox(QDomElement* svg_node, double relUnitsToPix, bool isMarker);
    void setSvgImageSize(SvgImage* svgImage, double relUnitsToPix);
};

#endif // IMGBORDER_H
