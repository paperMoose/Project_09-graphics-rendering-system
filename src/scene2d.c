/*
  Ryan
  Fall 2014

  2D View test function
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "matrix.h"
#include "point.h"
#include "polyline.h"
#include "polygon.h"
#include "line.h"
#include "view.h"
#include "color.h"
#include "circle.h"

void setScene( Image *src );
void setScene( Image *src ) {
  int i, j;
  Color sky;

  Color_set(&sky, .145, .474, 0.9);

  for(i=0;i<src->rows;i++) {
    for(j=0;j<src->cols;j++) {
        image_setColor( src, i, j, sky );
      }
    }
  }

int main(int argc, char *argv[]) {
  const int nFrames = 100;
  const int rows = 500;
  const int cols = 500;
  View2D view;
  Matrix vtm;
  Matrix ltm;
  Polygon car,tempCar,windshield, tempWind,road,tempRoad;
  Circle wheels[2];
  Point carPoints[4],windPoints[5],wheelPoints[2],tempWheels[2],roadPoints[4];
  Color red,white,black,grey;
  int t;
  char filename[256];
  Image *src;

  src = image_create( rows, cols );

  Color_set(&red, 1, 0, 0);
  Color_set(&white, 1, 1, 1);
  Color_set(&grey, .2, .2, .2);
  Color_set(&black, 0, 0, 0);

  //point setting
  point_set2D(&carPoints[0],0,0);
  point_set2D(&carPoints[1],0,15);
  point_set2D(&carPoints[2],25,15);
  point_set2D(&carPoints[3],25,0);

  point_set2D(&windPoints[0],20,10);
  point_set2D(&windPoints[1],20,15);
  point_set2D(&windPoints[2],25,15);
  point_set2D(&windPoints[3],25,10);

  point_set2D(&wheelPoints[0],5,0);
  point_set2D(&wheelPoints[1],20,0);



  point_set2D(&roadPoints[0],-10000,-4000);
  point_set2D(&roadPoints[1],-10000,20);
  point_set2D(&roadPoints[2],10000,20);
  point_set2D(&roadPoints[3],10000,-4000);


  // set up a view centered on (1.5, 1.5) with x pointing right
  point_set2D( &(view.vrp), 1.8, 1.8 );
  view.dx = 100.0;
  vector_set( &(view.x), 1.0, 0.0, 0.0 );
  view.screenx = cols;
  view.screeny = rows;

  matrix_setView2D( &vtm, &view );
  printf("vtm:\n");
  matrix_print(&vtm, stdout);
  // setScene( src );


  // local translation
  polygon_set(&car,4,carPoints);

  // Color_set(&red,1,0,0);

  polygon_set(&windshield,4,windPoints);

  polygon_set(&road,4,roadPoints);



  // loop for some number of frames
  for(t=0;t<nFrames;t++) {
      setScene( src );

      // copies polygons to eliminate change of original points
      polygon_copy(&tempCar,&car);
      polygon_copy(&tempWind,&windshield);
      polygon_copy(&tempRoad,&road);

// copy circle points
      point_copy(&tempWheels[0],&wheelPoints[0]);
      point_copy(&tempWheels[1],&wheelPoints[1]);


  // local translation
  matrix_identity(&ltm);
  matrix_scale2D(&ltm,1-(t*.01),1-(t*0.01));
    matrix_translate2D(&ltm,-30+(t*.01),-45+(t*0.01));
  // Color_set(&red,1,0,0);

// car and windshield setting
   matrix_xformPolygon(&ltm,&tempCar);
   matrix_xformPolygon(&vtm,&tempCar);


  matrix_xformPolygon(&ltm,&tempWind);
  matrix_xformPolygon(&vtm,&tempWind);

    matrix_xformPolygon(&ltm,&tempRoad);
  matrix_xformPolygon(&vtm,&tempRoad);


// wheels setting
  matrix_xformPoint(&ltm,&tempWheels[0],&tempWheels[0]);
matrix_xformPoint(&ltm,&tempWheels[1],&tempWheels[1]);
matrix_xformPoint(&vtm,&tempWheels[0],&tempWheels[0]);
matrix_xformPoint(&vtm,&tempWheels[1],&tempWheels[1]);

      circle_set(&wheels[0],tempWheels[0],10-(t*.1));
      circle_set(&wheels[1],tempWheels[1],10-(t*.1));

// drawing
    polygon_drawFill(&tempRoad,src,black);
    polygon_drawFill(&tempCar,src,red);
    polygon_drawFill(&tempWind,src,white);
    circle_drawFill(&wheels[0],src,grey);
    circle_drawFill(&wheels[1],src,grey);



    printf("writing file\n");
    sprintf(filename, "frame-%04d.ppm", t );
    image_write( src, filename );

    // translate the view across the scene
    point_set2D( &(view.vrp), 1.8 - 40*(t+1)/nFrames, 1.8 );
    matrix_setView2D( &vtm, &view );
    matrix_print( &vtm, stdout );
  }

  printf("Making the .gif....");
  system("convert -delay 10 frame-*.ppm ../images/scene2d.gif");
  system("rm -f frame-*");

  // cleanup
  image_free( src );

  return(0);
}