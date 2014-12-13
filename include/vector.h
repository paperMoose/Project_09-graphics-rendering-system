/*
Erfan & Ryan
Date: 4 Dec 2014
File: vector.h
*/

#ifndef __VECTOR_H__
#define __VECTOR_H__

#include "point.h"
#include "line.h"
#include "polygon.h"
#include "polyline.h"


typedef Point Vector;

void vector_set(Vector *v, double x, double y, double z);
void vector_print(Vector *v, FILE *fp);
void vector_copy(Vector *dest, Vector *src);
double vector_length(Vector *v);
void vector_normalize(Vector *v);
double vector_dot(Vector *a, Vector *b);
void vector_cross(Vector *a, Vector *b, Vector *c);
Vector vector_add(Vector *a, Vector *b);

#endif