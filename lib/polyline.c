//
//  point.c
//
//
//  Created by Ryan Brandt on 9/22/14.
//
//

#include <stdlib.h>
#include <stdio.h>
#include "polyline.h"
#include "point.h"
#include "color.h"
#include "Image.h"
#include "line.h"


/*
 returns an allocated Polyline pointer initialized so that numVertex is 0 and vertex is NULL
*/
Polyline* polyline_create(){
    Polyline *p=malloc(sizeof(Polyline));
    if(NULL == p){
        printf("ERROR: polyline_create >> malloc failed!\n");
        exit(-1);
    }
    p->vertex=NULL;
    p->numVertex=0;
    p->zBuffer = 1;
    return p;
}

/*
returns an allocated Polyline pointer with the vertex list initialized to the points in vlist
*/
Polyline *polyline_createp(int numV, Point *vlist){
    Polyline *p=malloc(sizeof(Polyline));
    if(NULL == p){
        printf("ERROR: polyline_createp >> malloc failed!\n");
        exit(-1);
    }
    p->vertex=malloc(sizeof(Point)*numV);
    for (int i = 0; i < numV; i++)
    {
        point_copy(&(p->vertex[i]),&vlist[i]);
    }
    // p=vlist;
    p->numVertex=numV;
    p->zBuffer = 1;
    return p;
}

/*
 frees the internal data and the Polyline pointer
*/
void polyline_free(Polyline *p){
    if (p!=NULL) {
        if (p->vertex!=NULL) {
            free(p->vertex);
        }
        free(p);
    }
}


/*
initializes the pre-existing Polyline to an empty Polyline.
*/
void polyline_init(Polyline *p){
    p->vertex=NULL;
    p->numVertex=0;
    p->zBuffer = 1;
}

/*
initializes the vertex list to the points in vlist. De-allocates/allocates the vertex list for p, as necessary
*/
void polyline_set(Polyline *p, int numV, Point *vlist){
    
    if (p->vertex!=NULL) {
        free(p->vertex);
    }
        p->vertex=malloc(sizeof(Point)*numV);
    for (int i = 0; i < numV; i++)
    {
        point_copy(&(p->vertex[i]),&vlist[i]);
    }
    p->numVertex = numV;
    p->zBuffer = 1;
}

/*
frees the internal data for a Polyline, if necessary, and sets numVertex to 0 and vertex to NULL
*/
void polyline_clear(Polyline *p){
    if(p->vertex != NULL){
        free(p->vertex);
    }
    p->vertex = NULL;
    p->numVertex = 0;
    p->zBuffer = 1;
}


/*
sets the z-buffer flag to the given value.
*/
void polyline_zBuffer(Polyline *p, int flag){
    if(NULL != p){
        p->zBuffer = flag;
    }
}

/*
De-allocates/allocates space as necessary in the destination Polyline data structure and copies the
vertex data from the source polyline (from) to the destination (to).
*/
void polyline_copy(Polyline *to, Polyline *from){
    if(NULL != to && NULL != from){
        polyline_clear(to);
        to->vertex = malloc(sizeof(Point)*from->numVertex);
        int i;
        for (i=0; i < from->numVertex; i++)
        {
          to->vertex[i] = from->vertex[i];
        }
        to->numVertex = from->numVertex;
        to->zBuffer = from->zBuffer;
    }
}

/*
prints Polyline data to the stream designated by the FILE pointer
*/
void polyline_print(Polyline *p, FILE *fp){
    if(NULL != p && NULL != fp){
        int i;
        fprintf(fp, "%d\n",p->numVertex);//writes number of vertecies
        fprintf(fp, "%d\n",p->zBuffer);//writes the zBuffer value
        for(i=0; i<p->numVertex; i++){
            fprintf(fp, "%f %f %f %f\n",p->vertex[i].val[0], p->vertex[i].val[1], p->vertex[i].val[2], p->vertex[i].val[3]);
        }
    }
}


/*
draw the polyline using color c
*/
void polyline_draw(Polyline *p, Image *src, Color colr){
    Line l;
    int i;
    for (i=0; i< p->numVertex-1; i++){
        line_set2D(&l,p->vertex[i].val[0], p->vertex[i].val[1], p->vertex[i+1].val[0], p->vertex[i+1].val[1]);
        line_draw(&l, src, colr);
    }
}

void polyline_normalize(Polyline *p){
    int i;
    for(i=0;i<p->numVertex;i++){
        p->vertex[i].val[0]/=p->vertex[i].val[3];
        p->vertex[i].val[1]/=p->vertex[i].val[3];
    }
}










