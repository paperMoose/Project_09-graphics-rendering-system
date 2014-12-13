/*
Erfan Azad & Ryan Brandt
Date: 16 Oct 2014
file: view.c
*/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "matrix.h"
#include "view.h"
#include "polygon.h"


void matrix_setView2D(Matrix *vtm, View2D *view){
	if(NULL != vtm && NULL != view){
		float dv = (view->dx*view->screeny)/view->screenx;
		matrix_identity(vtm);
 		 matrix_print(vtm, stdout);
		matrix_translate2D(vtm, -view->vrp.val[0], -view->vrp.val[1]);
		matrix_rotateZ(vtm, view->x.val[0], -view->x.val[1]);
		matrix_scale2D(vtm, view->screenx/view->dx, -view->screeny/dv);
		matrix_translate2D(vtm, view->screenx/2, view->screeny/2);
	}
}

void matrix_setView3D(Matrix *vtm, View3D *view){
	if(NULL != vtm && NULL != view){
		Vector u;
		Vector vup = view->vup;
		Vector vpn = view->vpn;
		Matrix project;
		double bPrime = view->d +view->b;
		double dPrime = view->d/bPrime;

		matrix_identity(vtm);
		printf("before everything:\n");
 		matrix_print(vtm, stdout);

		vector_cross(&vup,&vpn,&u);
		vector_cross(&vpn,&u,&vup);
		printf("vrp:\n");
		vector_print(&view->vrp,stdout);

		matrix_translate(vtm, -view->vrp.val[0], -view->vrp.val[1],-view->vrp.val[2]);
		printf("After VRP translation:\n");
 		matrix_print(vtm, stdout);

		vector_normalize(&u);
		vector_normalize(&vpn);
		vector_normalize(&vup);
		matrix_rotateXYZ(vtm, &u, &vup, &vpn );
		printf("After Rxyz :\n");
  		matrix_print(vtm, stdout);

		matrix_translate(vtm, 0, 0,view->d);
		printf("After translating COP to origin:\n");
  		matrix_print(vtm, stdout);
  		
		
		matrix_scale(vtm, (2*view->d)/(bPrime*view->du), (2*view->d)/(bPrime*view->dv), 1/bPrime);
		printf("After scaling to CVV:\n");
 		matrix_print(vtm, stdout);

		matrix_identity(&project);
		project.m[3][3]=0;
		project.m[3][2]=1/dPrime;
		printf("projection:\n");
		matrix_print(&project, stdout);
		matrix_multiply(&project,vtm,vtm);
		printf("After perspective:\n");
 		matrix_print(vtm, stdout);

		matrix_scale2D(vtm, -view->screenx/(2*dPrime), -view->screeny/(2*dPrime));
		printf("After scale to image coords:\n");
  		matrix_print(vtm, stdout);

		matrix_translate2D(vtm, view->screenx/2, view->screeny/2);
		printf("After final translation to image coords:\n");
  		matrix_print(vtm, stdout);
	}

}


/*
Tells you if a surface of a polygon is visible or not for drawing.
returns 1 if visible, 0 otherwise
*/
int is_surface_visible(Vector* vpn, Vector* surfaceNormal){
	double result = vector_dot(vpn, surfaceNormal);
	if( result > 0){
		return 1; //visible
	}
	else{
		return 0; // not visible
	}
}


void view_rotate_circle(Polygon* poly_vrp, Point* center, int sides, double scale, double thetax, double thetay, double thetaz){
	if(NULL != poly_vrp){
		//make a unit circle of "sides" number fo sides and store the points in the poly_vrp
		int i;
		double x, z;
		polygon_init(poly_vrp);
		Point p[sides];
		Matrix LTM;
		matrix_identity(&LTM);
		matrix_translate(&LTM, center->val[0], center->val[1], center->val[2]);
		matrix_rotateX(&LTM, cos(thetax*M_PI/180), sin(thetax*M_PI/180));
		matrix_rotateY(&LTM, cos(thetay*M_PI/180), sin(thetay*M_PI/180));
		matrix_rotateZ(&LTM, cos(thetaz*M_PI/180), sin(thetaz*M_PI/180));
		matrix_scale(&LTM, scale, scale, scale);
		for(i=0; i<sides; i++){
			x = cos( i * M_PI * 2.0 / sides );
    		z = sin( i * M_PI * 2.0 / sides );

    		point_set3D(&p[i], x, 0.1, z);
		}
		polygon_set(poly_vrp, sides, &p[0]);
		matrix_xformPolygon(&LTM, poly_vrp);
	}
}





