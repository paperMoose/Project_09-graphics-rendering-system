#include <stdio.h>
#include "point.h"
#include "color.h"

#ifndef __DRAWSTATE_H__
#define __DRAWSTATE_H__



typedef enum{
	ShadeFrame,  //- draw only the borders of objects, including polygons.
	ShadeConstant, //- draw objects using the current foreground color, fill polygons. 
	ShadeDepth, //- draw objects using their depth value.
	ShadeFlat,   //- draw objects using shading calculations, but each polygon is a constant value. 
	ShadeGouraud, //- draw objects using Gouraud shading.
	ShadePhong,
}ShadeMethod;


typedef struct{
	Color color;  //- the foreground color, used in the default drawing mode.
	Color flatColor; //- the color to flat-fill a polygon based on a shading calculation.
	Color body;  //- the body reflection color, used for shading calculations.
	Color surface;  // the surface reflection color, used for shading calculations.
	float surfaceCoeff; // a float that represents the shininess of the surface.
	ShadeMethod shade; // an enumerated type ShadeMethod.
	int zBufferFlag; // whether to use z-buffer hidden surface removal.
	Point viewer; // a Point representing the view location in 3D (identical to the VRP in View3D).
	float alpha;
}DrawState;


DrawState *drawstate_create( void );
void drawstate_setColor( DrawState *s, Color c );
void drawstate_setBody( DrawState *s, Color c );
void drawstate_setSurface( DrawState *s, Color c );
void drawstate_setSurfaceCoeff( DrawState *s, float f );
void drawstate_setAlpha( DrawState *s, float f );
void drawstate_copy( DrawState *to, DrawState *from );




#endif