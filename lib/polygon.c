/*
Erfan Azad
Date: 29 Sep 2014
file: polygon.c
*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "polygon.h"
#include "matrix.h"
#include "list.h"
#include "line.h"
#include "point.h"
#include "Image.h"
#include "color.h"
#include "drawState.h"
#include "light.h"


/****************************************
Start Scanline Fill
*****************************************/
typedef struct tEdge {
    float x0, y0, z0;                   /* start point for the edge */
    float x1, y1, z1;                   /* end point for the edge */
    int yStart, yEnd;               /* start row and end row */
    float xIntersect, dxPerScan;
    float zIntersect, dzPerScan;    /* where the edge intersects the current scanline and how it changes */
    Point pIntersect;
    Vector nIntersect;
    float dpPerScan[3];
    float dnPerScan[3];
    float dcPerScan[3];
    Color cIntersect;
    Polygon * p;
    /* we'll add more here later */
    struct tEdge *next;
} Edge;

typedef struct{
    Polygon* poly;
    float zVal;
}A_bufferInfo;



/*
    This is a comparison function that returns a value < 0 if a < b, a
    value > 0 if a > b, and 0 if a = b.  It uses the yStart field of the
    Edge structure.  It is used to sort the overall edge list.
 */
static int compYStart( const void *a, const void *b ) { 
    Edge *ea = (Edge *)a;
    Edge *eb = (Edge *)b;

    return(ea->yStart - eb->yStart);
}


/*
This is a comparison function that returns a value < 0 if a < b, a value 
 > 0 if a>b, and -1 if a = b. It uses the curZ value in the fillscan for each Fpixel.
 It is used to sort the linkedlist of fpixel by 1/z values.
*/
static int compZIntersect(const void *a, const void *b){
    float fa = *(float*)a;
    float fb = *(float*)b;

    if(fa < fb) //NOTE that the equal state will result in -1 in order not to replace the background image
        return (-1);
    else if(fa > fb)
        return (1);
    return (0);
}


/*
    This is a comparison function that returns a value < 0 if a < b, a
    value > 0 if a > b, and 0 if a = b.  It uses the xIntersect field of the
    Edge structure.  It is used to sort the active edge list.
 */
static int compXIntersect( const void *a, const void *b ) {
    Edge *ea = (Edge *)a;
    Edge *eb = (Edge *)b;

    if( ea->xIntersect < eb->xIntersect )
        return(-1);
    else if(ea->xIntersect > eb->xIntersect )
        return(1);

    return(0);
}

/*
    Allocates, creates, fills out, and returns an Edge structure given
    the inputs.

    Current inputs are just the start and end location in image space.
    Eventually, the points will be 3D and we'll add color and texture
    coordinates.
 */
static Edge *makeEdgeRec( Point start, Point end, Color c1, Color c2, DrawState* ds, Image *src,Vector normal1,Vector normal2,Point WorldPoint1, Point WorldPoint2)
{
    Edge *edge;
    float dscan = end.val[1] - start.val[1]; //y1 - y0
    
    edge = (Edge*)malloc(sizeof(Edge));
    edge->x0 = start.val[0];
    edge->y0 = start.val[1];
    edge->z0 = start.val[2];
    edge->x1 = end.val[0];
    edge->y1 = end.val[1];
    edge->z1 = end.val[2];
    edge->yStart = (int)(edge->y0 + 0.5);
    edge->yEnd = (int)(edge->y1 + 0.5)-1; 
    edge->dxPerScan = (edge->x1 - edge->x0)/(edge->y1 - edge->y0);
    if(ds->shade == ShadeDepth || ds->shade == ShadeGouraud || ds->shade == ShadePhong){
        edge->dzPerScan = (1/edge->z1 - 1/edge->z0)/(edge->y1 - edge->y0);
        if(ds->shade == ShadeGouraud){
            edge->dcPerScan[0] = (c2.rgb[0]/end.val[2] - c1.rgb[0]/start.val[2])/ dscan;
            edge->dcPerScan[1] = (c2.rgb[1]/end.val[2] - c1.rgb[1]/start.val[2])/ dscan;
            edge->dcPerScan[2] = (c2.rgb[2]/end.val[2] - c1.rgb[2]/start.val[2])/ dscan;
        }
        if(ds->shade == ShadePhong){
            edge->dpPerScan[0] = (WorldPoint2.val[0]/end.val[2] - WorldPoint1.val[0]/start.val[2])/dscan;
            edge->dpPerScan[1] = (WorldPoint2.val[1]/end.val[2] - WorldPoint1.val[1]/start.val[2])/dscan;
            edge->dpPerScan[2] = (WorldPoint2.val[2]/end.val[2] - WorldPoint1.val[2]/start.val[2])/dscan;

            edge->dnPerScan[0] = (normal2.val[0]/end.val[2] - normal1.val[0]/start.val[2])/dscan;
            edge->dnPerScan[1] = (normal2.val[1]/end.val[2] - normal1.val[1]/start.val[2])/dscan;
            edge->dnPerScan[2] = (normal2.val[2]/end.val[2] - normal1.val[2]/start.val[2])/dscan;
        }

    }else{
        edge->dzPerScan = 0;
        edge->dcPerScan[0] = 0;
        edge->dcPerScan[1] = 0;
        edge->dcPerScan[2] = 0;

        edge->dpPerScan[0] = 0;
        edge->dpPerScan[1] = 0;
        edge->dpPerScan[2] = 0;

        edge->dnPerScan[0] = 0;
        edge->dnPerScan[1] = 0;
        edge->dnPerScan[2] = 0;
    }

    // printf("z0 %f\n", edge->z0);





    //Correctly initializing xIntersect
    edge->xIntersect = edge->x0 + abs((edge->y0 - edge->yStart )) * edge->dxPerScan;
    edge->zIntersect = 1/edge->z0 + abs((edge->y0 - edge->yStart )) * edge->dzPerScan;
// c intersect
    edge->cIntersect.rgb[0] = c1.rgb[0]/start.val[2];
    edge->cIntersect.rgb[1] = c1.rgb[1]/start.val[2];
    edge->cIntersect.rgb[2] = c1.rgb[2]/start.val[2];
// worldpoint intersect
    edge->pIntersect.val[0] = WorldPoint1.val[0]/start.val[2];
    edge->pIntersect.val[1] = WorldPoint1.val[1]/start.val[2];
    edge->pIntersect.val[2] = WorldPoint1.val[2]/start.val[2];
// normals intersect
    edge->nIntersect.val[0] = normal1.val[0]/start.val[2];
    edge->nIntersect.val[1] = normal1.val[1]/start.val[2];
    edge->nIntersect.val[2] = normal1.val[2]/start.val[2];


    // printf("intersects r g b  %f  %f   %f \n", edge->cIntersect.rgb[0],edge->cIntersect.rgb[1],edge->cIntersect.rgb[2]);
    //  printf("color  %f \n", c1.rgb[0]);

    // if(edge->zIntersect >1){
    //     edge->zIntersect = 1/edge->zIntersect;
    // }
    // printf("zIntersect %f", edge->zIntersect);

    //Clipping if the edge starts off the image or goes off image
    if(edge->y0 < 0){ //if edge starts below row 0
        printf("edge starts above the image \n");
        edge->xIntersect += -edge->y0 * edge->dxPerScan;
        edge->zIntersect += -edge->y0 * edge->dzPerScan;
        edge->y0 = 0;
        edge->yStart = 0;
    }
    if(edge->yEnd > src->rows-1){ //if the edge starts inside the image but continues outside
        edge->yEnd = src->rows-1;
    }


    if(edge->xIntersect<edge->x1 && edge->dxPerScan<0){
        edge->xIntersect = edge->x1; 
    }
    if(edge->xIntersect>edge->x1 && edge->dxPerScan>0){
        edge->xIntersect = edge->x1; 
    }

    edge->next = NULL;

    return( edge );
}


