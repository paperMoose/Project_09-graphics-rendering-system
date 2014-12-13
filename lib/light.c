/*
Erfan & Ryan
File: light.c
Date: 2 December 2014
*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "light.h"


//light functions

//initialize the light to default values
void light_init( Light *light ){
	if(light != NULL){
		vector_set(&light->direction,0,-1,0);
		Color_set(&light->color,1,1,1);
		light->type = LightNone;
		point_set3D(&light->position, 0,1,0);
		light->cutoff = cos(45*M_PI/180);
		light->sharpness = 1;
	}
}


//copy the light information.
void light_copy( Light *to, Light *from ){
	if(NULL != to && NULL != from){
		*to = *from;
	}
}


//Lighting functions

//allocate and return a new lighting structure set to default values.
Lighting *lighting_create( void ){
	Lighting *new = malloc(sizeof(Lighting));
	new->nLights = 0;
	return new;
}

//initialize the lighting structure to default values.
void lighting_init( Lighting *l ){
	if(l != NULL){
		l->nLights = 0;
	}
}

//add a new light to the Lighting structure given the parameters, some of which maybe NULL,
//depending upon the type. Make sure you don’t add more lights than MAX LIGHTS.
void lighting_add( Lighting *l, LightType type, Color *c, Vector *dir, Point *pos, float cutoff, float sharpness ){
	if(l != NULL){
		if(l->nLights<MAX_LIGHTS){
			Light *new = &l->light[l->nLights];
			light_init(new);
			if(c != NULL){
				new->color = *c;
			}
			if(dir != NULL){
				new->direction = *dir;
			}
			if(pos != NULL){
				new->position = *pos;
			}
			new->cutoff = cutoff;
			new->type = type;
			new->sharpness = sharpness;
			// // l->light[l->nLights] = new;
			l->nLights++;
		}
	}
}

//calculate the proper color given the normal N, view vector V, 3D point P, body color Cb, surface
//color Cs, sharpness value s, the lighting, and whether the polygon is one-sided or two-sided. Put the
//result in the Color c.
void lighting_shading( Lighting *l, Vector *N, Vector *V, Point *p, Color *Cb, Color *Cs, float s, int oneSided, Color *c ){
	int i;
	float r = 0;
	float g = 0;
	float b = 0;
	Vector H;
	vector_normalize(N);
	vector_normalize(V);
	for(i=0; i<l->nLights; i++){
		if(l->light[i].type == LightAmbient){
			r +=l->light[i].color.rgb[0] * Cb->rgb[0];
			g +=l->light[i].color.rgb[1] * Cb->rgb[1];
			b +=l->light[i].color.rgb[2] * Cb->rgb[2];
			// printf("r %f,g %f,b %f,",r,g,b);
		}else if(l->light[i].type == LightPoint){
			Vector L;
			vector_set(&L,l->light[i].position.val[0] -p->val[0],l->light[i].position.val[1] -p->val[1],l->light[i].position.val[2] -p->val[2]);

			vector_normalize(&L);
			float dot1 = vector_dot(&L,N);

			
			if(dot1<0 && oneSided == 1){
				printf("light not on\n");
				continue;
			}

			float sigma = vector_dot(V,N);

			if((sigma>0 && dot1 < 0) || (sigma<0 && dot1 > 0)){
				printf("light and view on different sides of shape\n");
				continue;
			}



			Vector temp = vector_add(&L,V);
			vector_set(&H, temp.val[0]/2, temp.val[1]/2, temp.val[2]/2);
			float dot2 = vector_dot(&H,N);

			if(dot1<0){
				dot1= -dot1;
				dot2 = -dot2;
			}



			// printf("dot1 %f",dot1);
			// printf("dot2 %f",dot2);

			point_print(&L,stdout);

			point_print(N,stdout);

			r += Cb->rgb[0]*l->light[i].color.rgb[0]*dot1 + l->light[i].color.rgb[0]*Cs->rgb[0]*pow(dot2,s);
			g += Cb->rgb[1]*l->light[i].color.rgb[1]*dot1 + l->light[i].color.rgb[1]*Cs->rgb[1]*pow(dot2,s);
			b += Cb->rgb[2]*l->light[i].color.rgb[2]*dot1 + l->light[i].color.rgb[2]*Cs->rgb[2]*pow(dot2,s);


			// printf("body r %f,body g %f,body b %f, \n ",Cb->rgb[0] ,Cb->rgb[1],Cb->rgb[2]);

			// printf("surface r %f,surface g %f,surface b %f, \n ",Cs->rgb[0] ,Cs->rgb[1],Cs->rgb[2]);

			// printf("r %f,g %f,b %f, \n ",r,g,b);
		}else if(l->light[i].type == LightSpot){
			Vector L;
			Vector Lneg;
			Vector dir;
			vector_set(&L,l->light[i].position.val[0] -p->val[0],l->light[i].position.val[1] -p->val[1],l->light[i].position.val[2] -p->val[2]);
			vector_set(&Lneg,-(l->light[i].position.val[0] -p->val[0]),-(l->light[i].position.val[1] -p->val[1]),-(l->light[i].position.val[2] -p->val[2]));
			vector_normalize(&L);
			vector_normalize(&Lneg);
			vector_copy(&dir,&l->light[i].direction);
			vector_normalize(&dir);

			float dot1 = vector_dot(&L,N);

			
			if(dot1<0 && oneSided == 1){
				printf("light not on\n");
				continue;
			}

			float sigma = vector_dot(V,N);

			if((sigma>0 && dot1 < 0) || (sigma<0 && dot1 > 0)){
				printf("light and view on different sides of shape\n");
				continue;
			}



			Vector temp = vector_add(&L,V);
			vector_set(&H, temp.val[0]/2, temp.val[1]/2, temp.val[2]/2);
			float dot2 = vector_dot(&H,N);

			if(dot1<0){
				dot1= -dot1;
				dot2 = -dot2;
			}

			float t = vector_dot(&Lneg,&dir);
// pow(l->light[i].cutoff,l->light[i].sharpness)*
			if(t>l->light[i].cutoff){
				printf("happening \n" );
					(r += Cb->rgb[0]*l->light[i].color.rgb[0]*dot1 + l->light[i].color.rgb[0]*Cs->rgb[0]*pow(dot2,s));
					(g += Cb->rgb[1]*l->light[i].color.rgb[1]*dot1 + l->light[i].color.rgb[1]*Cs->rgb[1]*pow(dot2,s));
					(b += Cb->rgb[2]*l->light[i].color.rgb[2]*dot1 + l->light[i].color.rgb[2]*Cs->rgb[2]*pow(dot2,s));
				continue;
			}
			else{
				r += pow(l->light[i].cutoff,l->light[i].sharpness)*(Cb->rgb[0]*l->light[i].color.rgb[0]*dot1 + l->light[i].color.rgb[0]*Cs->rgb[0]*pow(dot2,s));
				g += pow(l->light[i].cutoff,l->light[i].sharpness)*(Cb->rgb[1]*l->light[i].color.rgb[1]*dot1 + l->light[i].color.rgb[1]*Cs->rgb[1]*pow(dot2,s));
				b += pow(l->light[i].cutoff,l->light[i].sharpness)*(Cb->rgb[2]*l->light[i].color.rgb[2]*dot1 + l->light[i].color.rgb[2]*Cs->rgb[2]*pow(dot2,s));

			}

		}else{
			continue;
		}
	}
// clipping color channels
		if(r>1){
			r=1;
		}
		if(g>1){
			g=1;
		}
		if(b>1){
			b=1;
		
		}
		if(r<0){
			r=0;
		}
		if(g<0){
			g=0;
		}
		if(b<0){
			b=0;
		}
		// printf("r %f,g %f,b %f, FINAL \n ",r,g,b);
	Color_set(c,r,g,b);
}





