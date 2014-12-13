/*
Erfan & Ryan 
Date: 4 Dec 2014
File: vector.c
*/


#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "vector.h"
#include "matrix.h"
#include "point.h"
#include "polyline.h"
#include "polygon.h"
#include "line.h"


// Set the Vector to (x, y, z, 0.0).
void vector_set(Vector *v, double x, double y, double z){
	v->val[0]=x;
    v->val[1]=y;
    v->val[2]=z;
    v->val[3]=0.0;
}

//Print out the Vector to stream fp in a pretty form
void vector_print(Vector *v, FILE *fp){
	if(NULL != v && NULL != fp){
		fprintf(fp, " ( %.3f %.3f %.3f %.3f )\n",v->val[0], v->val[1], v->val[2], v->val[3]);
	}
}

//Copy the src Vector into the dest Vector.
void vector_copy(Vector *dest, Vector *src){
	if(NULL != dest && NULL != src){
		*dest = *src;
	}
}

/*
Returns the Euclidean length of the vector,
assuming the homogeneous coordinate is 1.0.
*/
double vector_length(Vector *v){
	if(NULL != v){
		return (sqrt((v->val[0]*v->val[0])+(v->val[1]*v->val[1])+(v->val[2]*v->val[2])));
	}
	printf("ERROR: vector_length >> v is NULL");
	exit(-1);
}

//Normalize the Vector to unit length. Do not modify the homogeneous coordinate.
void vector_normalize(Vector *v){
	double len = vector_length(v);
	v->val[0] = v->val[0]/len;
	v->val[1] = v->val[1]/len;
	v->val[2] = v->val[2]/len;
}

// Returns the scalar product of ~a and ~b.
double vector_dot(Vector *a, Vector *b){
	if(NULL != a && NULL != b){
		return (a->val[0]*b->val[0])+(a->val[1]*b->val[1])+(a->val[2]*b->val[2]);
	}
	printf("ERROR: vector_dot >> v is NULL");
	exit(-1);
}

Vector vector_add(Vector *a, Vector *b){
	if(NULL != a && NULL != b){
		Vector sum;
		vector_set(&sum,a->val[0]+b->val[0],a->val[1] + b->val[1], a->val[2] + a->val[2]);
		return sum;
	}
	printf("ERROR: vector_add >> v is NULL");
	exit(-1);
}

//Calculates the the cross product (vector product) of ~a and ~b and puts the result in ~c.
void vector_cross(Vector *a, Vector *b, Vector *c){
	if(NULL != a && NULL != b && NULL != c){
		c->val[0]= (a->val[1]*b->val[2])-(a->val[2]*b->val[1]);
		c->val[1]= (a->val[2]*b->val[0])-(a->val[0]*b->val[2]);
		c->val[2]= (a->val[0]*b->val[1])-(a->val[1]*b->val[0]);
	}
}


Vector vector_add(Vector *a, Vector *b){
	
}