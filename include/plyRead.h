/*
Erfan & Ryan
File: light.h
Date: 2 December 2014
*/
#ifndef __PLYREAD__
#define __PLYREAD__
int readPLY(char filename[], int *nPolygons, Polygon **plist, Color **clist, int estNormals);
#endif