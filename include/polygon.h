/*
Erfan Azad
Date: 29 Sep 2014
file: polygon.h
*/
#ifndef __POLYGON_H__
#define __POLYGON_H__

#include "point.h"
#include "Image.h"
#include "color.h"
#include "drawState.h"
#include "light.h"

typedef struct{
	int numVertex;
	int zBuffer;
	int oneSided;
	float alpha; // for transparency 
	Point *vertex;
	Point *vertexWorld;
	Vector *normalWorld;
	int phong;
	Color *color;
	Vector *normal;
}Polygon;

typedef struct{
	Color color;
	float depth;
	float alpha;
}Alphainfo;


Polygon *polygon_create(void);
Polygon *polygon_createp(int numV, Point *vlist);
void polygon_free(Polygon *p);
void polygon_init(Polygon *p);
void polygon_set(Polygon *p, int numV, Point *vlist);
void polygon_clear(Polygon *p);
void polygon_zBuffer(Polygon *p, int flag);
void polygon_copy(Polygon *to, Polygon *from);
void polygon_print(Polygon *p, FILE *fp);
void polygon_draw(Polygon *p, DrawState* ds, Image *src);
void polygon_drawFill(Polygon *p, DrawState* ds, Image *src,Lighting *Lights);
void polygon_drawFillBitmap(Polygon *p, DrawState* ds, Image *src, Image *bitmap );
void polygon_drawFillB(Polygon *p, DrawState* ds, Image *src);
void polygon_drawFillB_Gradient(Polygon *p, Image *src, Color c1, Color c2,Color c3 );
void polygon_normalize(Polygon *p);
void polygon_setAlpha(Polygon *p, float a);
void polygon_setSided(Polygon *p, int oneSided);
void polygon_setColors(Polygon *p, int numV, Color *clist);
void polygon_setNormals(Polygon *p, int numV, Vector *nlist);
void polygon_setAll(Polygon *p, int numV, Point *vlist, Color *clist, Vector *nlist, int zBuffer, int oneSided);
void polygon_drawShade(Polygon *p, Image *src, DrawState *ds, Lighting *light);
void polygon_shade(Polygon *p, Lighting *lighting, DrawState *ds);
float Aux_implicit_line(Point p1, Point p2, float x,float y);
int compare (const void * a, const void * b);


#endif