/*
    Returns a list of all the edges in the polygon in sorted order by
    smallest row.
*/
static LinkedList *setupEdgeList( Polygon *p, DrawState* ds, Image *src) {
    LinkedList *edges = NULL;
    Point v1, v2;
    Color c1, c2;
    Point WorldPoint1,WorldPoint2;
    Vector normal1, normal2;

    int i;

    // create a linked list
    edges = ll_new();

    // walk around the polygon, starting with the last point
    v1 = p->vertex[p->numVertex-1];
    if(ds->shade == ShadeGouraud){
        Color_copy(&c1, &p->color[p->numVertex-1]);
    }

// setting normals and points for phong edge rec
    if(ds->shade == ShadePhong){
        point_copy(&WorldPoint1, &p->vertexWorld[p->numVertex-1]);
        vector_copy(&normal1, &p->normalWorld[p->numVertex-1]);
    }

    for(i=0;i<p->numVertex;i++) {
        
        // the current point (i) is the end of the segment
        v2 = p->vertex[i];
        if(ds->shade == ShadeGouraud){
            Color_copy(&c2, &p->color[i]);
        }
        if(ds->shade == ShadePhong){
            point_copy(&WorldPoint2, &p->vertexWorld[i]);
            vector_copy(&normal2, &p->normalWorld[i]);
        }



        // if it is not a horizontal line
        if( (int)(v1.val[1]+0.5) != (int)(v2.val[1]+0.5) ) {
            Edge *edge;

            // if the first coordinate is smaller (top edge)
            if( v1.val[1] < v2.val[1] )
                edge = makeEdgeRec( v1, v2, c1, c2 ,ds ,src,normal1,normal2,WorldPoint1,WorldPoint2 );
            else
                edge = makeEdgeRec( v2, v1, c2, c1, ds, src,normal2,normal1,WorldPoint2,WorldPoint1 );

            // insert the edge into the list of edges if it's not null
            if( edge )

                ll_insert( edges, edge, compYStart );
        }
        v1 = v2;
        c1 = c2;
        WorldPoint1 = WorldPoint2;
        normal1 = normal2;
    }

    // check for empty edges (like nothing in the viewport)
    if( ll_empty( edges ) ) {
        ll_delete( edges, NULL );
        edges = NULL;
    }

    return(edges);
}

/*
    Draw one scanline of a polygon given the scanline, the active edges,
    a DrawState, the image, and some Lights (for Phong shading only).
 */
