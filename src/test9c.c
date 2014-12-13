/*
	Bruce A. Maxwell
	Fall 2014

	Test program for project 9
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "module.h"
#include "drawState.h"
#include "line.h"
#include "point.h"
#include "polyline.h"
#include "polygon.h"
#include "circle.h"
#include "color.h"
#include "Image.h"
#include "matrix.h"
#include "view.h"
#include "light.h"
#include "plyRead.h"



int main(int argc, char *argv[]) {
  Image *src;
  Matrix VTM;
  Matrix GTM;
  Module *starfury;
  Module *scene;

  // make it big and shrink the result
  int rows = 2000;
  int cols = 2000;
  int i;

  int nPolygons;
  Polygon *plist;
  Color *clist;

  Color AmbientColor;
  Color PointColor;
  Color PointColor2;
  Color SurfaceColor;

  Lighting *light;
  DrawState *ds;

  View3D view;

  char buffer[256];

  double angle = 0.0;

	Color_set(&AmbientColor, 0.1, 0.1, 0.1);
	Color_set(&PointColor, 0.7, 0.6, 0.45);
	Color_set(&PointColor2, 0.2, 0.3, 0.45);
	Color_set(&SurfaceColor, 0.2, 0.2, 0.2);

  if(argc > 2) {
    angle = atof(argv[2]);
    angle = angle * M_PI / 180.0;
  }

  // initialize the image
  src = image_create(rows, cols);

  // initialize matrices
  matrix_identity(&GTM);
  matrix_identity(&VTM);

  // set the View parameters
  point_set3D(&(view.vrp), 0.0, 0.0, -15.0);
  vector_set(&(view.vpn), 0.0, 0.0, 1.0);
  vector_set(&(view.vup), 0.0, 1.0, 0.0);
  view.d = 2.0;
  view.du = 1.4;
  view.dv = 1.4;
  view.f = 0.0;
  view.b = 100;
  view.screenx = cols;
  view.screeny = rows;
  matrix_setView3D(&VTM, &view);

  // print out VTM
  printf("Final VTM: \n");
  matrix_print(&VTM, stdout);

  readPLY(argv[1], &nPolygons, &plist, &clist, 1);

  printf("Read %d polygons\n", nPolygons);

  starfury = module_create();
  module_surfaceColor(starfury, &SurfaceColor);
  for(i=0;i < nPolygons;i++) {
	  printf("%.2f %.2f %.2f\n", clist[i].rgb[0], clist[i].rgb[1], clist[i].rgb[2] );
    module_bodyColor(starfury, &clist[i]);
    module_polygon(starfury, &(plist[i]));
  }

  scene = module_create();
  module_rotateY(scene, cos(angle), sin(angle));
  module_translate(scene, -1.0, -2.0, 0.0);
  module_module(scene, starfury);
  module_translate(scene, 3, 3, 3);
  module_module(scene, starfury);

  light = lighting_create();

  // create one point light
  light->light[0].type = LightPoint;
  light->light[0].position.val[0] = 0.0;
  light->light[0].position.val[1] = 0.0;
  light->light[0].position.val[2] = -50.0;
  light->light[0].color = PointColor;

  // create another point light
  light->light[1].type = LightPoint;
  light->light[1].position.val[0] = 50.0;
  light->light[1].position.val[1] = -20.0;
  light->light[1].position.val[2] = -50.0;
  light->light[1].color = PointColor2;

  // create an ambient light
  light->light[2].type = LightAmbient;
  light->light[2].color = AmbientColor;
  light->nLights = 3;

  matrix_identity(&GTM);

  ds = drawstate_create();
  point_copy( &(ds->viewer), &(view.vrp) );
  ds->shade = ShadeGouraud;

  printf("shading frame\n");
  module_draw(scene, &VTM, &GTM, ds, light, src);

  // write out the image
  printf("Writing out high resolution image\n");
  sprintf(buffer, "starfuries.ppm");
  image_write(src, buffer);

  printf("Converting to low resolution image\n");
  sprintf(buffer, "convert -resize 25%% starfuries.ppm starfuries.png");
  system(buffer);

  printf("Deleting high-res image\n");
  //  system("rm starfuries.ppm");

  // ought to free stuff here
	image_free(src);

  return(0);
}
