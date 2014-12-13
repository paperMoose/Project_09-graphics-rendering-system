/*
Erfan Azad
Date: 9 Oct 2014
File: matrix.c
*/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "matrix.h"
#include "point.h"
#include "polyline.h"
#include "polygon.h"
#include "line.h"


//Print out the matrix in a nice 4x4 arrangement with a blank line below
void matrix_print(Matrix *mat, FILE *fp){
	if(NULL != mat && NULL != fp){
		fprintf(fp, "| %.3f     %.3f     %.3f     %.3f |\n",mat->m[0][0], mat->m[0][1], mat->m[0][2], mat->m[0][3]);
		fprintf(fp, "| %.3f     %.3f     %.3f     %.3f |\n",mat->m[1][0], mat->m[1][1], mat->m[1][2], mat->m[1][3]);
		fprintf(fp, "| %.3f     %.3f     %.3f     %.3f |\n",mat->m[2][0], mat->m[2][1], mat->m[2][2], mat->m[2][3]);
		fprintf(fp, "| %.3f     %.3f     %.3f     %.3f |\n",mat->m[3][0], mat->m[3][1], mat->m[3][2], mat->m[3][3]);
		fprintf(fp,"\n");
	}
}

//Set the matrix to all zeros.
void matrix_clear(Matrix *mat){
	if(NULL != mat){
		int i, j;
		for(i=0; i<4; i++){
			for(j=0; j<4; j++){
				mat->m[i][j] = 0.0;
			}
		}
	}
}

// Set the matrix to the identity matrix.
void matrix_identity(Matrix *mat){
	if(NULL != mat){
		int i, j;
		for(i=0; i<4; i++){
			for(j=0; j<4; j++){
				if(i == j){
					mat->m[i][j] = 1.0;
				}else{
					mat->m[i][j] = 0.0;
				}
			}
		}
	}
	
}

//Return the element of the matrix at row r, column c.
double matrix_get(Matrix *mat, int r, int c){
	if(NULL != mat){
		return mat->m[r][c];
	}
	printf("ERROR: matrix_get >> mat is NULL");
	exit(-1);
}

// Set the element of the matrix at row r, column c to v
void matrix_set(Matrix *mat, int r, int c, double v){
	if(NULL != mat){
		mat->m[r][c] = v;
	}
}

//Copy the src matrix into the dest matrix.
void matrix_copy(Matrix *dest, Matrix *src){
	if(NULL != dest && NULL != src){
		*dest = *src;
	}
}

//Transpose the matrix mat in place
void matrix_transpose(Matrix *mat){
	if(NULL != mat){
		double temp;
		temp = mat->m[0][1];
		mat->m[0][1] = mat->m[1][0];
		mat->m[1][0] = temp;

		temp = mat->m[0][2];
		mat->m[0][2] = mat->m[2][0];
		mat->m[2][0] = temp;

		temp = mat->m[0][3];
		mat->m[0][3] = mat->m[3][0];
		mat->m[3][0] = temp;

		temp = mat->m[1][2];
		mat->m[1][2] = mat->m[2][1];
		mat->m[2][1] = temp;

		temp = mat->m[1][3];
		mat->m[1][3] = mat->m[3][1];
		mat->m[3][1] = temp;

		temp = mat->m[2][3];
		mat->m[2][3] = mat->m[3][2];
		mat->m[3][2] = temp;
	}
}

//Multiply left and right and put the result in mat
void matrix_multiply(Matrix *left, Matrix *right, Matrix *mat){
	if(NULL != left && NULL != right && NULL != mat){
		int i, j;
		Matrix temp;
		for(i=0; i<4; i++){
			for(j=0; j<4; j++){
				temp.m[i][j] = left->m[i][0]*right->m[0][j];
				temp.m[i][j] += left->m[i][1]*right->m[1][j];
				temp.m[i][j] += left->m[i][2]*right->m[2][j];
				temp.m[i][j] += left->m[i][3]*right->m[3][j];
			}
		}
		*mat=temp;
	}
}

/*
Transform the point p by the matrix mat and put the result in q. 
For this function, p and q need to be different variables.
*/
void matrix_xformPoint(Matrix *mat, Point *p, Point *q){
	if(NULL != mat && NULL != p && NULL != q){
		int i;
		Point temp;
		for (i = 0; i < 4; i++)
		{
			temp.val[i] = mat->m[i][0]*p->val[0];
			temp.val[i] += mat->m[i][1]*p->val[1];
			temp.val[i] += mat->m[i][2]*p->val[2];
			temp.val[i] += mat->m[i][3]*p->val[3];
		}
		*q=temp;
	}
}