static void fillScan(Polygon* poly, int scan, LinkedList *active, DrawState* ds, Image *src,Lighting *light) {
  Edge *p1, *p2;
  float curZ;
  float dzPerColumn;
  float dcPerColumn[3];
  float dpPerColumn[3];
  float dnPerColumn[3];
  Color curC;
  Point curP;
  Vector curN;
  int i;

    // loop over the list
  p1 = ll_head( active );
  while(p1) {
        // the edges have to come in pairs, draw from one to the next
      p2 = ll_next( active );
      if( !p2 ) {
          printf("bad bad bad (your edges are not coming in pairs)\n");
          break;
      }
      

        // if the xIntersect values are the same, don't draw anything.
        // Just go to the next pair.
      if( p2->xIntersect == p1->xIntersect ) {
          p1 = ll_next( active );
          // printf("good good good pairs \n" );
          continue;
      }

      if(p1->xIntersect < 0){ // if starts to draw before the left of the image
        p1->xIntersect = 0;
      }
      if(p2->xIntersect > src->cols){ // if ends drawing beyound the right side of the image
        p2->xIntersect = src->cols;
      }

      int colStart = (int)(p1->xIntersect+0.5);
      int colEnd = (int)(p2->xIntersect+0.5);
      int row = scan;

      curZ = p1->zIntersect;
      dzPerColumn = (p2->zIntersect - p1->zIntersect) / (colEnd - colStart);

      // shade phong logic
      curP = p1->pIntersect;
      curN = p1->nIntersect;

      dpPerColumn[0] =  (p2->pIntersect.val[0] - p1->pIntersect.val[0]) / (colEnd - colStart);
      dpPerColumn[1] =  (p2->pIntersect.val[1] - p1->pIntersect.val[1]) / (colEnd - colStart);
      dpPerColumn[2] =  (p2->pIntersect.val[2] - p1->pIntersect.val[2]) / (colEnd - colStart);

      dnPerColumn[0] =  (p2->nIntersect.val[0] - p1->nIntersect.val[0]) / (colEnd - colStart);
      dnPerColumn[1] =  (p2->nIntersect.val[1] - p1->nIntersect.val[1]) / (colEnd - colStart);
      dnPerColumn[2] =  (p2->nIntersect.val[2] - p1->nIntersect.val[2]) / (colEnd - colStart);

// shade gouraud logic
      curC = p1->cIntersect;



      dcPerColumn[0] = (p2->cIntersect.rgb[0] - p1->cIntersect.rgb[0]) / (colEnd - colStart);
      dcPerColumn[1] = (p2->cIntersect.rgb[1] - p1->cIntersect.rgb[1]) / (colEnd - colStart);
      dcPerColumn[2] = (p2->cIntersect.rgb[2] - p1->cIntersect.rgb[2]) / (colEnd - colStart);

// calculates viewer vector once
    

      // printf("polygon fillScan called \n");
      for (i = colStart; i< colEnd; i++){
        // printf(" curZ %f  \n", curZ );

        if(ds->zBufferFlag == 0)
            goto label1;
        if((curZ - src->data[row][i].z) > 0.001 ){//using 0.01 as the epsilon
        label1:
            if(ds->shade == ShadeFrame){
                polygon_draw(poly, ds, src);
            }
            else if(ds->shade == ShadeConstant){
                Color colr;
                //alpha blending
                float r = ds->alpha*ds->color.rgb[0] + (1-ds->alpha)*src->data[row][i].rgb[0];
                float g = ds->alpha*ds->color.rgb[1] + (1-ds->alpha)*src->data[row][i].rgb[1];
                float b = ds->alpha*ds->color.rgb[2] + (1-ds->alpha)*src->data[row][i].rgb[2];
                Color_set(&colr, r, g, b);
                // printf("r, g, b == %f , %f, %f\n", r, g, b);
                image_setColor(src, row, i, colr);
            }else if(ds->shade == ShadeDepth){
                Color c;
                float r = ds->alpha*ds->color.rgb[0] + (1-ds->alpha)*src->data[row][i].rgb[0];
                float g = ds->alpha*ds->color.rgb[1] + (1-ds->alpha)*src->data[row][i].rgb[1];
                float b = ds->alpha*ds->color.rgb[2] + (1-ds->alpha)*src->data[row][i].rgb[2];
                float z = 1/curZ;
                Color_set(&c,(1-z)*r, (1-z)*g, (1-z)*b);
                image_setColor(src, row, i, c);
            }else if(ds->shade == ShadeFlat){
                // printfZ"in flat\n");
                Color colr;
                //alpha blending
                float r = ds->alpha*ds->flatColor.rgb[0] + (1-ds->alpha)*src->data[row][i].rgb[0];
                float g = ds->alpha*ds->flatColor.rgb[1] + (1-ds->alpha)*src->data[row][i].rgb[1];
                float b = ds->alpha*ds->flatColor.rgb[2] + (1-ds->alpha)*src->data[row][i].rgb[2];
                Color_set(&colr, r, g, b);
                // printf("r, g, b == %f , %f, %f\n", r, g, b);
                image_setColor(src, row, i, colr);
            }else if(ds->shade == ShadeGouraud){
                Color c;//reserved for the final color
                // Alphainfo info;//info structure for list in the fpixel
                // Alphainfo *p, *q; //while loop variables
                float r = src->data[row][i].rgb[0];
                float g = src->data[row][i].rgb[1];
                float b = src->data[row][i].rgb[2];

                // if(src->data[row][i].list == NULL){
                //     src->data[row][i].list = ll_new();
                // }

                // if(src->data[row][i].list->root == NULL){//adding the first node to background image
                //     Color ctemp;
                //     Color_set(&ctemp, src->data[row][i].rgb[0], src->data[row][i].rgb[1], src->data[row][i].rgb[2]); 
                //     info.color = ctemp;//color of the image background
                //     info.alpha = 1;// opaque image background;
                //     info.depth = 1;//backclip plane (this is a 1/z value)
                // }else{//not the first node
                //     Color ctemp;
                //     Color_set(&ctemp, curC.rgb[0]/curZ,curC.rgb[1]/curZ,curC.rgb[2]/curZ);
                //     info.color = ctemp;//color of the image background
                //     info.alpha = poly->alpha;// opaque image background;
                //     info.depth = curZ;//backclip plane (this is a 1/z value)
                // }
                // ll_insert(src->data[row][i].list , &info, compZIntersect);

                //Traverse the linked list and calculate the color from back to front using alpha blending
                // q = ll_head(src->data[row][i].list);
                // p = ll_next(src->data[row][i].list);
                // while(p != NULL){
                //     r += q->alpha*(q->color.rgb[0]) + (1-q->alpha)*r;
                //     g += q->alpha*(q->color.rgb[1]) + (1-q->alpha)*g;
                //     b += q->alpha*(q->color.rgb[2]) + (1-q->alpha)*b;

                //     q = p;
                //     p = ll_next(src->data[row][i].list);
                // }                
                
                r = ds->alpha*(curC.rgb[0]/curZ) + (1-ds->alpha)*src->data[row][i].rgb[0];
                g = ds->alpha*(curC.rgb[1]/curZ) + (1-ds->alpha)*src->data[row][i].rgb[1];
                b = ds->alpha*(curC.rgb[2]/curZ) + (1-ds->alpha)*src->data[row][i].rgb[2];

                Color_set(&c, r, g, b);
                image_setColor(src, row, i, c);

                // Color_set(&curC,(z)*curC.rgb[0], (z)*curC.rgb[1], (z)*curC.rgb[2]);
                // printf("r, g, b == %f , %f, %f\n",  curC.rgb[0],  curC.rgb[1],  curC.rgb[2]);
                // printf("dcPerColumn[] == %f , %f, %f\n",  dcPerColumn[0],  dcPerColumn[1],  dcPerColumn[2]);
                
            }else if(ds->shade == ShadePhong){
                Color c;
                Point interp;
                Vector interpN;
                Vector viewer;


                point_set3D(&interp,curP.val[0]/curZ,curP.val[1]/curZ,curP.val[2]/curZ);
                vector_set(&interpN,curN.val[0]/curZ,curN.val[1]/curZ,curN.val[2]/curZ);

                vector_set(&viewer,(ds->viewer.val[0] - interp.val[0]), (ds->viewer.val[1] - interp.val[1]), (ds->viewer.val[2] - interp.val[2]));

                // point_set3D(&interp, p->vertex[i].val[0]*curZ,p->vertex[i].val[1]*curZ,p->vertex[i].val[2]*curZ);
                // lighting_shading(light, &interpN, &viewer, &interp, &ds->body, &ds->surface, ds->surfaceCoeff, poly->oneSided, &c);
                lighting_shading(light, &interpN, &viewer, &interp, &ds->body, &ds->surface, ds->surfaceCoeff, poly->oneSided, &c);
                image_setColor(src, row, i, c);

                // need updates for point and normal?
                // need lighting

            }
            // printf("hi %d \n", colEnd);
            src->data[row][i].z = curZ;
        }
        // updates
        curZ += dzPerColumn;
        curC.rgb[0]+= dcPerColumn[0];
        curC.rgb[1]+= dcPerColumn[1];
        curC.rgb[2]+= dcPerColumn[2];

        curN.val[0]+= dnPerColumn[0];
        curN.val[1]+= dnPerColumn[1];
        curN.val[2]+= dnPerColumn[2];

        curP.val[0]+= dpPerColumn[0];
        curP.val[1]+= dpPerColumn[1];
        curP.val[2]+= dpPerColumn[2];

      }

        // move ahead to the next pair of edges
      p1 = ll_next( active );
  }
    return;
}


