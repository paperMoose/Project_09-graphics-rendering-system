/*
Ryan Brandt & Erfan Azad
Date: 24 Oct 2014
File: module.c
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
#include "view.h"
#include "bezier.h"
#include "light.h"




//Allocate and return an initialized but empty Element.
Element *element_create(){
	Element *e = malloc(sizeof(Element));
	e->next = NULL;
	return e;
}

//Allocate an Element and store a duplicate of the data pointed to by obj in the Element. Modules do
//not get duplicated. The function needs to handle each type of object separately in a case statement.
Element *element_init(ObjectType type, void *obj){
	Element *e = malloc(sizeof(Element));
	e->type = type;
	e->next = NULL;
	switch (type){
		case ObjNone:
			break;
		case ObjLine:
			line_copy(&e->obj.line,(Line *)obj);
			break;
		case ObjPoint:
			point_copy(&e->obj.point,(Point *)obj);
			break;
		case ObjPolyline:
			polyline_init(&e->obj.polyline);
			polyline_copy(&e->obj.polyline, (Polyline *)obj);
			break;
		case ObjPolygon:
			polygon_init(&e->obj.polygon);
			polygon_copy(&e->obj.polygon,(Polygon *)obj);
			break;
		case ObjIdentity:
			break;
		case ObjMatrix:
			matrix_copy(&e->obj.matrix, (Matrix *)obj);
			break;
		case ObjColor:
		case ObjBodyColor:
		case ObjSurfaceColor:
			Color_copy(&e->obj.color, (Color *)obj);
			break;
		case ObjSurfaceCoeff:
			e->obj.coeff = *(float*)obj;
			break;
		case ObjLight:
			break;
		case ObjModule:
			e->obj.module = (Module *) obj;
			break;
		default:
			e->type = ObjNone;
			break;
	}
	return e;
}

//free the element and the object it contains, as appropriate.
void element_delete(Element *e){
	switch (e->type){
		case ObjPolyline:
			polyline_free(&(e->obj.polyline));
			break;
		case ObjPolygon:
			polygon_free(&(e->obj.polygon));
			break;
		case ObjModule:
			free(&(e->obj));
			break;
		default:
			break;
	}
	free(e);
}

//Allocate an empty module
Module* module_create(void){
	Module* mod = malloc(sizeof(Module));
	mod->head = NULL;
	mod->tail = NULL;
	return mod;
}

// clear the module’s list of Elements, freeing memory as appropriate.
void module_clear(Module *md){
	Element *iterator;
	for(iterator = (md->head); iterator != NULL; iterator = (Element*)iterator->next){
		switch (iterator->type){
			case ObjPolyline:
				polyline_clear(&(iterator->obj.polyline));
				break;
			case ObjPolygon:
				polygon_clear(&(iterator->obj.polygon));
				break;
			default:
				break;
		}
	}
	md->head = NULL;
	md->tail = NULL;
}

// Free all of the memory associated with a module, including the memory pointed to by md
void module_delete(Module *md){
	Element *iterator;
	for(iterator = (md->head); iterator != NULL; iterator = (Element*)iterator->next){
		switch (iterator->type){
			case ObjPolyline:
				polyline_clear(&(iterator->obj.polyline));
				break;
			case ObjPolygon:
				polygon_clear(&(iterator->obj.polygon));
				break;
			default:
				break;
		}
	}
	md->head = NULL;
	md->tail = NULL;
	free(md);
}

// Generic insert of an element into the module at the tail of the list.
void module_insert(Module *md, Element *e){
	if(NULL != md && NULL != e){
		if (md->head == NULL)
		{
			md->head = e;
			md->tail = e;
		}else{
			md->tail->next = e;
			md->tail = e;
		}
	}
}

// Adds a pointer to the Module sub to the tail of the module’s list.
void module_module(Module* md, Module* sub){
	if(NULL != md && NULL != sub){
		Element* e = element_init(ObjModule, sub);
		if (md->head == NULL)
		{
			md->head = e;
			md->tail = e;
		}else{
			md->tail->next = e;
			md->tail = e;
		}
	}
}

// Adds p to the tail of the module’s list.
void module_point(Module *md, Point *p){
	if(NULL != md && NULL != p){
		Element* e = element_init(ObjPoint, p);
		if (md->head == NULL)
		{
			md->head = e;
			md->tail = e;
		}else{
			md->tail->next = e;
			md->tail = e;
		}
	}
}

// Adds p to the tail of the module’s list.
void module_line(Module *md, Line *p){
	if(NULL != md && NULL != p){

		Element* e = element_init(ObjLine, p);
		if (md->head == NULL)
		{
			md->head = e;
			md->tail = e;
		}else{	
			md->tail->next = e;
			md->tail = e;
		}
	}	
}

// Adds p to the tail of the module’s list.
void module_polyline(Module *md, Polyline *p){
	if(NULL != md && NULL != p){
		Element* e = element_init(ObjPolyline, p);
		if (md->head == NULL)
		{
			md->head = e;
			md->tail = e;
		}else{	
			md->tail->next = e;
			md->tail = e;
		}
	}
}

// Adds p to the tail of the module’s list.
void module_polygon(Module *md, Polygon *p){
	if(NULL != md && NULL != p){
		Element* e = element_init(ObjPolygon, p);
		if (md->head == NULL)
		{
			md->head = e;
			md->tail = e;
		}else{
			md->tail->next = e;
			md->tail = e;
		}
	}
}

/*
use the de Casteljau algorithm to subdivide the Bezier curve divisions times, then add the lines
connecting the control points to the module. For example, if divisions is 1, the four original Bezier
curve control points will be used to generate eight control points and two new Bezier curves. Then
the algorithm will add six lines to the module, three for each of the smaller Bezier curves.
*/
void module_bezierCurve(Module *m, BezierCurve *b, int divisions){
	if(divisions == 0){
		Line l[3];
		int i;
		for(i=0; i<3; i++){
			line_set(&l[i], b->ctrl[i], b->ctrl[i+1]);
			module_line(m, &l[i]);
		}
	}else{
		BezierCurve curve1;
		BezierCurve curve2;
		de_Casteljau_module(b, &curve1, &curve2);

		module_bezierCurve(m, &curve1, divisions-1);
		module_bezierCurve(m, &curve2, divisions-1);
	}
}	



