#ifndef __PPMMETHODS_H__
#define __PPMMETHODS_H__

#include "ppmIO.h"

Pixel* makeMask(Pixel* image,int imageSize);
Pixel* setBackground(Pixel* background, Pixel* image, Pixel* imageMask, int imageSize);
Pixel* translate(Pixel* background, Pixel* image, Pixel* imageMask, int imageSize, int cols,int rows, int dx, int dy);
Pixel* toGreyscale(Pixel* image, int imageSize);
Pixel* toNegative(Pixel* image, int imageSize);
Pixel* horizontalBlur(Pixel* image, int imageSize);


#endif