/*
 Draw one scanline of a polygon given the scanline, the active edges,
 a DrawState, the image, and some Lights (for Phong shading only) using a bitmap
 */
static void fillScanBitmap( int scan, LinkedList *active, Image *src,Image *bitmap ) {
    Edge *p1, *p2;
    int i;
    
    // loop over the list
    p1 = ll_head( active );
    while(p1) {
        // the edges have to come in pairs, draw from one to the next
        p2 = ll_next( active );
        if( !p2 ) {
            printf("bad bad bad (your edges are not coming in pairs)\n");
            break;
        }
        
        // if the xIntersect values are the same, don't draw anything.
        // Just go to the next pair.
        if( p2->xIntersect == p1->xIntersect ) {
            p1 = ll_next( active );
            printf("good good good pairs \n" );
            continue;
        }
        
        /**** Your code goes here ****/
        if(p1->xIntersect < 0){
            p1->xIntersect = 0;
        }
        if(p2->xIntersect > src->cols){
            p2->xIntersect = src->cols;
        }
        int colStart = (int)(p1->xIntersect);
        int colEnd = (int)(p2->xIntersect+1);
        int row = scan;
        
        
        int incrementor=0;
        int bitmapRow=0;
        
        
        for (i=colStart; i< colEnd; i++){
            incrementor++;
            if (incrementor % 8 == 0) {
                bitmapRow++;
            }
            if (bitmapRow>bitmap->rows-1) {
                bitmapRow=0;
            }
            
            image_setf(src, row, i, bitmap->data[bitmapRow][incrementor]);
        }
        // identify the starting column
        // clip to the left side of the image
        // identify the ending column
        // clip to the right side of the image
        // loop from start to end and color in the pixels
        
        // move ahead to the next pair of edges
        p1 = ll_next( active );
    }
    
    return;
}