/*
use the de Casteljau algorithm to subdivide the Bezier surface divisions times, then draw either the
lines connecting the control points, if solid is 0, or draw triangles using the four corner control points.
For example, if divisions is 1, the 16 original Bezier curve control points will be used to generate 64
control points and four new Bezier surfaces, 1 level of subdivision, and then the algorithm will draw
lines or triangles to connect adjacent control points.
*/
void module_bezierSurface(Module *m, BezierSurface *b, int divisions, int solid){
	if(solid == 0){//framewire
		if(divisions == 0){//addlines to module
			Line l[3];
			int i,j;

			for(i=0; i<4; i++){ // draw horizontal
				for(j=0; j<3; j++){
					line_set(&l[j], b->ctrl[i][j], b->ctrl[i][j+1]);
					module_line(m, &l[j]);
				}
			}

			for(j=0; j<4; j++){ // draw vertical
				for(i=0; i<3; i++){
					line_set(&l[i], b->ctrl[i][j], b->ctrl[i+1][j]);
					module_line(m, &l[i]);
				}
			}
		}else{ //subdevide lines
			//Lets break the points into horizintal and vertical and use the de_Casteljau_surface algo.
			//for each row of 4 points we will recieve a row of 7 points
			BezierSurface surface[4];// 4 funal surfaces of 16 points
			int i, j;
			Point rows[4][7]; //for rows of 7 points (will be filles with the result of DC algo. on the original rows of b)
			Point final[7][7]; // will contain 49 points that are then given to the 4 sub surfaces.
			Point tempRow[4]; //temporary row for passing points between the original b, and the rows.
			
			for(i=0; i<4; i++){ // horizontal
				for(j=0; j<4; j++){
					point_copy(&tempRow[j], &b->ctrl[i][j]);
				}
				de_Casteljau_point(&tempRow[0] ,&rows[i][0]);
			}

			for(i=0; i<7; i++){ // vertical
				for(j=0; j<4; j++){
					point_copy(&tempRow[j], &rows[j][i]);
				}
				de_Casteljau_point(&tempRow[0], &final[i][0]);
			}

			for(i=0; i<4; i++){//top left surface[0]
				for(j=0; j<4; j++){
					point_copy(&surface[0].ctrl[i][j], &final[i][j]); 
				}
			}

			for(i=3; i<7; i++){//top right surface[1]
				for(j=0; j<4; j++){
					point_copy(&surface[1].ctrl[i-3][j], &final[i][j]);
				}
			}

			for(i=0; i<4; i++){//bottom left surface[2]
				for(j=3; j<7; j++){
					point_copy(&surface[2].ctrl[i][j-3], &final[i][j]);
				}
			}

			for(i=3; i<7; i++){//lower right surface[0]
				for(j=3; j<7; j++){
					point_copy(&surface[3].ctrl[i-3][j-3], &final[i][j]);
				}
			}

			module_bezierSurface(m, &surface[0], divisions-1, solid);
			module_bezierSurface(m, &surface[1], divisions-1, solid);
			module_bezierSurface(m, &surface[2], divisions-1, solid);
			module_bezierSurface(m, &surface[3], divisions-1, solid);
		}
	}else{//drawusing triangles
		if(divisions == 0){//addlines to module
			Polygon poly[2];
			Point ptemp1[3];
			Point ptemp2[3];
			int i,j;

			for(i=0; i<3; i++){ // draw horizontal
				for(j=0; j<3; j++){
					polygon_init(&poly[0]);
					polygon_init(&poly[1]);

					point_copy(&ptemp1[0], &b->ctrl[i][j]);//top left corner
					point_copy(&ptemp1[1], &b->ctrl[i][j+1]);//top right corner
					point_copy(&ptemp1[2], &b->ctrl[i+1][j]);//bottom left corner

					point_copy(&ptemp2[0], &b->ctrl[i][j+1]);//top right corner
					point_copy(&ptemp2[1], &b->ctrl[i+1][j+1]);//bottom right corner
					point_copy(&ptemp2[2], &b->ctrl[i+1][j]);//bottom left corner

					polygon_set(&poly[0], 3, ptemp1);
					polygon_set(&poly[1], 3, ptemp2);

					module_polygon(m, &poly[0]);
					module_polygon(m, &poly[1]);
				}
			}

		}else{ //subdevide lines
			//Lets break the points into horizintal and vertical and use the de_Casteljau_surface algo.
			//for each row of 4 points we will recieve a row of 7 points
			BezierSurface surface[4];// 4 funal surfaces of 16 points
			int i, j;
			Point rows[4][7]; //for rows of 7 points (will be filles with the result of DC algo. on the original rows of b)
			Point final[7][7]; // will contain 49 points that are then given to the 4 sub surfaces.
			Point tempRow[4]; //temporary row for passing points between the original b, and the rows.
			
			for(i=0; i<4; i++){ // horizontal
				for(j=0; j<4; j++){
					point_copy(&tempRow[j], &b->ctrl[i][j]);
				}
				de_Casteljau_point(&tempRow[0] ,&rows[i][0]);
			}

			for(i=0; i<7; i++){ // vertical
				for(j=0; j<4; j++){
					point_copy(&tempRow[j], &rows[j][i]);
				}
				de_Casteljau_point(&tempRow[0], &final[i][0]);
			}

			for(i=0; i<4; i++){//top left surface[0]
				for(j=0; j<4; j++){
					point_copy(&surface[0].ctrl[i][j], &final[i][j]); 
				}
			}

			for(i=3; i<7; i++){//top right surface[1]
				for(j=0; j<4; j++){
					point_copy(&surface[1].ctrl[i-3][j], &final[i][j]);
				}
			}

			for(i=0; i<4; i++){//bottom left surface[2]
				for(j=3; j<7; j++){
					point_copy(&surface[2].ctrl[i][j-3], &final[i][j]);
				}
			}

			for(i=3; i<7; i++){//lower right surface[0]
				for(j=3; j<7; j++){
					point_copy(&surface[3].ctrl[i-3][j-3], &final[i][j]);
				}
			}

			module_bezierSurface(m, &surface[0], divisions-1, solid);
			module_bezierSurface(m, &surface[1], divisions-1, solid);
			module_bezierSurface(m, &surface[2], divisions-1, solid);
			module_bezierSurface(m, &surface[3], divisions-1, solid);
		}

	}
}

