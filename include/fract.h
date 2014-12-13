/*
Erfan Azad
Date: Wed 17 Sep 2014
File: fract.h
*/
#ifndef __FRACT_H__
#define __FRACT_H__
#include "Image.h"

void mandelbrot( Image *dst, float x0, float y0, float dx);
void julia(Image *dst, float x0, float y0, float dx);
Image* makeNoise(Image* img);

#endif