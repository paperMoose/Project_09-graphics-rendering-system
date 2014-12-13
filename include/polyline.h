/*
Ryan Brandt
 Date: Mon 15 Sep 2014
 */

#ifndef __POLYLINE_H__
#define __POLYLINE_H__

#include "color.h"
#include "point.h"
#include "Image.h"


typedef struct{
    int zBuffer;
    int numVertex;
    Point *vertex;
}Polyline;


Polyline* polyline_create(void);
Polyline* polyline_createp(int numV, Point *vlist);
void polyline_free(Polyline *p);
void polyline_init(Polyline *p);
void polyline_set(Polyline *p, int numV, Point *vlist);
void polyline_clear(Polyline *p);
void polyline_zBuffer(Polyline *p, int flag);
void polyline_copy(Polyline *to, Polyline *from);
void polyline_print(Polyline *p, FILE *fp);
void polyline_draw(Polyline *p, Image *src, Color colr);
void polyline_normalize(Polyline *p);
#endif