//This function takes 4 points run the D.C. algo. on them
//then spits out 7 points which are basically the points for the 
//two subcurves. 
void de_Casteljau_point(Point *p, Point* row){
	Point temp[6];
	average(&p[0],&p[1],&temp[0]);
	average(&p[1],&p[2],&temp[1]);
	average(&p[2],&p[3],&temp[2]);

	average(&temp[0],&temp[1],&temp[3]);
	average(&temp[1],&temp[2],&temp[4]);

	average(&temp[3],&temp[4], &temp[5]); //point on the curve

	point_copy(&row[0], &p[0]);
	point_copy(&row[1], &temp[0]);	
	point_copy(&row[2], &temp[3]);
	point_copy(&row[3], &temp[5]);
	point_copy(&row[4], &temp[4]);
	point_copy(&row[5], &temp[2]);
	point_copy(&row[6], &p[3]);
}

// Object that sets the current transform to the identity, placed at the tail of the module’s list.
void module_identity(Module *md){
	Element* e  = element_create();
	e->type = ObjIdentity;
	if (md->head == NULL)
	{
		md->head = e;
		md->tail = e;
	}
	else{
		md->tail->next = e;
		md->tail = e;
	}
}

// Matrix operand to add a translation matrix to the tail of the module’s list.
void module_translate2D(Module *md, double tx, double ty){
	if(NULL != md){
		Matrix mat;
		matrix_identity(&mat);
		matrix_translate2D(&mat, tx, ty);
		Element* e = element_init(ObjMatrix, &mat);
		if (md->head == NULL)
		{
			md->head = e;
			md->tail = e;
		}
		else{
			md->tail->next = e;
			md->tail = e;
		}
	}
}

// Matrix operand to add a scale matrix to the tail of the module’s list.
void module_scale2D(Module *md, double sx, double sy){
	if(NULL != md){
		Matrix mat;
		matrix_identity(&mat);
		matrix_scale2D(&mat, sx, sy);
		Element* e = element_init(ObjMatrix, &mat);
		if (md->head == NULL)
		{
			md->head = e;
			md->tail = e;
		}
		else{
			md->tail->next = e;
			md->tail = e;
		}
	}
}

// Matrix operand to add a rotation about the Z axis to the tail of the module’s list.
void module_rotateZ(Module *md, double cth, double sth){
	if(NULL != md){
		Matrix mat;
		matrix_identity(&mat);
		matrix_rotateZ(&mat, cth, sth);
		Element* e = element_init(ObjMatrix, &mat);
		if (md->head == NULL)
		{
			md->head = e;
			md->tail = e;
		}else{
			md->tail->next = e;
			md->tail = e;
		}
	}
}

