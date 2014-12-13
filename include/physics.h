/*
Erfan & Ryan
Date: 4 November 2014
File: bezier.h
*/

#ifndef __PHYSICS_H__
#define __PHYSICS_H__

#include <math.h>
#include "module.h"
#include "drawState.h"
#include "point.h"


void physics_Animation(Module *mod,Point velocity, Point gravity, int numFrames,Matrix * VTM, Matrix *GTM,DrawState ds);



#endif