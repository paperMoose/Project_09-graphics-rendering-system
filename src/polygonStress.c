/*
 Assignment 4, polygon stress test
 
 C version
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "polygon.h"
#include "line.h"
#include "point.h"
#include "Image.h"
#include "color.h"

/*
 Program to stress test the polygon API
 */
int main(int argc, char *argv[]) {
    Image *src;
    const int rows = 300;
    const int cols = 400;
    Polygon *pdyn;
    Polygon pstat;
    Color Red;
    Color Blue;
    Point pt[5];
    
    Color_set(&Red, 8.0, 0.1, 0.2);
    Color_set(&Blue, 0.2, 0.1, 0.8);
    
    src = image_create(rows, cols);
    
    // make some random polygon
    point_set2D(&(pt[0]), 30, 30);
    point_set2D(&(pt[1]), 220, 30);
    point_set2D(&(pt[2]), 180, 200);
    point_set2D(&(pt[3]), 150, 130);
    point_set2D(&(pt[4]), 60, 180);
    
    printf("polygon_createp(): Creating pdyn with a set of points\n");
    pdyn = polygon_createp(5, pt);
    
    printf("pdyn:\n");
    polygon_print(pdyn, stdout);
    
    printf("polygon_init(): initializing pstat\n");
    polygon_init(&pstat);
    
    printf("pstat:\n");
    polygon_print(&pstat, stdout);
    
    printf("polygon_set(): adding points to pstat\n");
    polygon_set(&pstat, 3, pt);
    
    printf("pstat:\n");
    polygon_print(&pstat, stdout);
    
    printf("polygon_clear(): deleting points from pstat\n");
    polygon_clear(&pstat);
    
    printf("pstat:\n");
    polygon_print(&pstat, stdout);
    
    printf("polygon_copy(): copying from pdyn to pstat\n");
    polygon_copy(&pstat, pdyn);
    
    printf("pstat:\n");
    polygon_print(&pstat, stdout);
    
    printf("polygon_clear(): deleting points from pdyn\n");
    polygon_clear(pdyn);
    
    printf("pdyn:\n");
    polygon_print(pdyn, stdout);
    
    printf("polygon_set(): overwriting vertices in pstat\n");
    polygon_set(&pstat, 4, pt);
    
    printf("pstat:\n");
    polygon_print(&pstat, stdout);
    
    printf("polygon_copy(): copying from pstat to pdyn\n");
    polygon_copy(pdyn, &pstat);
    
    printf("pdyn:\n");
    polygon_print(pdyn, stdout);
    
    printf("polygon_set(): overwriting vertices in pstat\n");
    polygon_set(pdyn, 5, pt);
    
    printf("polygon_drawFill(): drawing pdyn\n");
    polygon_drawFill(pdyn, src, Blue);
    
    printf("polygon_set(): overwriting vertices in pstat\n");
    polygon_set(&pstat, 3, pt);
    
    printf("pstat:\n");
    polygon_print(&pstat, stdout);
    
    printf("polygon_drawFill(): drawing pstat\n");
    polygon_drawFill(&pstat, src, Red);
    
    printf("writing output\n");
    image_write(src, "test4c.ppm");
    
    polygon_free(pdyn);
    polygon_clear(&pstat);
    
    image_free(src);
    
    return(0);
}