/* 
     Process the edge list, assumes the edges list has at least one entry
*/
static int processEdgeList(Polygon* p, LinkedList *edges, DrawState* ds, Image *src, Image *bitmap,Lighting *Lights) {
    LinkedList *active = NULL;
    LinkedList *tmplist = NULL;
    LinkedList *transfer = NULL;
    Edge *current;
    Edge *tedge;
    int scan = 0;

    active = ll_new( );
    tmplist = ll_new( );
//
//    // get a pointer to the first item on the list and reset the current pointer
    current = ll_head( edges );
    // start at the first scanline and go until the active list is empty
    for(scan = current->yStart ;scan < src->rows;scan++ ) {

        // grab all edges starting on this row
        while( current != NULL && current->yStart == scan ) {
            ll_insert( active, current, compXIntersect );
            current = ll_next( edges );
        }
//        // current is either null, or the first edge to be handled on some future scanline
//
        if( ll_empty(active) ) {
            break;
        }
//
////        checks to see if a bitmap has been added, defaults to regular scan if not
        if (bitmap == NULL) {
            // printf("polygon process called \n");
            fillScan(p, scan, active, ds, src,Lights);
        }
//
////        if so, bitmap scan
        else{
            fillScanBitmap( scan, active, src, bitmap);
        }
//
//        // remove any ending edges and update the rest
        for( tedge = ll_pop( active ); tedge != NULL; tedge = ll_pop( active ) ) {

            // keep anything that's not ending
            if( tedge->yEnd > scan ) {
                float a = 1.0;

                // update the edge information with the dPerScan values
                tedge->xIntersect += tedge->dxPerScan;
                tedge->zIntersect += tedge->dzPerScan;
                tedge->cIntersect.rgb[0] += tedge->dcPerScan[0];
                tedge->cIntersect.rgb[1] += tedge->dcPerScan[1];
                tedge->cIntersect.rgb[2] += tedge->dcPerScan[2];

                tedge->nIntersect.val[0] += tedge->dnPerScan[0];
                tedge->nIntersect.val[1] += tedge->dnPerScan[1];
                tedge->nIntersect.val[2] += tedge->dnPerScan[2];

                tedge->pIntersect.val[0] += tedge->dpPerScan[0];
                tedge->pIntersect.val[1] += tedge->dpPerScan[1];
                tedge->pIntersect.val[2] += tedge->dpPerScan[2];
                // printf("polygon process called \n");
                // adjust in the case of partial overlap
                if( tedge->dxPerScan < 0.0 && tedge->xIntersect < tedge->x1 ) {
                    a = (tedge->xIntersect - tedge->x1) / tedge->dxPerScan;
                    tedge->xIntersect = tedge->x1;
                }
                else if( tedge->dxPerScan > 0.0 && tedge->xIntersect > tedge->x1 ) {
                    a = (tedge->xIntersect - tedge->x1) / tedge->dxPerScan;
                    tedge->xIntersect = tedge->x1;
                }

                ll_insert( tmplist, tedge, compXIntersect );
            }
        }
//
        transfer = active;
        active = tmplist;
        tmplist = transfer;

    }

    // get rid of the lists, but not the edge records
    ll_delete(active, NULL);
    ll_delete(tmplist, NULL);

    return(0);
}


/****************************************
End Scanline Fill
*****************************************/


// returns an allocated Polygon pointer initialized so that numVertex is 0 and vertex is NULL.
Polygon *polygon_create(){
	Polygon *p=malloc(sizeof(Polygon));
    if(NULL == p){
        printf("ERROR: polygon_create >> malloc failed!\n");
        exit(-1);
    }
    p->vertex=NULL;
    p->normal=NULL;
    p->color=NULL;
    p->vertexWorld = NULL;
    p->numVertex=0;
    p->zBuffer = 1;
    p->alpha = 1;
    p->oneSided = 0;
    return p;
}

// returns an allocated Polygon pointer with the vertex list
//initialized to a copy of the points in vlist.
Polygon *polygon_createp(int numV, Point *vlist){
	Polygon *p=malloc(sizeof(Polygon));
    if(NULL == p){
        printf("ERROR: polygon_createp >> malloc failed!\n");
        exit(-1);
    }
    p->vertex=malloc(sizeof(Point)*numV);
    p->color=malloc(sizeof(Color)*numV);
    p->normal=malloc(sizeof(Vector)*numV);
    // p->vertexWorld=malloc(sizeof(Point)*numV);
    for (int i = 0; i < numV; i++){
        point_copy(&(p->vertex[i]),&vlist[i]);
    }
    p->numVertex=numV;
    p->zBuffer = 1;
    p->alpha = 1;
    p->oneSided = 0;
    return p;
}

// frees the internal data for a Polygon and the Polygon pointer
void polygon_free(Polygon *p){
	if (p!=NULL) {
        if (p->vertex!=NULL) {
            free(p->vertex);
        }
        if (p->color!=NULL) {
            free(p->color);
        }
        if (p->normal!=NULL) {
            free(p->normal);
        }
        if (p->vertexWorld!=NULL && p->phong == 1) {
            free(p->vertexWorld);
        }
        if(p->normalWorld != NULL && p->phong == 1){
            free(p->normalWorld);
        }
        free(p);
    }
}

//initializes the existing Polygon to an empty Polygon.
void polygon_init(Polygon *p){
	p->vertex=NULL;
    p->color=NULL;
    p->normal=NULL;  
    p->vertexWorld=NULL; 
    p->normalWorld = NULL;
    p->numVertex=0;
    p->zBuffer = 1;
    p->oneSided = 0;
    p->alpha = 1;
    p->phong = 0;
}

// initializes the vertex array to the points in vlist
void polygon_set(Polygon *p, int numV, Point *vlist){
	if (p->vertex!=NULL) {
        free(p->vertex);
    }
    p->vertex=malloc(sizeof(Point)*numV);
    p->normal=malloc(sizeof(Vector)*numV);
    p->color=malloc(sizeof(Color)*numV);
    for (int i = 0; i < numV; i++)
    {
        point_copy(&(p->vertex[i]),&vlist[i]);
    }
    p->numVertex = numV;
    p->zBuffer = 1;
    p->oneSided = 0;
    p->alpha = 1;
}



