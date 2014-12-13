/*
	Bruce A Maxwell
	Fall 2014
	CS 351

	Creates a pair of spinning Bezier surfaces

	Accepts one command line parameter, which is the number of
	subdivisions to execute.
*/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
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

int main(int argc, char *argv[]) {
	int frame;
	Color orange,white;
	Point p[118];
	Point rim[16];
	Point body1[16];
	Point body2[16];
	Point lid1[16];
	Point lid2[16];
	Point handle1[16];
	Point handle2[16];
	Point spout1[16];
	Point spout2[16];
	BezierSurface bc;
	DrawState ds;
	Module *curve;
	View3D view;
	Matrix VTM, GTM;
	int divisions = 4;
	int rows = 300, cols = 400;
	Image *src = image_create(rows, cols);

	// grab the command line argument, if one exists
	if(argc > 1) {
		int tmp = atoi(argv[1]);
		if( tmp >= 0 && tmp < 10 )
			divisions = tmp;
	}
	printf("Creating Bezier curves with %d subdivisions\n", divisions);

	Color_set(&white, 1.0, 1.0, 1.0 );
	Color_set(&orange, 1, 0.549, 0 );

	curve = module_create();

	// create a flat plane
point_set3D(&p[0],  0.2000,  0.0000, 2.70000 );
point_set3D(&p[1],  0.2000, -0.1120, 2.70000 );
point_set3D(&p[2],  0.1120, -0.2000, 2.70000 );
point_set3D(&p[3],  0.0000, -0.2000, 2.70000 );
point_set3D(&p[4],  1.3375,  0.0000, 2.53125 );
point_set3D(&p[5],  1.3375, -0.7490, 2.53125 );
point_set3D(&p[6],  0.7490, -1.3375, 2.53125 );
point_set3D(&p[7],  0.0000, -1.3375, 2.53125 );
point_set3D(&p[8],  1.4375,  0.0000, 2.53125 );
point_set3D(&p[9],  1.4375, -0.8050, 2.53125 );
point_set3D(&p[10],  0.8050, -1.4375, 2.53125 );
point_set3D(&p[11],  0.0000, -1.4375, 2.53125 );
point_set3D(&p[12],  1.5000,  0.0000, 2.40000 );
point_set3D(&p[13],  1.5000, -0.8400, 2.40000 );
point_set3D(&p[14],  0.8400, -1.5000, 2.40000 );
point_set3D(&p[15],  0.0000, -1.5000, 2.40000 );
point_set3D(&p[16],  1.7500,  0.0000, 1.87500 );
point_set3D(&p[17],  1.7500, -0.9800, 1.87500 );
point_set3D(&p[18],  0.9800, -1.7500, 1.87500 );
point_set3D(&p[19],  0.0000, -1.7500, 1.87500 );
point_set3D(&p[20],  2.0000,  0.0000, 1.35000 );
point_set3D(&p[21],  2.0000, -1.1200, 1.35000 );
point_set3D(&p[22],  1.1200, -2.0000, 1.35000 );
point_set3D(&p[23],  0.0000, -2.0000, 1.35000 );
point_set3D(&p[24],  2.0000,  0.0000, 0.90000 );
point_set3D(&p[25],  2.0000, -1.1200, 0.90000 );
point_set3D(&p[26],  1.1200, -2.0000, 0.90000 );
point_set3D(&p[27],  0.0000, -2.0000, 0.90000 );
point_set3D(&p[28], -2.0000,  0.0000, 0.90000 );
point_set3D(&p[29],  2.0000,  0.0000, 0.45000 );
point_set3D(&p[30],  2.0000, -1.1200, 0.45000 );
point_set3D(&p[31],  1.1200, -2.0000, 0.45000 );
point_set3D(&p[32],  0.0000, -2.0000, 0.45000 );
point_set3D(&p[33],  1.5000,  0.0000, 0.22500 );
point_set3D(&p[34],  1.5000, -0.8400, 0.22500 );
point_set3D(&p[35],  0.8400, -1.5000, 0.22500 );
point_set3D(&p[36],  0.0000, -1.5000, 0.22500 );
point_set3D(&p[37],  1.5000,  0.0000, 0.15000 );
point_set3D(&p[38],  1.5000, -0.8400, 0.15000 );
point_set3D(&p[39],  0.8400, -1.5000, 0.15000 );
point_set3D(&p[40],  0.0000, -1.5000, 0.15000 );
point_set3D(&p[41], -1.6000,  0.0000, 2.02500 );
point_set3D(&p[42], -1.6000, -0.3000, 2.02500 );
point_set3D(&p[43], -1.5000, -0.3000, 2.25000 );
point_set3D(&p[44], -1.5000,  0.0000, 2.25000 );
point_set3D(&p[45], -2.3000,  0.0000, 2.02500 );
point_set3D(&p[46], -2.3000, -0.3000, 2.02500 );
point_set3D(&p[47], -2.5000, -0.3000, 2.25000 );
point_set3D(&p[48], -2.5000,  0.0000, 2.25000 );
point_set3D(&p[49], -2.7000,  0.0000, 2.02500 );
point_set3D(&p[50], -2.7000, -0.3000, 2.02500 );
point_set3D(&p[51], -3.0000, -0.3000, 2.25000 );
point_set3D(&p[52], -3.0000,  0.0000, 2.25000 );
point_set3D(&p[53], -2.7000,  0.0000, 1.80000 );
point_set3D(&p[54], -2.7000, -0.3000, 1.80000 );
point_set3D(&p[55], -3.0000, -0.3000, 1.80000 );
point_set3D(&p[56], -3.0000,  0.0000, 1.80000 );
point_set3D(&p[57], -2.7000,  0.0000, 1.57500 );
point_set3D(&p[58], -2.7000, -0.3000, 1.57500 );
point_set3D(&p[59], -3.0000, -0.3000, 1.35000 );
point_set3D(&p[60], -3.0000,  0.0000, 1.35000 );
point_set3D(&p[61], -2.5000,  0.0000, 1.12500 );
point_set3D(&p[62], -2.5000, -0.3000, 1.12500 );
point_set3D(&p[63], -2.6500, -0.3000, 0.93750 );
point_set3D(&p[64], -2.6500,  0.0000, 0.93750 );
point_set3D(&p[65], -2.0000, -0.3000, 0.90000 );
point_set3D(&p[66], -1.9000, -0.3000, 0.60000 );
point_set3D(&p[67], -1.9000,  0.0000, 0.60000 );
point_set3D(&p[68],  1.7000,  0.0000, 1.42500 );
point_set3D(&p[69],  1.7000, -0.6600, 1.42500 );
point_set3D(&p[70],  1.7000, -0.6600, 0.60000 );
point_set3D(&p[71],  1.7000,  0.0000, 0.60000 );
point_set3D(&p[72],  2.6000,  0.0000, 1.42500 );
point_set3D(&p[73],  2.6000, -0.6600, 1.42500 );
point_set3D(&p[74],  3.1000, -0.6600, 0.82500 );
point_set3D(&p[75],  3.1000,  0.0000, 0.82500 );
point_set3D(&p[76],  2.3000,  0.0000, 2.10000 );
point_set3D(&p[77],  2.3000, -0.2500, 2.10000 );
point_set3D(&p[78],  2.4000, -0.2500, 2.02500 );
point_set3D(&p[79],  2.4000,  0.0000, 2.02500 );
point_set3D(&p[80],  2.7000,  0.0000, 2.40000 );
point_set3D(&p[81],  2.7000, -0.2500, 2.40000 );
point_set3D(&p[82],  3.3000, -0.2500, 2.40000 );
point_set3D(&p[83],  3.3000,  0.0000, 2.40000 );
point_set3D(&p[84],  2.8000,  0.0000, 2.47500 );
point_set3D(&p[85],  2.8000, -0.2500, 2.47500 );
point_set3D(&p[86],  3.5250, -0.2500, 2.49375 );
point_set3D(&p[87],  3.5250,  0.0000, 2.49375 );
point_set3D(&p[88],  2.9000,  0.0000, 2.47500 );
point_set3D(&p[89],  2.9000, -0.1500, 2.47500 );
point_set3D(&p[90],  3.4500, -0.1500, 2.51250 );
point_set3D(&p[91],  3.4500,  0.0000, 2.51250 );
point_set3D(&p[92],  2.8000,  0.0000, 2.40000 );
point_set3D(&p[93],  2.8000, -0.1500, 2.40000 );
point_set3D(&p[94],  3.2000, -0.1500, 2.40000 );
point_set3D(&p[95],  3.2000,  0.0000, 2.40000 );
point_set3D(&p[96],  0.0000,  0.0000, 3.15000 );
point_set3D(&p[97],  0.8000,  0.0000, 3.15000 );
point_set3D(&p[98],  0.8000, -0.4500, 3.15000 );
point_set3D(&p[99],  0.4500, -0.8000, 3.15000 );
point_set3D(&p[100],  0.0000, -0.8000, 3.15000 );
point_set3D(&p[101],  0.0000,  0.0000, 2.85000 );
point_set3D(&p[102],  1.4000,  0.0000, 2.40000 );
point_set3D(&p[103],  1.4000, -0.7840, 2.40000 );
point_set3D(&p[104],  0.7840, -1.4000, 2.40000 );
point_set3D(&p[105],  0.0000, -1.4000, 2.40000 );
point_set3D(&p[106],  0.4000,  0.0000, 2.55000 );
point_set3D(&p[107],  0.4000, -0.2240, 2.55000 );
point_set3D(&p[108],  0.2240, -0.4000, 2.55000 );
point_set3D(&p[109],  0.0000, -0.4000, 2.55000 );
point_set3D(&p[110],  1.3000,  0.0000, 2.55000 );
point_set3D(&p[111],  1.3000, -0.7280, 2.55000 );
point_set3D(&p[112],  0.7280, -1.3000, 2.55000 );
point_set3D(&p[113],  0.0000, -1.3000, 2.55000 );
point_set3D(&p[114],  1.3000,  0.0000, 2.40000 );
point_set3D(&p[115],  1.3000, -0.7280, 2.40000 );
point_set3D(&p[116],  0.7280, -1.3000, 2.40000 );
point_set3D(&p[117],  0.0000, -1.3000, 2.40000 );


// 102, 103, 104, 105,   4,   5,   6,   7,
//         8,   9,  10,  11,  12,  13,  14,  15

        point_copy(&rim[0],&p[102]);
        point_copy(&rim[1],&p[103]);
        point_copy(&rim[2],&p[104]);
        point_copy(&rim[3],&p[105]);
        point_copy(&rim[4],&p[4]);
        point_copy(&rim[5],&p[5]);
        point_copy(&rim[6],&p[6]);
        point_copy(&rim[7],&p[7]);
        point_copy(&rim[8],&p[8]);
        point_copy(&rim[9],&p[9]);
        point_copy(&rim[10],&p[10]);
        point_copy(&rim[11],&p[11]);
        point_copy(&rim[12],&p[12]);
        point_copy(&rim[13],&p[13]);
        point_copy(&rim[14],&p[14]);
        point_copy(&rim[15],&p[15]);


       //  12,  13,  14,  15,  16,  17,  18,  19,
       // 20,  21,  22,  23,  24,  25,  26,  27

		point_copy(&body1[0],&p[12]);
		point_copy(&body1[1],&p[13]);
		point_copy(&body1[2],&p[14]);
		point_copy(&body1[3],&p[15]);
		point_copy(&body1[4],&p[16]);
		point_copy(&body1[5],&p[17]);
		point_copy(&body1[6],&p[18]);
		point_copy(&body1[7],&p[19]);
		point_copy(&body1[8],&p[20]);
		point_copy(&body1[9],&p[21]);
		point_copy(&body1[10],&p[22]);
		point_copy(&body1[11],&p[23]);
		point_copy(&body1[12],&p[24]);
		point_copy(&body1[13],&p[25]);
		point_copy(&body1[14],&p[26]);
		point_copy(&body1[15],&p[27]);

       //   24,  25,  26,  27,  29,  30,  31,  32,
       // 33,  34,  35,  36,  37,  38,  39,  40

       	point_copy(&body2[0],&p[24]);
		point_copy(&body2[1],&p[25]);
		point_copy(&body2[2],&p[26]);
		point_copy(&body2[3],&p[27]);
		point_copy(&body2[4],&p[29]);
		point_copy(&body2[5],&p[30]);
		point_copy(&body2[6],&p[31]);
		point_copy(&body2[7],&p[32]);
		point_copy(&body2[8],&p[33]);
		point_copy(&body2[9],&p[34]);
		point_copy(&body2[10],&p[35]);
		point_copy(&body2[11],&p[36]);
		point_copy(&body2[12],&p[37]);
		point_copy(&body2[13],&p[38]);
		point_copy(&body2[14],&p[39]);
		point_copy(&body2[15],&p[40]);


        point_copy(&lid1[0],&p[96]);
		point_copy(&lid1[1],&p[96]);
		point_copy(&lid1[2],&p[96]);
		point_copy(&lid1[3],&p[96]);
		point_copy(&lid1[4],&p[97]);
		point_copy(&lid1[5],&p[98]);
		point_copy(&lid1[6],&p[99]);
		point_copy(&lid1[7],&p[100]);
		point_copy(&lid1[8],&p[101]);
		point_copy(&lid1[9],&p[101]);
		point_copy(&lid1[10],&p[101]);
		point_copy(&lid1[11],&p[101]);
		point_copy(&lid1[12],&p[0]);
		point_copy(&lid1[13],&p[1]);
		point_copy(&lid1[14],&p[2]);
		point_copy(&lid1[15],&p[3]);

    // {   0,   1,   2,   3, 106, 107, 108, 109,
    //   110, 111, 112, 113, 114, 115, 116, 117 }



		point_copy(&lid2[0],&p[0]);
		point_copy(&lid2[1],&p[1]);
		point_copy(&lid2[2],&p[2]);
		point_copy(&lid2[3],&p[3]);
		point_copy(&lid2[4],&p[106]);
		point_copy(&lid2[5],&p[107]);
		point_copy(&lid2[6],&p[108]);
		point_copy(&lid2[7],&p[109]);
		point_copy(&lid2[8],&p[110]);
		point_copy(&lid2[9],&p[111]);
		point_copy(&lid2[10],&p[112]);
		point_copy(&lid2[11],&p[113]);
		point_copy(&lid2[12],&p[114]);
		point_copy(&lid2[13],&p[115]);
		point_copy(&lid2[14],&p[116]);
		point_copy(&lid2[15],&p[117]);







		point_copy(&handle1[0],&p[41]);
		point_copy(&handle1[1],&p[42]);
		point_copy(&handle1[2],&p[43]);
		point_copy(&handle1[3],&p[44]);
		point_copy(&handle1[4],&p[45]);
		point_copy(&handle1[5],&p[46]);
		point_copy(&handle1[6],&p[47]);
		point_copy(&handle1[7],&p[48]);
		point_copy(&handle1[8],&p[49]);
		point_copy(&handle1[9],&p[50]);
		point_copy(&handle1[10],&p[51]);
		point_copy(&handle1[11],&p[52]);
		point_copy(&handle1[12],&p[53]);
		point_copy(&handle1[13],&p[54]);
		point_copy(&handle1[14],&p[55]);
		point_copy(&handle1[15],&p[56]);


		point_copy(&handle2[0],&p[53]);
		point_copy(&handle2[1],&p[54]);
		point_copy(&handle2[2],&p[55]);
		point_copy(&handle2[3],&p[56]);
		point_copy(&handle2[4],&p[57]);
		point_copy(&handle2[5],&p[58]);
		point_copy(&handle2[6],&p[59]);
		point_copy(&handle2[7],&p[60]);
		point_copy(&handle2[8],&p[61]);
		point_copy(&handle2[9],&p[62]);
		point_copy(&handle2[10],&p[63]);
		point_copy(&handle2[11],&p[64]);
		point_copy(&handle2[12],&p[28]);
		point_copy(&handle2[13],&p[65]);
		point_copy(&handle2[14],&p[66]);
		point_copy(&handle2[15],&p[67]);


		point_copy(&spout1[0],&p[68]);
		point_copy(&spout1[1],&p[69]);
		point_copy(&spout1[2],&p[70]);
		point_copy(&spout1[3],&p[71]);
		point_copy(&spout1[4],&p[72]);
		point_copy(&spout1[5],&p[73]);
		point_copy(&spout1[6],&p[74]);
		point_copy(&spout1[7],&p[75]);
		point_copy(&spout1[8],&p[76]);
		point_copy(&spout1[9],&p[77]);
		point_copy(&spout1[10],&p[78]);
		point_copy(&spout1[11],&p[79]);
		point_copy(&spout1[12],&p[80]);
		point_copy(&spout1[13],&p[81]);
		point_copy(&spout1[14],&p[82]);
		point_copy(&spout1[15],&p[83]);



		point_copy(&spout2[0],&p[80]);
		point_copy(&spout2[1],&p[81]);
		point_copy(&spout2[2],&p[82]);
		point_copy(&spout2[3],&p[83]);
		point_copy(&spout2[4],&p[84]);
		point_copy(&spout2[5],&p[85]);
		point_copy(&spout2[6],&p[86]);
		point_copy(&spout2[7],&p[87]);
		point_copy(&spout2[8],&p[88]);
		point_copy(&spout2[9],&p[89]);
		point_copy(&spout2[10],&p[90]);
		point_copy(&spout2[11],&p[91]);
		point_copy(&spout2[12],&p[92]);
		point_copy(&spout2[13],&p[93]);
		point_copy(&spout2[14],&p[94]);
		point_copy(&spout2[15],&p[95]);




bezierSurface_set(&bc, rim);
module_color(curve,&orange);
module_bezierSurface(curve, &bc, divisions, 1);
module_rotateZ(curve,cos(M_PI/2),sin(M_PI/2));
module_bezierSurface(curve, &bc, divisions, 1);
module_rotateZ(curve,cos(M_PI/2),sin(M_PI/2));
module_bezierSurface(curve, &bc, divisions, 1);
module_rotateZ(curve,cos(M_PI/2),sin(M_PI/2));
module_bezierSurface(curve, &bc, divisions, 1);
module_identity(curve);


bezierSurface_set(&bc, body1);
module_bezierSurface(curve, &bc, divisions, 1);
bezierSurface_set(&bc, body2);
module_bezierSurface(curve, &bc, divisions, 1);
module_rotateZ(curve,cos(M_PI/2),sin(M_PI/2));
bezierSurface_set(&bc, body1);
module_bezierSurface(curve, &bc, divisions, 1);
bezierSurface_set(&bc, body2);
module_bezierSurface(curve, &bc, divisions, 1);
module_rotateZ(curve,cos(M_PI/2),sin(M_PI/2));
bezierSurface_set(&bc, body1);
module_bezierSurface(curve, &bc, divisions, 1);
bezierSurface_set(&bc, body2);
module_bezierSurface(curve, &bc, divisions, 1);
module_rotateZ(curve,cos(M_PI/2),sin(M_PI/2));
bezierSurface_set(&bc, body1);
module_bezierSurface(curve, &bc, divisions, 1);
bezierSurface_set(&bc, body2);
module_bezierSurface(curve, &bc, divisions, 1);
module_rotateZ(curve,cos(M_PI/2),sin(M_PI/2));
module_identity(curve);



bezierSurface_set(&bc, lid1);
module_bezierSurface(curve, &bc, divisions, 1);

bezierSurface_set(&bc, lid2);
module_bezierSurface(curve, &bc, divisions, 1);
module_rotateZ(curve,cos(M_PI/2),sin(M_PI/2));
bezierSurface_set(&bc, lid1);
module_bezierSurface(curve, &bc, divisions, 1);

bezierSurface_set(&bc, lid2);
module_bezierSurface(curve, &bc, divisions, 1);
module_rotateZ(curve,cos(M_PI/2),sin(M_PI/2));
bezierSurface_set(&bc, lid1);
module_bezierSurface(curve, &bc, divisions, 1);

bezierSurface_set(&bc, lid2);
module_bezierSurface(curve, &bc, divisions, 1);
module_rotateZ(curve,cos(M_PI/2),sin(M_PI/2));
bezierSurface_set(&bc, lid1);
module_bezierSurface(curve, &bc, divisions, 1);

bezierSurface_set(&bc, lid2);
module_bezierSurface(curve, &bc, divisions, 1);
module_rotateZ(curve,cos(M_PI/2),sin(M_PI/2));
module_identity(curve);


bezierSurface_set(&bc, handle1);
module_bezierSurface(curve, &bc, divisions, 1);

bezierSurface_set(&bc, handle2);
module_bezierSurface(curve, &bc, divisions, 1);
module_scale(curve,1,-1,1);
bezierSurface_set(&bc, handle1);
module_bezierSurface(curve, &bc, divisions, 1);
bezierSurface_set(&bc, handle2);
module_bezierSurface(curve, &bc, divisions, 1);
module_scale(curve,1,-1,1);
module_identity(curve);

bezierSurface_set(&bc, spout1);
module_bezierSurface(curve, &bc, divisions, 1);

bezierSurface_set(&bc, spout2);
module_bezierSurface(curve, &bc, divisions, 1);
module_scale(curve,1,-1,1);
bezierSurface_set(&bc, spout1);
module_bezierSurface(curve, &bc, divisions, 1);

bezierSurface_set(&bc, spout2);
module_bezierSurface(curve, &bc, divisions, 1);
module_scale(curve,1,-1,1);
module_identity(curve);



// bezierSurface_set(&bc, &p[41]);
// 	// put the curve into a module
// 	module_color(curve, &green);
// bezierSurface_set(&bc, &p[53]);
// 	// put the curve into a module
// 	module_bezierSurface(curve, &bc, divisions, 0);

// 	bezierSurface_set(&bc, &p[68]);
// 	// put the curve into a module
// 	module_bezierSurface(curve, &bc, divisions, 0);
// bezierSurface_set(&bc, &p[88]);
// 	// put the curve into a module
// 	module_bezierSurface(curve, &bc, divisions, 0);


	// set up the drawstate
	drawstate_setColor(&ds, white);

	// set up the view
	point_set3D(&(view.vrp), 5, 5, 1.5 );
	vector_set( &(view.vpn), -view.vrp.val[0], -view.vrp.val[1],-view.vrp.val[2]+2 );
	vector_set( &(view.vup), 0.0, 0, 1.0 );
	view.d = 1.5;
	view.du = 1.0;
	view.dv = 1.0*rows/cols;
	view.screeny = rows;
	view.screenx = cols;
	view.f = 0.0;
	view.b = 3.0;

	matrix_setView3D( &VTM, &view );
	matrix_identity( &GTM );

	// Create the animation by adjusting the GTM
	for(frame=0;frame<60;frame++) {
		char buffer[256];
		
		matrix_rotateZ(&GTM, cos(M_PI/30.0), sin(M_PI/30.0) );
		module_draw( curve, &VTM, &GTM, &ds, src );

		sprintf(buffer, "teapot-frame%03d.ppm", frame);
		image_write(src, buffer);
		image_reset(src);
	}

	// clean up
	image_free( src );

	module_delete( curve );

	return(0);
}