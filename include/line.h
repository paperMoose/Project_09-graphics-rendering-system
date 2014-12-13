//
//  line.h
//  
//
//  Created by Ryan Brandt on 9/22/14.
//
//

#ifndef __LINE_H__
#define __LINE_H__

#include "Image.h"
#include "point.h"
#include "color.h"

typedef  struct{
    int zBuffer;
    Point a;
    Point b;
}Line;


void line_set2D(Line *l, double x0, double y0, double x1, double y1);
void line_set(Line *l, Point ta, Point tb);
void line_zBuffer(Line *l, int flag);	
void line_copy(Line *to, Line *from);
void line_draw(Line *l, Image *src, Color c);
void line_drawDash(Line *l, Image *src, Color c);
void line_normalize(Line* l);
void line_drawDash(Line *l, Image *src, Color c);
Line Liang_Barsky(Line *l, Image *src);
float min(float a, float b);
float max(float a, float b);
void line_print(Line* l, FILE *fp);

#endif