// frees the internal data and resets the fields.
void polygon_clear(Polygon *p){
	if(p->vertex != NULL){
        free(p->vertex);
    }
    if(p->color != NULL){
        free(p->color);
    }
    if(p->normal != NULL){
        free(p->normal);
    }
    if(p->vertexWorld != NULL && p->phong == 1){
        free(p->vertexWorld);
    }
    if(p->normalWorld != NULL && p->phong == 1){
        free(p->normalWorld);
    }
    p->normalWorld = NULL;
    p->vertex = NULL;
    p->color = NULL;
    p->normal = NULL;
    p->vertexWorld = NULL;
    p->numVertex = 0;
    p->zBuffer = 1;
    p->oneSided = 0;
    p->alpha = 1;
    p->phong = 0;

}

//sets the z-buffer flag to the given value
void polygon_zBuffer(Polygon *p, int flag){
	if(NULL != p){
        p->zBuffer = flag;
    }
}

void polygon_setAlpha(Polygon *p, float a){
    if(NULL != p){
        p->alpha = a;
    }
}

//De-allocates/allocates space and copies the 
//vertex and color data from one polygon to the other
void polygon_copy(Polygon *to, Polygon *from){
	if(NULL != to && NULL != from){
        polygon_clear(to); 
        to->vertex = malloc(sizeof(Point)*from->numVertex);
        to->color = malloc(sizeof(Color)*from->numVertex);
        to->normal = malloc(sizeof(Vector)*from->numVertex);
        if(from->phong == 1){
            printf("not null\n" );
            to->vertexWorld = malloc(sizeof(Point)*from->numVertex);
            to->normalWorld = malloc(sizeof(Vector)*from->numVertex);
        }
        int i;
        for (i=0; i < from->numVertex; i++)
        {
          to->vertex[i] = from->vertex[i];
          to->color[i] = from->color[i];
          to->normal[i] = from->normal[i];
            if(from->phong == 1){
                printf("not null1\n" );
                to->vertexWorld[i] = from->vertexWorld[i];
                to->normalWorld[i] = from ->normalWorld[i];
            }

        }
        to->phong = from->phong;
        to->numVertex = from->numVertex;
        to->zBuffer = from->zBuffer;
        to->oneSided = from->oneSided;
        to->alpha = from->alpha;
    }
}

// prints polygon data to the stream designated by the FILE pointer
void polygon_print(Polygon *p, FILE *fp){
	if(NULL != p && NULL != fp){
        int i;
        fprintf(fp, "%d\n",p->numVertex);//writes number of vertecies
        fprintf(fp, "%d\n",p->zBuffer);//writes the zBuffer value
        for(i=0; i<p->numVertex; i++){
            fprintf(fp, "%.3f %.3f %.3f %.3f\n",p->vertex[i].val[0], p->vertex[i].val[1], p->vertex[i].val[2], p->vertex[i].val[3]);
        }
    }
}

// draw the outline of the polygon using color c.
void polygon_draw(Polygon *p, DrawState* ds, Image *src){
	Line l;
    int i;
    for (i=0; i< p->numVertex-1; i++){
        line_set2D(&l,p->vertex[i].val[0], p->vertex[i].val[1], p->vertex[i+1].val[0], p->vertex[i+1].val[1]);
        line_draw(&l, src, ds->color);
    }
    line_set2D(&l, p->vertex[i].val[0], p->vertex[i].val[1], p->vertex[0].val[0], p->vertex[0].val[1]);
    line_draw(&l, src, ds->color);
}

/*
    Draws a filled polygon of the specified color into the image src.
 */
void polygon_drawFill(Polygon *p, DrawState* ds, Image *src,Lighting * Lights ) {
    LinkedList *edges = NULL;

    // set up the edge list
    edges = setupEdgeList( p, ds, src );
    if( !edges )
        return;
    // printf("polygon drawfill called \n");
    // process the edge list (should be able to take an arbitrary edge list)
    processEdgeList(p, edges, ds, src, NULL, Lights);

    // clean up
    ll_delete( edges, (void (*)(const void *))free );

    return;
}


/*
 Draws a filled polygon of the specified color into the image src, allows for bitmap texture insert
 */
void polygon_drawFillBitmap(Polygon *p, DrawState* ds, Image *src, Image *bitmap ) {
    LinkedList *edges = NULL;
    
//    passing unitialized pointer, will not be used
    
    // set up the edge list
    edges = setupEdgeList( p, ds, src );
    if( !edges )
        return;
    // process the edge list (should be able to take an arbitrary edge list)
    processEdgeList(p, edges, ds, src, bitmap,NULL);
    
    // clean up
    ll_delete( edges, (void (*)(const void *))free );
    
    return;
}

/*
Auxilary function to be used with qsort
*/
int compare (const void * a, const void * b)
{
  return ( *(int*)a - *(int*)b );
}