// Matrix operand to add a 2D shear matrix to the tail of the module’s list.
void module_shear2D(Module *md, double shx, double shy){
	if(NULL != md){
		Matrix mat;
		matrix_identity(&mat);
		matrix_shear2D(&mat, shx, shy);
		Element* e = element_init(ObjMatrix, &mat);
		if (md->head == NULL)
		{
			md->head = e;
			md->tail = e;
		}else{
			md->tail->next = e;
			md->tail = e;
		}
	}
}

//Draw the module into the image using the given view transformation matrix [VTM], Lighting and
//DrawState by traversing the list of Elements. (For now, Lighting can be an empty structure.)
void module_draw(Module *md, Matrix *VTM, Matrix *GTM, DrawState *ds, /*Vector* vpn,*/Lighting *lighting, Image *src){
	Matrix LTM;
	Matrix tempGTM;
	DrawState tempDraw;
	Element *iterator;
	Point tempPoint;
	Line tempLine;
	Polygon tempPolygon;
	Polyline tempPolyline;

	matrix_identity(&LTM);
	polygon_init(&tempPolygon);
	polyline_init(&tempPolyline);
	// printf("LTM _ begining\n");
	// matrix_print(&LTM, stdout);

	for(iterator = md->head; iterator != NULL; iterator = (Element*)iterator->next){
		switch(iterator->type){
			case ObjColor:
				ds->color = iterator->obj.color ;
				break;
			case ObjBodyColor:
				ds->body = iterator->obj.color ;
				break;
			case ObjSurfaceColor:
				ds->surface = iterator->obj.color ;
				break;
			case ObjPoint:
				printf("objPoint\n");
				point_copy(&tempPoint,&iterator->obj.point);
				matrix_xformPoint(&LTM,&tempPoint,&tempPoint);
				matrix_xformPoint(GTM,&tempPoint,&tempPoint);
				matrix_xformPoint(VTM,&tempPoint,&tempPoint);
				point_normalize(&tempPoint);
				printf("src (rows, cols): (%d, %d)\n",src->rows, src->cols);
				// point_print(&tempPoint, stdout);
				point_draw(&tempPoint,src,ds->color);
				break;
	 		case ObjLine:
				printf("objline\n");
				line_copy(&tempLine,&iterator->obj.line);
				matrix_xformLine(&LTM,&tempLine);
				matrix_xformLine(GTM,&tempLine);
				matrix_xformLine(VTM,&tempLine);

				line_normalize(&tempLine);
				line_draw(&tempLine,src,ds->color);
				break;
			case ObjPolyline:
				printf("objPolyline\n");
				polyline_copy(&tempPolyline,&iterator->obj.polyline);
				matrix_xformPolyline(&LTM,&tempPolyline);
				matrix_xformPolyline(GTM,&tempPolyline);
				matrix_xformPolyline(VTM,&tempPolyline);
				polyline_normalize(&tempPolyline);
				polyline_draw(&tempPolyline,src,ds->color);
				polyline_clear(&tempPolyline);
				break;
			case ObjPolygon:
				printf("objPolygon\n");
				polygon_copy(&tempPolygon,&iterator->obj.polygon);
				// printf("1\n");
				matrix_xformPolygon(&LTM,&tempPolygon);
				// printf("2\n");
				matrix_xformPolygon(GTM,&tempPolygon);
				// printf("3\n");
				if(ds->shade == ShadeGouraud){
					// printf("poly> g > before\n");
					polygon_shade(&tempPolygon,lighting, ds);
					// printf("poly> g > after\n");
				}
				// copying the world coordinates, mallocing space
				if(ds->shade == ShadePhong){
					int i;
					if(tempPolygon.vertexWorld != NULL){
						free(tempPolygon.vertexWorld);
					}
					if(tempPolygon.normalWorld != NULL){
						free(tempPolygon.normalWorld);
					}
					// printf("phong");
					tempPolygon.phong = 1;
					tempPolygon.vertexWorld = malloc(sizeof(Point)*tempPolygon.numVertex);
					tempPolygon.normalWorld = malloc(sizeof(Vector)*tempPolygon.numVertex);
					for(i = 0; i< tempPolygon.numVertex; i++){
						point_copy(&tempPolygon.vertexWorld[i],&tempPolygon.vertex[i]);
						vector_copy(&tempPolygon.normalWorld[i],&tempPolygon.normal[i]);
						point_print(&tempPolygon.normalWorld[i],stdout);
					}
				}

				matrix_xformPolygon(VTM,&tempPolygon);
				polygon_normalize(&tempPolygon);

				if (ds->shade == ShadeFrame)
				{
					polygon_draw(&tempPolygon, ds, src);
				}
				else if(ds->shade == ShadeFlat || ds->shade == ShadeDepth){
				//	Vector c;
				//	polygon_normal(&tempPolygon, &c);
				//	if(!is_surface_visible(vpn, &c)){
				polygon_drawFill(&tempPolygon, ds,src,NULL);
				//	}
				}
				else if(ds->shade == ShadeGouraud || ds->shade == ShadePhong){
					polygon_drawShade(&tempPolygon, src, ds,lighting);
				}
				polygon_clear(&tempPolygon);
				break;
			case ObjMatrix:
				printf("objMatrix\n");
				matrix_multiply(&(iterator->obj.matrix), &LTM, &LTM);
				break;
			case ObjIdentity:
				printf("objIdentity\n");
				matrix_identity(&LTM);
				break;
			case ObjLight:
				break;
			case ObjModule:
				printf("objModule\n");
				matrix_multiply(GTM, &LTM, &tempGTM);
				tempDraw = *ds;
				module_draw(iterator->obj.module, VTM, &tempGTM, &tempDraw, /*vpn,*/ lighting, src); 
				break;
			default:
				break;
		}
	}
}


