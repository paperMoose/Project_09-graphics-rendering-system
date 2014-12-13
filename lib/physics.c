/*
Ryan Brandt & Erfan Azad
Date: 24 Oct 2014
File: module.c
*/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "physics.h"
#include "module.h"
#include "drawState.h"
#include "line.h"
#include "point.h"
#include "polyline.h"
#include "polygon.h"
#include "circle.h"
#include "color.h"
#include "Image.h"
#include "view.h"


void physics_Animation(Module *mod,Point velocity, Point gravity,int numFrames,Matrix * VTM, Matrix *GTM,DrawState ds){
		Image *src;
		src = image_create(500,500);
		int frame;
		// printf("happening!" );
		for(frame=0;frame<numFrames;frame++) {
			char buffer[256];
			Module *Scene = module_create();
			// velocityZ = -velocity.val[2];
			module_translate(Scene,velocity.val[0]*(frame*0.01),velocity.val[1]*(frame*0.01),velocity.val[2]*(frame*0.01));

// terminal velocity
			// if (velocity.val[2]>-10)
			// {
				point_set3D(&velocity,velocity.val[0],velocity.val[1],velocity.val[2]-gravity.val[2]);
			// }


			// printf("vel2 %f \n", velocity.val[2]);

			module_module(Scene,mod);
			module_draw( Scene, VTM, GTM, &ds, src );

			sprintf(buffer, "physics-frame%03d.ppm", frame);
			image_write(src, buffer);
			image_reset(src);
	}
}