//draw the filled polygon using color c with the
//Barycentric coordinates algorithm.
void polygon_drawFillB(Polygon *p, DrawState* ds, Image *src){
	const float epsilon = 0.00001;
	float alpha, beta, gamma;
	float xlist[3] = {p->vertex[0].val[0], p->vertex[1].val[0], p->vertex[2].val[0]};
	float ylist[3] = {p->vertex[0].val[1], p->vertex[1].val[1], p->vertex[2].val[1]};
	qsort(xlist, 3, sizeof(float), compare);
	qsort(ylist, 3, sizeof(float), compare);
	int Xmin = (int)(xlist[0]+0.5);
	int Ymin = (int)(ylist[0]+0.5);
	int Xmax = (int)(xlist[2]+0.5);
	int Ymax = (int)(ylist[2]+0.5);

	for (int i = Ymin; i < Ymax; i++){
		for (int j = Xmin; j < Xmax; j++){
			beta = Aux_implicit_line(p->vertex[0], p->vertex[2], j,i)/ Aux_implicit_line(p->vertex[0], p->vertex[2],p->vertex[1].val[0], p->vertex[1].val[1]);
			gamma = Aux_implicit_line(p->vertex[0], p->vertex[1], j,i)/ Aux_implicit_line(p->vertex[0], p->vertex[1],p->vertex[2].val[0], p->vertex[2].val[1]);
			alpha = 1- beta - gamma; 

			if (beta<-epsilon || gamma <-epsilon || alpha<-epsilon){			
				continue;
			}
			else{
				image_setColor(src,i,j,ds->color);
			}
		}
	}
}


/*
Draws a filled polygon (triangle) with color gradiant using Barycentric coordinates algorithm.
Parameters:
p : pointer to a polygon structure
src : pointer to an image structure
c1, c2, c3: Are color structures that will set the color of each corner of the triangle
*/
void polygon_drawFillB_Gradient(Polygon *p, Image *src, Color c1 ,Color c2, Color c3){
    const float epsilon = 0.00001;
    float alpha, beta, gamma;
    float xlist[3] = {p->vertex[0].val[0], p->vertex[1].val[0], p->vertex[2].val[0]};
    float ylist[3] = {p->vertex[0].val[1], p->vertex[1].val[1], p->vertex[2].val[1]};
    qsort(xlist, 3, sizeof(float), compare);
    qsort(ylist, 3, sizeof(float), compare);
    int Xmin = (int)(xlist[0]+0.5);
    int Ymin = (int)(ylist[0]+0.5);
    int Xmax = (int)(xlist[2]+0.5);
    int Ymax = (int)(ylist[2]+0.5);

    
    for (int i = Ymin; i < Ymax; i++){
        for (int j = Xmin; j < Xmax; j++){
            beta = Aux_implicit_line(p->vertex[0], p->vertex[2], j,i)/ Aux_implicit_line(p->vertex[0], p->vertex[2],p->vertex[1].val[0], p->vertex[1].val[1]);
            gamma = Aux_implicit_line(p->vertex[0], p->vertex[1], j,i)/ Aux_implicit_line(p->vertex[0], p->vertex[1],p->vertex[2].val[0], p->vertex[2].val[1]);
            alpha = 1- beta - gamma;
            
            if (beta<-epsilon || gamma <-epsilon || alpha<-epsilon){ //if outside of the triangle
                    continue;
            }
            else{ // if inside of the thriangle
                float im_red =(alpha*c1.rgb[0]) + (beta*c2.rgb[0]) + (gamma*c3.rgb[0]);
                float im_green =(alpha*c1.rgb[1]) + (beta*c2.rgb[1]) + (gamma*c3.rgb[1]);
                float im_blue =(alpha*c1.rgb[2]) + (beta*c2.rgb[2]) + (gamma*c3.rgb[2]);
                
                image_setc(src,i,j,0,im_red);
                image_setc(src,i,j,1,im_green);
                image_setc(src,i,j,2,im_blue);
            }
        }
    }
}

/*
Auxilary function to calculate the distance of point p_in from
the line segment p1p2
*/

float Aux_implicit_line(Point p1, Point p2, float x,float y){
	float x0=p1.val[0];
	float y0=p1.val[1];

	float x1=p2.val[0];
	float y1=p2.val[1];
	x += 0.5;
	y += 0.5;
	return (y0-y1)*x + (x1-x0)*y + x0*y1 - x1*y0;
}

/*
    normalizes the vectors of the polygon
*/
void polygon_normalize(Polygon *p){
    int i;
    for(i=0;i<p->numVertex;i++){
        p->vertex[i].val[0]/=p->vertex[i].val[3];
        p->vertex[i].val[1]/=p->vertex[i].val[3];
    }
}

//sets the oneSided field to the value.
void polygon_setSided(Polygon *p, int oneSided){
    if(NULL != p){
        p->oneSided = oneSided;
    }
}

// initializes the color array to the colors in clist
void polygon_setColors(Polygon *p, int numV, Color *clist){
    if (p->color!=NULL) {
        free(p->color);
    }
    p->color=malloc(sizeof(Color)*numV);
    for (int i = 0; i < numV; i++)
    {
        Color_copy(&(p->color[i]),&clist[i]);
    }
}


//– initializes the normal array to the vectors in nlist.
void polygon_setNormals(Polygon *p, int numV, Vector *nlist){
    if (p->normal!=NULL) {
        free(p->normal);
    }
    p->normal=malloc(sizeof(Vector)*numV);
    for (int i = 0; i < numV; i++)
    {
        vector_copy(&(p->normal[i]),&nlist[i]);
    }
}