// //3D functions

// // Matrix operand to add a 3D translation to the Module.
void module_translate(Module *md, double tx, double ty, double tz){
	if(NULL != md){
		Matrix mat;
		matrix_identity(&mat);
		matrix_translate(&mat, tx, ty, tz);
		Element* e = element_init(ObjMatrix, &mat);
		if (md->head == NULL)
		{
			md->head = e;
			md->tail = e;
		}
		else{
			md->tail->next = e;
			md->tail = e;
		}
	}
}

// // Matrix operand to add a 3D scale to the Module.
void module_scale(Module *md, double sx, double sy, double sz){
	if(NULL != md){
		Matrix mat;
		matrix_identity(&mat);
		matrix_scale(&mat, sx, sy, sz);
		Element* e = element_init(ObjMatrix, &mat);
		if (md->head == NULL)
		{
			md->head = e;
			md->tail = e;
		}
		else{
			md->tail->next = e;
			md->tail = e;
		}
	}
}

// // Matrix operand to add a rotation about the X-axis to the Module.
void module_rotateX(Module *md, double cth, double sth){
	if(NULL != md){
		Matrix mat;
		matrix_identity(&mat);
		matrix_rotateX(&mat, cth, sth);
		Element* e = element_init(ObjMatrix, &mat);
		if (md->head == NULL)
		{
			md->head = e;
			md->tail = e;
		}else{
			md->tail->next = e;
			md->tail = e;
		}
	}
}

// // Matrix operand to add a rotation about the Y-axis to the Module.
void module_rotateY(Module *md, double cth, double sth){
	if(NULL != md){
		Matrix mat;
		matrix_identity(&mat);
		matrix_rotateY(&mat, cth, sth);
		Element* e = element_init(ObjMatrix, &mat);
		if (md->head == NULL)
		{
			md->head = e;
			md->tail = e;
		}else{
			md->tail->next = e;
			md->tail = e;
		}
	}
}

// //Matrix operand to add a rotation that orients to the orthonormal axes ~u, ~v, w~
void module_rotateXYZ(Module *md, Vector *u, Vector *v, Vector *w){
	if((NULL != md) && (NULL != u) && (NULL != v) && (NULL != w)){
		Matrix mat;
		matrix_identity(&mat);
		matrix_rotateXYZ(&mat, u,v,w);
		Element* e = element_init(ObjMatrix, &mat);
		if (md->head == NULL)
		{
			md->head = e;
			md->tail = e;
		}else{
			md->tail->next = e;
			md->tail = e;
		}
	}
}




// //shading/color module functions

// // Adds the foreground color value to the tail of the module’s list.
void module_color(Module *md, Color *c){
	if((NULL != md) && (NULL != c)){
		Element* e = element_init(ObjColor, c);
		if (md->head == NULL)
		{
			md->head = e;
			md->tail = e;
		}else{
			md->tail->next = e;
			md->tail = e;
		}
	}
}

//Adds the surface color  value to the tail of the module’s list.
void module_surfaceColor(Module *md, Color *c){
	if((NULL != md) && (NULL != c)){
		Element* e = element_init(ObjSurfaceColor, c);
		if (md->head == NULL)
		{
			md->head = e;
			md->tail = e;
		}else{
			md->tail->next = e;
			md->tail = e;
		}
	}
}

//Adds the body color  value to the tail of the module’s list.
void module_bodyColor(Module *md, Color *c){
	if((NULL != md) && (NULL != c)){
		Element* e = element_init(ObjBodyColor, c);
		if (md->head == NULL)
		{
			md->head = e;
			md->tail = e;
		}else{
			md->tail->next = e;
			md->tail = e;
		}
	}
}

//Adds the surface coefficient value to the tail of the module’s list.
void module_surfaceCoeff(Module *md, float* f){
	if((NULL != md) && (NULL != f)){
		Element* e = element_init(ObjSurfaceCoeff, f);
		if (md->head == NULL)
		{
			md->head = e;
			md->tail = e;
		}else{
			md->tail->next = e;
			md->tail = e;
		}
	}
}



//shapes 

