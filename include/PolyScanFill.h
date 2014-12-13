/*
Erfan Azad
Date: 1 Oct 2014
*/
#ifndef __POLYSCANFILL_H__
#define __POLYSCANFILL_H__

#include "polygon.h"
#include "list.h"
#include "Image.h"
#include "point.h"
#include "color.h"


typedef struct tEdge {
	float x0, y0;                   /* start point for the edge */
	float x1, y1;                   /* end point for the edge */
	int yStart, yEnd;               /* start row and end row */
  	float xIntersect, dxPerScan;    /* where the edge intersects the current scanline and how it changes */
	/* we'll add more here later */
  	struct tEdge *next;
} Edge;

static int compYStart( const void *a, const void *b );
static int compXIntersect( const void *a, const void *b );
static Edge *makeEdgeRec( Point start, Point end, Image *src);
static LinkedList *setupEdgeList( Polygon *p, Image *src);
static void fillScan( int scan, LinkedList *active, Image *src, Color c );
static int processEdgeList( LinkedList *edges, Image *src, Color c );
void polygon_drawFill_(Polygon *p, Image *src, Color c ) ;

#endif