//initializes the vertex list to the points in vlist, the colors to the colors in clist, the normals to the
//vectors in nlist, and the zBuffer and oneSided flags to their respectively values.
void polygon_setAll(Polygon *p, int numV, Point *vlist, Color *clist, Vector *nlist, int zBuffer, int oneSided){
    if(NULL != p && NULL != vlist && NULL != clist && NULL != nlist){
        if(p->vertex != NULL){
            free(p->vertex);
        }
        if(p->color != NULL){
            free(p->color);
        }
        if(p->normal != NULL){
            free(p->normal);
        }
        p->normal = malloc(sizeof(Vector)*numV);
        p->color = malloc(sizeof(Color)*numV);
        p->vertex = malloc(sizeof(Point)*numV);
        for (int i = 0; i < numV; i++)
        {   
            point_copy(&(p->vertex[i]), &vlist[i]);
            vector_copy(&(p->normal[i]), &nlist[i]);
            Color_copy(&(p->color[i]), &clist[i]);
        }
        p->numVertex = numV;
        p->zBuffer = zBuffer;
        p->oneSided = oneSided;
    }
}

//draw the filled polygon using the given DrawState. The shade field of the DrawState determines
//how the polygon should be rendered. The Lighting parameter should be NULL unless you are doing
//Phong shading.
void polygon_drawShade(Polygon *p, Image *src, DrawState *ds, Lighting *light){
    if(NULL != p && NULL != src && NULL != ds && NULL != light){
        if(ds->shade == ShadeFrame){
            polygon_draw(p, ds, src);
        }else if(ds->shade == ShadeConstant || ds->shade == ShadeDepth){
            polygon_drawFill(p,ds,src,NULL );
        }
        else if(ds->shade == ShadeFlat || ds->shade == ShadeGouraud){
            // polygon_shade(p, light, ds);
            // printf("rgb    %f,%f,%f \n",ds->flatColor.rgb[0],ds->flatColor.rgb[1],ds->flatColor.rgb[2]);
            polygon_drawFill(p, ds, src,NULL);
        }
        else if(ds->shade == ShadePhong){
            polygon_drawFill(p, ds, src,light);
        }
    }
}

//For the ShadeFlat
//and ShadeGouraud cases of the shade field of DrawState, calculate colors at each vertex and create
//and fill out the color array of the Polygon data structure. For ShadeFlat, use the average surface
//normal and average polygon location to calculate one color and set the color at each vertex to the
//calculated value. For ShadeGouraud use the surface normals and locations of each vertex
void polygon_shade(Polygon *p, Lighting *lighting, DrawState *ds){
    if(NULL != p && NULL != lighting &&  NULL != ds){
        if(ds->shade == ShadeFlat){
            Point avgVertex = point_average(p->vertex, p->numVertex);
            Vector avgNormal = vector_average(p->normal, p->numVertex);
            Vector viewer;
            vector_set(&viewer, (ds->viewer.val[0] - avgVertex.val[0]), (ds->viewer.val[1] - avgVertex.val[1]), (ds->viewer.val[2] - avgVertex.val[2]));

            lighting_shading(lighting, &avgNormal, &viewer, &avgVertex, &ds->body, &ds->surface, ds->surfaceCoeff, p->oneSided, &ds->flatColor);
        }else if(ds->shade == ShadeGouraud){
            Vector viewer;
            for (int i = 0; i < p->numVertex; i++)
            {
                vector_set(&viewer, (ds->viewer.val[0] - p->vertex[i].val[0]), (ds->viewer.val[1] - p->vertex[i].val[1]), (ds->viewer.val[2] - p->vertex[i].val[2]));

                // printf("i %d  surfaceCoeff: %f \n", i,ds->surfaceCoeff);
                // printf("rcolor  gcolor bcolor  %f   %f  %f \n", p->color[i].rgb[0],p->color[i].rgb[1],p->color[i].rgb[2]);
                // printf("rsurface  gsurface bsurface  %f   %f  %f \n", ds->surface.rgb[0],ds->surface.rgb[1],ds->surface.rgb[2]);
                // printf("rbody gbody bbody  %f   %f  %f \n", ds->body.rgb[0],ds->body.rgb[1],ds->body.rgb[2]);
                // point_print(&p->normal[i],stdout);
                // point_print(&p->vertex[i],stdout);

                lighting_shading(lighting, &p->normal[i], &viewer, &p->vertex[i], &ds->body, &ds->surface, ds->surfaceCoeff, p->oneSided, &p->color[i]);

                // printf("r  g  b  %f   %f  %f \n", p->color[i].rgb[0],p->color[i].rgb[1],p->color[i].rgb[2]);
            }
        }
        // else if(ds->shade == ShadePhong){
        //     for (int i = 0; i < p->numVertex; i++)
        //     {
                // vector_set(&viewer, (ds->viewer.val[0] - p->vertex[i].val[0]), (ds->viewer.val[1] - p->vertex[i].val[1]), (ds->viewer.val[2] - p->vertex[i].val[2]));

                // printf("i %d  surfaceCoeff: %f \n", i,ds->surfaceCoeff);
                // printf("rcolor  gcolor bcolor  %f   %f  %f \n", p->color[i].rgb[0],p->color[i].rgb[1],p->color[i].rgb[2]);
                // printf("rsurface  gsurface bsurface  %f   %f  %f \n", ds->surface.rgb[0],ds->surface.rgb[1],ds->surface.rgb[2]);
                // printf("rbody gbody bbody  %f   %f  %f \n", ds->body.rgb[0],ds->body.rgb[1],ds->body.rgb[2]);
                // point_print(&p->normal[i],stdout);
                // point_print(&p->vertex[i],stdout);

                // lighting_shading(lighting, &p->normal[i], &viewer, &p->vertex[i], &ds->body, &ds->surface, ds->surfaceCoeff, p->oneSided, &p->color[i]);

                // printf("r  g  b  %f   %f  %f \n", p->color[i].rgb[0],p->color[i].rgb[1],p->color[i].rgb[2]);
            // }




        // }
    }
}