// //Adds a unit cube, axis-aligned and centered on zero to the Module. If solid is zero, add only lines.
// //If solid is non-zero, use polygons. Make sure each polygon has surface normals defined for it.
// void module_cube(Module *md, int solid){
// 	if(NULL != md){
// 		Point p[8];
// 		point_set3D( &p[0], -1, -1, -1 );
//   		point_set3D( &p[1],  1, -1, -1 );
//   		point_set3D( &p[2],  1,  1, -1 );
// 		point_set3D( &p[3], -1,  1, -1 );
// 		point_set3D( &p[4], -1, -1,  1 );
// 		point_set3D( &p[5],  1, -1,  1 );
// 		point_set3D( &p[6],  1,  1,  1 );
// 		point_set3D( &p[7], -1,  1,  1 );
// 		if(solid == 0){//use lines
// 			Line l[12];
// 			line_set(&l[0], p[0], p[1]);
// 			line_set(&l[1], p[1], p[2]);
// 			line_set(&l[2], p[2], p[3]);
// 			line_set(&l[3], p[3], p[0]);
// 			line_set(&l[4], p[0], p[4]);
// 			line_set(&l[5], p[4], p[7]);
// 			line_set(&l[6], p[7], p[3]);
// 			line_set(&l[7], p[5], p[1]);
// 			line_set(&l[8], p[2], p[6]);
// 			line_set(&l[9], p[6], p[7]);
// 			line_set(&l[10], p[6], p[5]);
// 			line_set(&l[11], p[4], p[5]);
// 			int i;
// 			for (i = 0; i < 12; i++)
// 			{
// 				module_line(md,&l[i]);
// 			}			
// 		}else{//use polygons
// 			Polygon pol[6];
// 			int i;
// 			Point temp[4];
// 			for (i = 0; i < 6; i++)
// 			{
// 				polygon_init(&pol[i]);
// 			}
// 			point_copy(&temp[0],&p[7]);
// 			point_copy(&temp[1],&p[6]);
// 			point_copy(&temp[2],&p[2]);
// 			point_copy(&temp[3],&p[3]);

// 			polygon_set(&pol[0],4,&temp[0]);

// 			point_copy(&temp[0],&p[6]);
// 			point_copy(&temp[1],&p[5]);
// 			point_copy(&temp[2],&p[1]);
// 			point_copy(&temp[3],&p[2]);

// 			polygon_set(&pol[1],4,&temp[0]);


// 			point_copy(&temp[0],&p[4]);
// 			point_copy(&temp[1],&p[0]);
// 			point_copy(&temp[2],&p[1]);
// 			point_copy(&temp[3],&p[5]);

// 			polygon_set(&pol[2],4,&temp[0]);


// 			point_copy(&temp[0],&p[7]);
// 			point_copy(&temp[1],&p[3]);
// 			point_copy(&temp[2],&p[0]);
// 			point_copy(&temp[3],&p[4]);

// 			polygon_set(&pol[3],4,&temp[0]);


// 			point_copy(&temp[0],&p[4]);
// 			point_copy(&temp[1],&p[5]);
// 			point_copy(&temp[2],&p[6]);
// 			point_copy(&temp[3],&p[7]);

// 			polygon_set(&pol[4],4,&temp[0]);


// 			point_copy(&temp[0],&p[2]);
// 			point_copy(&temp[1],&p[1]);
// 			point_copy(&temp[2],&p[0]);
// 			point_copy(&temp[3],&p[3]);

// 			polygon_set(&pol[5],4,&temp[0]);

// 			for (i = 0; i < 6; i++)
// 			{
// 				module_polygon(md,&pol[i]);
// 			}	
// 		}
// 	}

// }

void module_cube( Module *mod ) {
  Point pt[4];
  Polygon p;
  Vector n[4];
  int i;

  polygon_init( &p );
  point_set3D( &pt[0], -1, -1, -1 );
  point_set3D( &pt[1], -1, -1,  1 );
  point_set3D( &pt[2], -1,  1,  1 );
  point_set3D( &pt[3], -1,  1, -1 );
  polygon_set( &p, 4, pt );
  for(i=0;i<4;i++)
	  vector_set( &(n[i]), -1, 0, 0 );
  polygon_setNormals( &p, 4, n );
  polygon_setSided(&p,1);
  module_polygon( mod, &p );

  point_set3D( &pt[0], 1, -1, -1 );
  point_set3D( &pt[1], 1, -1,  1 );
  point_set3D( &pt[2], 1,  1,  1 );
  point_set3D( &pt[3], 1,  1, -1 );
  polygon_set( &p, 4, pt );
  for(i=0;i<4;i++)
	  vector_set( &(n[i]), 1, 0, 0 );
  polygon_setNormals( &p, 4, n );
  polygon_setSided(&p,1);
  module_polygon( mod, &p );

  point_set3D( &pt[0], -1, -1, -1 );
  point_set3D( &pt[1], -1, -1,  1 );
  point_set3D( &pt[2],  1, -1,  1 );
  point_set3D( &pt[3],  1, -1, -1 );
  polygon_set( &p, 4, pt );
  for(i=0;i<4;i++)
	  vector_set( &(n[i]), 0, -1, 0 );
  polygon_setNormals( &p, 4, n );
  polygon_setSided(&p,1);
  module_polygon( mod, &p );

  point_set3D( &pt[0], -1, 1, -1 );
  point_set3D( &pt[1], -1, 1,  1 );
  point_set3D( &pt[2],  1, 1,  1 );
  point_set3D( &pt[3],  1, 1, -1 );
  polygon_set( &p, 4, pt );
  for(i=0;i<4;i++)
	  vector_set( &(n[i]), 0, 1, 0 );
  polygon_setNormals( &p, 4, n );
  polygon_setSided(&p,1);
  module_polygon( mod, &p );

  point_set3D( &pt[0], -1, -1, -1 );
  point_set3D( &pt[1], -1,  1, -1 );
  point_set3D( &pt[2],  1,  1, -1 );
  point_set3D( &pt[3],  1, -1, -1 );
  polygon_set( &p, 4, pt );
  for(i=0;i<4;i++)
	  vector_set( &(n[i]), 0, 0, -1 );
  polygon_setNormals( &p, 4, n );
  polygon_setSided(&p,1);
  module_polygon( mod, &p );

  point_set3D( &pt[0], -1, -1, 1 );
  point_set3D( &pt[1], -1,  1, 1 );
  point_set3D( &pt[2],  1,  1, 1 );
  point_set3D( &pt[3],  1, -1, 1 );
  polygon_set( &p, 4, pt );
  for(i=0;i<4;i++)
	  vector_set( &(n[i]), 0, 0, 1 );
  polygon_setNormals( &p, 4, n );
  polygon_setSided(&p,1);
  module_polygon( mod, &p );

}