void matrix_xformVector(Matrix *mat, Vector *p, Vector *q){
	if(NULL != mat && NULL != p && NULL != q){
		int i;
		Vector temp;
		for (i = 0; i < 4; i++)
		{
			temp.val[i] = mat->m[i][0]*p->val[0];
			temp.val[i] += mat->m[i][1]*p->val[1];
			temp.val[i] += mat->m[i][2]*p->val[2];
			temp.val[i] += mat->m[i][3]*p->val[3];
		}
		*q=temp;
	}
}


//Transform the points and surface normals (if they exist) in the Polygon p by the matrix mat
void matrix_xformPolygon(Matrix *mat, Polygon *p){
	if(NULL != mat && NULL != p){
		int i,j;
		for (i = 0; i < p->numVertex; i++)
		{
			Point temp;
			Vector tempVec;
			for (j = 0; j < 4; j++)
			{
				temp.val[j] = mat->m[j][0]*p->vertex[i].val[0];
				temp.val[j] += mat->m[j][1]*p->vertex[i].val[1];
				temp.val[j] += mat->m[j][2]*p->vertex[i].val[2];
				temp.val[j] += mat->m[j][3]*p->vertex[i].val[3];

				tempVec.val[j] = mat->m[j][0]*p->normal[i].val[0];
				tempVec.val[j] += mat->m[j][1]*p->normal[i].val[1];
				tempVec.val[j] += mat->m[j][2]*p->normal[i].val[2];
				tempVec.val[j] += mat->m[j][3]*p->normal[i].val[3];

			}
			p->vertex[i]=temp;
			p->normal[i] = tempVec;
		}
	}	
}

// – Transform the points in the Polyline p by the matrix m.
void matrix_xformPolyline(Matrix *mat, Polyline *p){
	if(NULL != mat && NULL != p){
		int i,j;
		for (i = 0; i < p->numVertex; i++)
		{
			Point temp;
			for (j = 0; j < 4; j++)
			{
				temp.val[j] = mat->m[j][0]*p->vertex[i].val[0];
				temp.val[j] += mat->m[j][1]*p->vertex[i].val[1];
				temp.val[j] += mat->m[j][2]*p->vertex[i].val[2];
				temp.val[j] += mat->m[j][3]*p->vertex[i].val[3];
			}
			(p->vertex[i])=temp;
		}
	}	
}

// – Transform the points in line by the matrix m.
void matrix_xformLine(Matrix *mat, Line *line){
	if(NULL != mat && NULL != &line->a && &line->b != NULL){
		int j;
		Point temp;
		for (j = 0; j < 4; j++)
		{
			temp.val[j] = mat->m[j][0]*line->a.val[0];
			temp.val[j] += mat->m[j][1]*line->a.val[1];
			temp.val[j] += mat->m[j][2]*line->a.val[2];
			temp.val[j] += mat->m[j][3]*line->a.val[3];
		}
		line->a = temp;

		for (j = 0; j < 4; j++)
		{
			temp.val[j] = mat->m[j][0]*line->b.val[0];
			temp.val[j] += mat->m[j][1]*line->b.val[1];
			temp.val[j] += mat->m[j][2]*line->b.val[2];
			temp.val[j] += mat->m[j][3]*line->b.val[3];
		}
		line->b = temp;
	}
}

//– Premultiply the matrix by a scale matrix parameterized by sx and sy.
void matrix_scale2D(Matrix *mat, double sx, double sy){
	if(NULL != mat){
		int i;
		for(i=0; i<4; i++){
			mat->m[0][i]=mat->m[0][i]*sx;
			mat->m[1][i]=mat->m[1][i]*sy;
		}
	}
}

/*
– Premultiply the matrix by a Z-axis rotation matrix parameterized by cos(✓) and sin(✓), where ✓ is
the angle of rotation about the Z-axis
*/
void matrix_rotateZ(Matrix *mat, double cth, double sth){
	if(NULL != mat){
		Matrix rotate;
		matrix_identity(&rotate);
		rotate.m[0][0]=cth;
		rotate.m[1][0]=sth;
		rotate.m[0][1]=-sth;
		rotate.m[1][1]=cth;
		matrix_multiply(&rotate,mat,mat);
	}
}

// – Premultiply the matrix by a 2D translation matrix parameterized by tx and ty.
void matrix_translate2D(Matrix *mat, double tx, double ty){
	if(NULL != mat){
		int i;
		for(i=0; i<4; i++){
			mat->m[0][i] = mat->m[0][i] + (tx * mat->m[3][i]);
			mat->m[1][i] = mat->m[1][i] + (ty * mat->m[3][i]);
		}
	}
}

