/*
Ryan Brandt & Erfan Azad
Date: 24 Oct 2014
File: module.h
*/

#ifndef __MODULE_H__
#define __MODULE_H__

#include "point.h"
#include "line.h"
#include "polyline.h"
#include "polygon.h"
#include "color.h"
#include "matrix.h"
#include "drawState.h"
#include "bezier.h"

typedef enum { // example of an enumerated type
	ObjNone,
	ObjLine,
	ObjPoint,
	ObjPolyline,
	ObjPolygon,
	ObjIdentity,
	ObjMatrix,
	ObjColor,
	ObjBodyColor,
	ObjSurfaceColor,
	ObjSurfaceCoeff,
	ObjLight,
	ObjModule
} ObjectType;

typedef union {
	Point point;
	Line line;
	Polyline polyline;
	Polygon polygon;
	Matrix matrix;
	Color color;
	float coeff;
	void *module;	
} Object;

typedef struct {
	ObjectType type;
	Object obj;
	void *next;
} Element;

typedef struct {
	Element *head; // pointer to the head of the linked list
	Element *tail; // keep around a pointer to the last object
} Module;


//functions
Element* element_create(void);
Element* element_init(ObjectType type, void *obj);
void element_delete(Element *e);
Module* module_create(void);
void module_clear(Module *md);
void module_delete(Module *md);
void module_insert(Module *md, Element *e);
void module_module(Module *md, Module *sub);
void module_point(Module *md, Point *p);
void module_line(Module *md, Line *p);
void module_polyline(Module *md, Polyline *p);
void module_polygon(Module *md, Polygon *p);
void module_bezierCurve(Module *m, BezierCurve *b, int divisions);
void module_bezierSurface(Module *m, BezierSurface *b, int divisions, int solid);
void module_identity(Module *md);
void module_translate2D(Module *md, double tx, double ty);
void module_scale2D(Module *md, double sx, double sy);
void module_rotateZ(Module *md, double cth, double sth);
void module_shear2D(Module *md, double shx, double shy);
void module_draw(Module *md, Matrix *VTM, Matrix *GTM, DrawState *ds, /*Vector* vpn,*/ Lighting *lighting, Image *src);

//3D functions
void module_translate(Module *md, double tx, double ty, double tz);
void module_scale(Module *md, double sx, double sy, double sz);
void module_rotateX(Module *md, double cth, double sth);
void module_rotateY(Module *md, double cth, double sth);
void module_rotateXYZ(Module *md, Vector *u, Vector *v, Vector *w);

//shading/color module functions
void module_color(Module *md, Color *c);
void module_surfaceColor(Module *md, Color *c);
void module_bodyColor(Module *md, Color *c);
void module_surfaceCoeff(Module *md, float* f);

//shapes
void module_cube(Module *md/*, int solid*/);
void module_circleFrame( Module *mod, int sides );//using lines -> cannot be filled
void module_circle( Module *mod, int sides ); // using polygons -> can be filled
void module_ellipseFrame(Module* mod, double a, double b, int sides);
void module_ellipse(Module* mod, double a, double b, int sides);
void module_cylinder(Module* mod, int sides);
void module_pyramid(Module* mod, int sides);
void module_tetrahedron(Module *md);

//auxilary functions
void de_Casteljau_point(Point *p, Point* row);










#endif