//draws a unit circle center at x=0, z=0, flat on x-z plane using "sides" number of lines.
//it uses lines so it CANNOT be filled.
void module_circleFrame( Module *mod, int sides ) {
  Line l;
  Point xCenter;
  double x1, x2, z1, z2;
  int i;
  point_set3D( &xCenter, 0, 0.0, 0.0 );

  // make a fan for the top and bottom sides
  // and quadrilaterals for the sides
  for(i=0;i<sides;i++) {
    Point pt[2];

    x1 = cos( i * M_PI * 2.0 / sides );
    z1 = sin( i * M_PI * 2.0 / sides );
    x2 = cos( ( (i+1)%sides ) * M_PI * 2.0 / sides );
    z2 = sin( ( (i+1)%sides ) * M_PI * 2.0 / sides );

    point_set3D( &pt[0], x1, 0.0, z1 );
    point_set3D( &pt[1], x2, 0.0, z2 );
    line_set(&l, pt[0], pt[1]);
    module_line(mod, &l);
  }
}

//draws a unit circle center at x=0, z=0, flat on x-z plane using "sides" number of polygons.
//It uses polygons so it can be filled
void module_circle( Module *mod, int sides ) {
  Polygon p;
  Point xCenter;
  double x1, x2, z1, z2;
  int i;

  polygon_init( &p );
  point_set3D( &xCenter, 0, 0.0, 0.0 );

  // make a fan for the top and bottom sides
  // and quadrilaterals for the sides
  for(i=0;i<sides;i++) {
    Point pt[3];

    x1 = cos( i * M_PI * 2.0 / sides );
    z1 = sin( i * M_PI * 2.0 / sides );
    x2 = cos( ( (i+1)%sides ) * M_PI * 2.0 / sides );
    z2 = sin( ( (i+1)%sides ) * M_PI * 2.0 / sides );

    point_copy( &pt[0], &xCenter );
    point_set3D( &pt[1], x1, 0.0, z1 );
    point_set3D( &pt[2], x2, 0.0, z2 );

    polygon_set( &p, 3, pt );
    module_polygon( mod, &p );
  }
  polygon_clear( &p );
}

/*
Draws the frame of an ellipse using lines. it CANNOT be filled.
Parameters:
	mod : module to add the ellipse in.
	a, b: radiux of x and z axix respetively.
	sides: number of lines to use when drawing the polygon.
*/
void module_ellipseFrame(Module* mod, double a, double b, int sides){
	Line l;
	Point xCenter;
	double x1, x2, z1, z2;
	int i;
	point_set3D( &xCenter, 0, 0.0, 0.0 );

	// make a fan for the top and bottom sides
	// and quadrilaterals for the sides
	for(i=0;i<sides;i++) {
	Point pt[2];

	x1 = a * cos( i * M_PI * 2.0 / sides );
	z1 = b * sin( i * M_PI * 2.0 / sides );
	x2 = a * cos( ( (i+1)%sides ) * M_PI * 2.0 / sides );
	z2 = b * sin( ( (i+1)%sides ) * M_PI * 2.0 / sides );

	point_set3D( &pt[0], x1, 0.0, z1 );
	point_set3D( &pt[1], x2, 0.0, z2 );
	line_set(&l, pt[0], pt[1]);
	module_line(mod, &l);
  }

}

/*
Draws the frame of an ellipse using polygons. it CAN be filled.
Parameters:
	mod : module to add the ellipse in.
	a, b: radiux of x and z axix respetively.
	sides: number of polygons (triangles) to use when drawing the polygon.
*/
void module_ellipse(Module* mod, double a, double b, int sides){
	Polygon p;
	Point xCenter;
	double x1, x2, z1, z2;
	int i;

	polygon_init( &p );
	point_set3D( &xCenter, 0, 0.0, 0.0 );

	// make a fan for the top and bottom sides
	// and quadrilaterals for the sides
	for(i=0;i<sides;i++) {
	Point pt[3];

	x1 = a * cos( i * M_PI * 2.0 / sides );
	z1 = b * sin( i * M_PI * 2.0 / sides );
	x2 = a * cos( ( (i+1)%sides ) * M_PI * 2.0 / sides );
	z2 = b * sin( ( (i+1)%sides ) * M_PI * 2.0 / sides );

	point_copy( &pt[0], &xCenter );
	point_set3D( &pt[1], x1, 0.0, z1 );
	point_set3D( &pt[2], x2, 0.0, z2 );

	polygon_set( &p, 3, pt );
	module_polygon( mod, &p );
	}
	polygon_clear( &p );
}

