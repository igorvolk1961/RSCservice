#if !defined(__linux__)
#include "stdafx.h"
#endif

#include "palette.h"

struct Rgb{
    Rgb(int r, int g, int b){
        this->r = r;
        this->g = g;
        this->b = b;
    }
    int r;
    int g;
    int b;
};

Rgb palette[] = {
    Rgb(0, 0, 0),
    Rgb(0, 0, 255),
    Rgb(0, 170, 0),
    Rgb(0, 85, 85),
    Rgb(255, 0, 0),
    Rgb(255, 170, 85),
    Rgb(170, 85, 0),
    Rgb(170, 170, 170),

    Rgb(85, 85, 85),
    Rgb(0, 170, 255),
    Rgb(85, 255, 85),
    Rgb(85, 85, 255),
    Rgb(185, 0, 85),
    Rgb(255, 85, 255),
    Rgb(255, 255, 85),
    Rgb(255, 255, 255),

    Rgb(255, 0, 0),
    Rgb(255, 0, 0),
    Rgb(255, 0, 0),
    Rgb(255, 0, 0),
    Rgb(255, 0, 0),
    Rgb(255, 0, 0),
    Rgb(255, 0, 0),
    Rgb(255, 0, 0),

    Rgb(255, 0, 0),
    Rgb(255, 0, 0),
    Rgb(255, 0, 0),
    Rgb(255, 0, 0),
    Rgb(255, 0, 0),
    Rgb(255, 0, 0),
    Rgb(255, 0, 0),
    Rgb(0, 0, 0),

    Rgb(0, 0, 255),
    Rgb(0, 170, 0),
    Rgb(0, 85, 85),
    Rgb(255, 0, 0),
    Rgb(255, 0, 0),
    Rgb(255, 0, 0),
    Rgb(255, 0, 0),
    Rgb(255, 0, 0),

    Rgb(255, 0, 0),
    Rgb(255, 0, 0),
    Rgb(255, 0, 0),
    Rgb(255, 0, 0),
    Rgb(255, 0, 0),
    Rgb(255, 0, 0),
    Rgb(255, 0, 0),
    Rgb(255, 0, 0),

    Rgb(255, 0, 0),
    Rgb(255, 0, 0),
    Rgb(255, 0, 0),
    Rgb(255, 0, 0),
    Rgb(255, 0, 0),
    Rgb(255, 0, 0),
    Rgb(255, 0, 0),
    Rgb(255, 0, 0),

    Rgb(255, 0, 0),
    Rgb(255, 0, 0),
    Rgb(255, 0, 0),
    Rgb(255, 0, 0),
    Rgb(255, 0, 0),
    Rgb(255, 0, 0),
    Rgb(255, 0, 0),
    Rgb(255, 0, 0)
};

int getPaletteColor(int index){
    Rgb rgb = palette[index];
    int color = rgb.r + (rgb.g << 8) + (rgb.b << 16);
    return color;
}
