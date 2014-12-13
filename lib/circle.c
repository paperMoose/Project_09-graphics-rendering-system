//
//  circle.c
//  
//
//  Created by Ryan Brandt on 9/22/14.
//
//

#include <stdio.h>
#include <stdlib.h>
#include "circle.h"
#include "point.h"
#include "line.h"

void circle_set(Circle *center, Point tc, double tr){
   /*initialize to center tc and radius tr.*/
	point_copy(&center->c, &tc);
	center->r = tr;
}

void circle_draw(Circle *c, Image *src, Color colr){
    /*draw the circle into src using color p.*/
  double radius = c->r;
  int x = 0;
  int y = radius;

  int xCenter = (int)c->c.val[0];
  int yCenter = (int)c->c.val[1];
  int p = 1 - radius;

  /* Plot first set of points */
  circle_PlotPoints(src,xCenter, yCenter, x, y,colr);

  while (x < y) {
    x++;
    if (p < 0) 
      p += 2 * x + 1;
    else {
      y--;
      p += 2 * (x - y) + 1;
    }
    	circle_PlotPoints(src,xCenter, yCenter, x, y,colr);
	}
}

/*
Draws a dashed circle
*/
void circle_drawDash(Circle *c, Image *src, Color colr){
	/*draw the dashed circle into src using color p.*/
  double radius = c->r;
  int x = 0;
  int y = radius;

  int xCenter = (int)c->c.val[0];
  int yCenter = (int)c->c.val[1];
  int p = 1 - radius;

  /* Plot first set of points */
  circle_PlotPoints(src,xCenter, yCenter, x, y,colr);

  while (x < y) {
    x++;
    if (p < 0) 
      p += 2 * x + 1;
    else {
      y--;
      p += 2 * (x - y) + 1;
    }	
    	if(x%4 == 0){
    		circle_PlotPoints(src,xCenter, yCenter, x, y,colr);
		}
	}
}


void circle_drawFill(Circle *c, Image *src, Color colr){
/*draw a filled circle into src using color p.*/
  
	double radius = c->r;
	int x = 0;
	int y = radius;

	int xCenter = (int)c->c.val[0];
	int yCenter = (int)c->c.val[1];
	int p = 1 - radius;


	/* Plot first set of points */
	circle_PlotFillLines(src,xCenter, yCenter, x, y,colr);



	while (x < y) {
		x++;
		if (p < 0) 
		  p += 2 * x + 1;
		else {
		  y--;
		  p += 2 * (x - y) + 1;
		}
		circle_PlotFillLines(src,xCenter, yCenter, x, y,colr);
	}
}


/*
Auxilary function to be used with circle_draw to draw 8 points 
in different actants.
*/
void circle_PlotPoints(Image* src, int yCenter, int xCenter, int y, int x, Color c){

	Point p;
	point_set2D(&p,(double)(xCenter+x),(double)(yCenter+y));
	point_draw(&p,src,c);
	point_set2D(&p,(double)(xCenter-x),(double)(yCenter+y));
	point_draw(&p,src,c);
	point_set2D(&p,(double)(xCenter+x),(double)(yCenter-y));
	point_draw(&p,src,c);
	point_set2D(&p,(double)(xCenter-x),(double)(yCenter-y));
	point_draw(&p,src,c);
	point_set2D(&p,(double)(xCenter+y),(double)(yCenter+x));
	point_draw(&p,src,c);
	point_set2D(&p,(double)(xCenter-y),(double)(yCenter+x));
	point_draw(&p,src,c);
	point_set2D(&p,(double)(xCenter+y),(double)(yCenter-x));
	point_draw(&p,src,c);
	point_set2D(&p,(double)(xCenter-y),(double)(yCenter-x));
	point_draw(&p,src,c);
}

void circle_PlotFillLines(Image* src, int xCenter, int yCenter, int x, int y, Color colr){
	Line l;
	line_set2D(&l, (double)(xCenter+x),(double)(yCenter+y), (double)(xCenter-x), (double)(yCenter+y));
	line_draw(&l, src, colr);

	line_set2D(&l, (double)(xCenter+x),(double)(yCenter-y), (double)(xCenter-x), (double)(yCenter-y));
	line_draw(&l, src, colr);

	line_set2D(&l, (double)(xCenter+y),(double)(yCenter+x), (double)(xCenter-y), (double)(yCenter+x));
	line_draw(&l, src, colr);

	line_set2D(&l, (double)(xCenter+y),(double)(yCenter-x), (double)(xCenter-y), (double)(yCenter-x));
	line_draw(&l, src, colr);

}