void module_cylinder( Module *mod, int sides ) {
  Polygon p;
  Point xtop, xbot;
  double x1, x2, z1, z2;
  int i;

  polygon_init( &p );
  point_set3D( &xtop, 0, 1.0, 0.0 );
  point_set3D( &xbot, 0, 0.0, 0.0 );

  // make a fan for the top and bottom sides
  // and quadrilaterals for the sides
  for(i=0;i<sides;i++) {
    Point pt[4];

    x1 = cos( i * M_PI * 2.0 / sides );
    z1 = sin( i * M_PI * 2.0 / sides );
    x2 = cos( ( (i+1)%sides ) * M_PI * 2.0 / sides );
    z2 = sin( ( (i+1)%sides ) * M_PI * 2.0 / sides );

    point_copy( &pt[0], &xtop );
    point_set3D( &pt[1], x1, 1.0, z1 );
    point_set3D( &pt[2], x2, 1.0, z2 );

    polygon_set( &p, 3, pt );
    module_polygon( mod, &p );

    point_copy( &pt[0], &xbot );
    point_set3D( &pt[1], x1, 0.0, z1 );
    point_set3D( &pt[2], x2, 0.0, z2 );

    polygon_set( &p, 3, pt );
    module_polygon( mod, &p );

    point_set3D( &pt[0], x1, 0.0, z1 );
    point_set3D( &pt[1], x2, 0.0, z2 );
    point_set3D( &pt[2], x2, 1.0, z2 );
    point_set3D( &pt[3], x1, 1.0, z1 );
    
    polygon_set( &p, 4, pt );
    module_polygon( mod, &p );
  }

  polygon_clear( &p );
}

//draws a frame wire of a "sides" based unit pyramid
//int to the module "mod".
void module_pyramid(Module* mod, int sides){
	if((NULL != mod) && (sides >=3)){
		int i;	
		double x, z;
		Point top;
		Polygon *base;
		Point p[sides];
		Line l[sides];
		point_set3D(&top, 0, 1, 0);

		for(i=0; i<sides; i++){
			x = cos( i * M_PI *2.0 / sides );
    		z = sin( i * M_PI *2.0 / sides );
			point_set3D(&p[i], x ,0, z);
			line_set(&l[i],p[i], top);
			module_line(mod, &l[i]);
		}
		base = polygon_createp(sides, &p[0]);
		module_polygon(mod, base);
		polygon_clear(base);

	}
}

// a unit tetrahedron
void module_tetrahedron(Module *md){
	Point tet[6];
	Point temp[3];	
	Polygon pol[8];
	int i;
	point_set3D(&tet[0],0,0,0);
	point_set3D(&tet[1],1,0,0);
	point_set3D(&tet[2],1,0,1);
	point_set3D(&tet[3],0,0,1);
	point_set3D(&tet[4],.5,1,.5);
	point_set3D(&tet[5],.5,-1,.5);

	for (i = 0; i < 8; i++)
	{
		polygon_init(&pol[i]);
	}

	point_copy(&temp[0],&tet[0]);
	point_copy(&temp[1],&tet[1]);
	point_copy(&temp[2],&tet[4]);


	polygon_set(&pol[0],3,temp);

	point_copy(&temp[0],&tet[1]);
	point_copy(&temp[1],&tet[2]);
	point_copy(&temp[2],&tet[4]);


	polygon_set(&pol[1],3,temp);

	point_copy(&temp[0],&tet[2]);
	point_copy(&temp[1],&tet[3]);
	point_copy(&temp[2],&tet[4]);


	polygon_set(&pol[2],3,temp);

	point_copy(&temp[0],&tet[0]);
	point_copy(&temp[1],&tet[3]);
	point_copy(&temp[2],&tet[4]);


	polygon_set(&pol[3],3,temp);

	point_copy(&temp[0],&tet[0]);
	point_copy(&temp[1],&tet[1]);
	point_copy(&temp[2],&tet[5]);

	polygon_set(&pol[4],3,temp);

	point_copy(&temp[0],&tet[1]);
	point_copy(&temp[1],&tet[2]);
	point_copy(&temp[2],&tet[5]);

	polygon_set(&pol[5],3,temp);

	point_copy(&temp[0],&tet[2]);
	point_copy(&temp[1],&tet[3]);
	point_copy(&temp[2],&tet[5]);


	polygon_set(&pol[6],3,temp);

	point_copy(&temp[0],&tet[0]);
	point_copy(&temp[1],&tet[3]);
	point_copy(&temp[2],&tet[5]);


	polygon_set(&pol[7],3,temp);

	for (i = 0; i < 8; i++)
	{
		module_polygon(md,&pol[i]);
	}
}