// – Premultiply the matrix by a 2D shear matrix parameterized by shx and shy.
// will attempt to optimize later
void matrix_shear2D(Matrix *mat, double shx, double shy){
	if(NULL != mat){
		double temp1, temp2;
		int i;
		for(i=0; i<4; i++){
			temp1 = mat->m[0][i] + (shx * mat->m[1][i]);
			temp2 = mat->m[1][i] + (shy * mat->m[0][i]);
			mat->m[0][i] = temp1;
			mat->m[1][i] = temp2;
		}
		// Matrix shear;
		// matrix_identity(&shear);
		// shear.m[0][1]=shx;
		// shear.m[1][0]=shy;
		// matrix_multiply(&shear,mat,mat);
	}
}

// – Premultiply the matrix by a translation matrix parameterized by tx, ty, and tz.
void matrix_translate(Matrix *mat, double tx, double ty, double tz){
	if(NULL != mat){
		int i;
		for(i=0; i<4; i++){
			mat->m[0][i] = mat->m[0][i] + (tx * mat->m[3][i]);
			mat->m[1][i] = mat->m[1][i] + (ty * mat->m[3][i]);
			mat->m[2][i] = mat->m[2][i] + (tz * mat->m[3][i]);
		}
	}
}

// – Premultiply the matrix by a scale matrix parameterized by sx, sy, sz.
void matrix_scale(Matrix *mat, double sx, double sy, double sz){
	if(NULL != mat){
		int i;
		for(i=0; i<4; i++){
			mat->m[0][i]=mat->m[0][i]*sx;
			mat->m[1][i]=mat->m[1][i]*sy;
			mat->m[2][i]=mat->m[2][i]*sz;
		}
	}
}
// – Premultiply the matrix by a X-axis rotation matrix parameterized by cos(✓) and sin(✓), where ✓ is
// the angle of rotation about the X-axis.
void matrix_rotateX(Matrix *mat, double cth, double sth){
	if(NULL != mat){
		Matrix rotate;
		matrix_identity(&rotate);
		rotate.m[1][1]=cth;
		rotate.m[1][2]=-sth;
		rotate.m[2][1]=sth;
		rotate.m[2][2]=cth;
		matrix_multiply(&rotate,mat,mat);
	}
}

// Premultiply the matrix by a Y-axis rotation matrix parameterized by cos(✓) and sin(✓), where ✓ is
// the angle of rotation about the Y-axis.
void matrix_rotateY(Matrix *mat, double cth, double sth){
		if(NULL != mat){
		Matrix rotate;
		matrix_identity(&rotate);
		rotate.m[0][0]=cth;
		rotate.m[0][2]=sth;
		rotate.m[2][0]=-sth;
		rotate.m[2][2]=cth;
		matrix_multiply(&rotate,mat,mat);
	}
}

// – Premultiply the matrix by an XYZ-axis rotation matrix parameterized by the vectors ~u, ~v, and w~,
// where the three vectors represent an orthonormal 3D basis.
void matrix_rotateXYZ(Matrix *mat, Vector *u, Vector *v, Vector *w){
	vector_normalize(u);
	vector_normalize(v);
	vector_normalize(w);
	Matrix rotate;
	matrix_identity(&rotate);
	rotate.m[0][0]= u->val[0];
	rotate.m[0][1]= u->val[1];
	rotate.m[0][2]= u->val[2];
	rotate.m[1][0]= v->val[0];
	rotate.m[1][1]= v->val[1];
	rotate.m[1][2]= v->val[2];
	rotate.m[2][0]= w->val[0];
	rotate.m[2][1]= w->val[1];
	rotate.m[2][2]= w->val[2];
	matrix_multiply(&rotate,mat,mat);
}

// – Premultiply the matrix by a shear Z matrix parameterized by shx and shy.
void matrix_shearZ(Matrix *mat, double shx, double shy){
	if(NULL != mat){
		int i;
		for(i=0; i<4; i++){
			mat->m[0][i] = mat->m[0][i] + (shx * mat->m[2][i]);
			mat->m[1][i] = mat->m[1][i] + (shy * mat->m[2][i]);
		}
	}
}

// – Premultiply the matrix by a perspective matrix parameterized by d.
void matrix_perspective(Matrix *mat, double d){
	if(NULL != mat){
		int i;
		for(i=0; i<4; i++){
			mat->m[2][i] = mat->m[2][i]/d;
		}
	}

}


/*
    Calculates the  polygons srface normal vector. 
    puts the new vector in c.
*/
void polygon_normal(Polygon* p, Vector* c){
    if((NULL != p) && (NULL != p->vertex)){
        Vector a, b; 
        vector_set(&a, (p->vertex[1].val[0] - p->vertex[0].val[0]), (p->vertex[1].val[1] - p->vertex[0].val[1]), (p->vertex[1].val[2] - p->vertex[0].val[2]));
        vector_set(&b, (p->vertex[2].val[0] - p->vertex[0].val[0]), (p->vertex[2].val[1] - p->vertex[0].val[1]), (p->vertex[2].val[2] - p->vertex[0].val[2]));
        vector_cross(&a, &b, c);
    }
}









