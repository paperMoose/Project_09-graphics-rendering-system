//
//  point.c
//  
//
//  Created by Ryan Brandt on 9/22/14.
//
//

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "point.h"
#include "color.h"
#include "Image.h"


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
        c->val[3]= 0;
    }
}

//returns an average vector of all the vectors in the "vector" array
Vector vector_average(Vector* vector, int size){
    if(NULL != vector){
        int i;
        Vector vect;
        float a=0;
        float b=0;
        float c=0;
        for(i=0; i<size; i++){
            a += vector[i].val[0];
            b += vector[i].val[1];
            c += vector[i].val[2];
        }
        a /= size;
        b /= size;
        c /= size;
        vector_set(&vect, a, b, c);
        return vect;
    }
    printf("vector_average >> vector pointer in NULL\n");
    exit(-1);
}


void point_set2D(Point *p, double x, double y){
/*set the first two values of the vector to x and y. Set the third value to 0 and the fourth value to 1 .*/
    p->val[0]=x;
    p->val[1]=y;
    p->val[2]=0.0;
    p->val[3]=1.0;
}

void point_set3D(Point *p, double x, double y, double z){
    /*set the point’s values to x and y and z. Set the homogeneous coordinate to 1 .*/
    p->val[0]=x;
    p->val[1]=y;
    p->val[2]=z;
    p->val[3]=1;
}

void point_set(Point *p, double x, double y, double z, double h){
/*set the four values of the vector to x, y, z, and h, respectively.*/
    p->val[0]=x;
    p->val[1]=y;
    p->val[2]=z;
    p->val[3]=h;
}


void point_copy(Point *to, Point *from){
/*copy the point data structure.*/
    if (to==NULL||from ==NULL) {
        printf("ERROR: point_copy >> pointer parameter is NULL\n");
        exit(-1);
    }
    *to = *from; // you can copy structuresd like this as long as there is no dynamic allocation
}


void point_draw(Point *p, Image *src, Color c){
/*draw the point into src using color c.*/
    if (p==NULL||src ==NULL) {
        printf("ERROR: point_draw >> pointer parameter is NULL\n");
        exit(-1);
    }
    int x=(int)p->val[1];
    int y=(int)p->val[0];
    image_setc(src, x, y, 0, c.rgb[0]);
    image_setc(src, x, y, 1, c.rgb[1]);
    image_setc(src, x, y, 2, c.rgb[2]);
    image_setz(src, x, y, p->val[2]);
}


void point_drawf(Point *p, Image *src, FPixel c){
/*draw the p into src using FPixel c.*/
    if (p==NULL||src ==NULL) {
        printf("ERROR: point_drawf >> pointer parameter is NULL\n");
        exit(-1);
    }
    double x=p->val[1];
    double y=p->val[0];
    image_setf(src, x, y, c);
    image_setz(src, x, y, p->val[2]);
}

void point_print(Point *p, FILE *fp){
    if(NULL!=p && NULL!=fp){
        fprintf(fp, "( %.3f %.3f %.3f %.3f )\n",p->val[0], p->val[1], p->val[2], p->val[3]);
        fprintf(fp, "\n");
    }
}

//normalizes the point p by the h coords.
void point_normalize(Point *p){
    p->val[0] = p->val[0]/ p->val[3];
    p->val[1] = p->val[1]/ p->val[3];
}

//returns an average point of the "point" array
Point point_average(Point* point, int size){
    if(NULL != point){
        int i;
        Point p;
        float a=0;
        float b=0;
        float c=0;
        float d=0;
        for(i=0; i<size; i++){
            a += point[i].val[0];
            b += point[i].val[1];
            c += point[i].val[2];
            d += point[i].val[3];
        }
        a /= size;
        b /= size;
        c /= size;
        d /= size;
        point_set(&p, a, b, c, d);
        return p;
    }    
    printf("point_average >> point pointer is NULL\n");
    exit(-1);
}
