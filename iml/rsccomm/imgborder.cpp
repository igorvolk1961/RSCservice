#if !defined(__linux__)
#include "stdafx.h"
#endif

#include "imgborder.h"
#include "imgtype.h"
#include "svgimage.h"
#include <climits>
#include <cmath>

ImgBorder::ImgBorder(){
    minX = INT_MAX;
    minY = INT_MAX;
    maxX = INT_MIN;
    maxY = INT_MIN;
    maxThick = 0;
    withTexts = true;
}

void ImgBorder::init(){
    minX = INT_MAX;
    minY = INT_MAX;
    maxX = INT_MIN;
    maxY = INT_MIN;
    maxThick = 0;
}

void ImgBorder::update(double x, double y){
    maxX = fmax(maxX, x);
    maxY = fmax(maxY, y);
    minX = fmin(minX, x);
    minY = fmin(minY, y);
}

void ImgBorder::extend(double value){
    maxX += value;
    maxY += value;
    minX -= value;
    minY -= value;
}

void ImgBorder::expand(ImgBorder* imgBorder){
    maxX = fmax(maxX, imgBorder->maxX);
    maxY = fmax(maxY, imgBorder->maxY);
    minX = fmin(minX, imgBorder->minX);
    minY = fmin(minY, imgBorder->minY);
    maxThick = fmax(maxThick, imgBorder->maxThick);
}

void ImgBorder::translate(double dx, double dy){
    maxX += dx;
    maxY += dy;
    minX += dx;
    minY += dy;
}

void ImgBorder::rotate(double a, double dx, double dy){
    double cosA = cos(a * M_PI / 180.);
    double sinA = sin(a * M_PI / 180.);
    double maxX1 = maxX - dx;
    double maxY1 = maxY - dy;
    double minX1 = minX - dx;
    double minY1 = minY - dy;
    double x0 = maxX1 * cosA - maxY1 * sinA;
    double y0 = maxX1 * sinA + maxY1 * cosA;
    update(x0, y0);
    double x1 = maxX1 * cosA - minY1 * sinA;
    double y1 = maxX1 * sinA + minY1 * cosA;
    update(x1, y1);
    double x2 = minX1 * cosA - minY1 * sinA;
    double y2 = minX1 * sinA + minY1 * cosA;
    update(x2, y2);
    double x3 = minX1 * cosA - maxY1 * sinA;
    double y3 = minX1 * sinA + maxY1 * cosA;
    update(x3, y3);
}

void ImgBorder::setSvgImageSize(SvgImage* svgImage, double relUnitsToPix){
    double vb_factor = 1.;

    double width = (maxX - minX) * vb_factor;
    double height = (maxY - minY) * vb_factor;
    svgImage->width_px = width * relUnitsToPix;
    svgImage->height_px = height * relUnitsToPix;
    svgImage->minX_px = minX * vb_factor * relUnitsToPix;
    svgImage->minY_px = minY * vb_factor * relUnitsToPix;
}


void ImgBorder::addViewBox(QDomElement* svg_node, double relUnitsToPix, bool isMarker){
//    printf ("ImgBorder::addViewBox baseLineLength_px=%0.2f maxThick=%0.2f\n", baseLineLength_px, maxThick);
//    printf ("ImgBorder::addViewBox maxX=%0.2fx minX=%0.2f maxY=%0.2fx minY=%0.2f\n", maxX, minX, maxY, minY);

    extend(maxThick);
    double vb_factor = 1.;

    double width = (maxX - minX) * vb_factor;
    double height = (maxY - minY) * vb_factor;
    double widthPx = round(width * relUnitsToPix);
    double heightPx = round(height * relUnitsToPix);

    char viewBox_s[255];
    sprintf(viewBox_s, "%0.1f %0.1f %0.1f %0.1f", minX * vb_factor, minY * vb_factor, width, height);

    char widthPx_s[32];
    sprintf(widthPx_s, "%0.0f", widthPx);

    char heightPx_s[32];
    sprintf(heightPx_s, "%0.0f", heightPx);

    svg_node->setAttribute("preserveAspectRatio", "xMinYMin meet");
    svg_node->setAttribute("viewBox", viewBox_s);
    if (!isMarker){
      svg_node->setAttribute("width", widthPx_s);
      svg_node->setAttribute("height", heightPx_s);
    } else {
        svg_node->setAttribute("markerWidth", widthPx_s);
        svg_node->setAttribute("markerHeight", heightPx_s);
    }
}
