/*
Erfan Azad
Date: 9 Oct 2014
File: matrix.h
*/
#include "point.h"
#include "line.h"
#include "polygon.h"
#include "polyline.h"


#ifndef __MATRIX_H__
#define __MATRIX_H__

typedef struct {
	double m[4][4];
} Matrix;


void matrix_print(Matrix *m, FILE *fp);
void matrix_clear(Matrix *m);
void matrix_identity(Matrix *m);
double matrix_get(Matrix *m, int r, int c);
void matrix_set(Matrix *m, int r, int c, double v);
void matrix_copy(Matrix *dest, Matrix *src);
void matrix_transpose(Matrix *m);
void matrix_multiply(Matrix *left, Matrix *right, Matrix *m);
void matrix_xformPoint(Matrix *m, Point *p, Point *q);
void matrix_xformVector(Matrix *m, Vector *p, Vector *q);
void matrix_xformPolygon(Matrix *m, Polygon *p);
void matrix_xformPolyline(Matrix *m, Polyline *p);
void matrix_xformLine(Matrix *m, Line *line);
void matrix_scale2D(Matrix *m, double sx, double sy);
void matrix_rotateZ(Matrix *m, double cth, double sth);
void matrix_translate2D(Matrix *m, double tx, double ty);
void matrix_shear2D(Matrix *m, double shx, double shy);
void matrix_translate(Matrix *m, double tx, double ty, double tz);
void matrix_scale(Matrix *m, double sx, double sy, double sz);
void matrix_rotateX(Matrix *m, double cth, double sth);
void matrix_rotateY(Matrix *m, double cth, double sth);
void matrix_rotateXYZ(Matrix *m, Vector *u, Vector *v, Vector *w);
void matrix_shearZ(Matrix *m, double shx, double shy);
void matrix_perspective(Matrix *m, double d);

//polygon surface normal
void polygon_normal(Polygon* p, Vector* c);





#endif