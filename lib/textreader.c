// #include <stdlib.h>
// #include <stdio.h>
// #include "module.h"
// #include "drawState.h"
// #include "line.h"
// #include "point.h"
// #include "polyline.h"
// #include "polygon.h"
// #include "circle.h"
// #include "color.h"
// #include "Image.h"
// #include "textreader.h"
// #include "string.h"
// #include "view.h"



// StringList * string_create( int rows){
// 	StringList* list = malloc(sizeof(StringList));
// 	if(NULL == list){exit(-1);}
// 	list->lines = rows;
// 	string_init(list);
// 	string_alloc(list,rows);
// 	return list;
// }


// // sets all the fields to null
// void string_init( StringList* list){
// 	if(list != NULL){
// 		list->text = NULL;
// 		list->lines = 0;
// 	}
// }

// // allocates the memory
// void string_alloc(StringList* list, int rows){
// 	if(NULL != list){
// 		if(rows==0){
// 			printf("WARNING: rows is 0\n");
// 		}else{ // if rows and cols are non-zero, deallocate the src first
// 				string_dealloc(list);
// 			}

// 		list->text = malloc(sizeof(char*)*rows); // creating "rows". an array of char pointers
// 		if(NULL == list->text){exit(-1);}

// 		int k;
// 		for(k=0; k<rows; k++){
// 			list->text[k] = malloc(sizeof(char)*1000); // creating "cols". arrays of chars.
// 			if(NULL == list->text[k]){exit(-1);}
// 		}
// 		list->lines = rows;

// 		// resets data so no garbage
// 	}else{
// 		printf("ERROR: text >> text pointer in NULL\n");
// 		exit(-1);
// 	}
// }

// void string_dealloc(StringList *list){
// 	if(NULL != list){
// 		if(NULL != list->text){
// 			int i;
// 			for (i=0; i<list->lines; i++){ 
// 				free(list->text[i]); // free all the data cols
// 			}
// 			free(list->text); // free the data rows
// 			list->text[i] = NULL;
// 			list->lines = 0;
// 		}
// 	}else{
// 		printf("WARNING: problem with string dealloc is NULL\n");
// 	}
// }

// // reads in the text file line by line, will calculate the number of lines at run time
// StringList* readFile(void){
// 	printf("happening \n");
// 	// StringList *list = string_create(rows);
// 	FILE * ptr;
// 	char rowCounter[100][100];
// 		// printf("happening");
// 	ptr =fopen("instructions.txt","r");
// 	if (!ptr)
// 		exit(-1);
// 	int rows = 0;


// 	while (fgets(rowCounter[rows],1000, ptr)!=NULL){
// 		rows++;
// 	}
// 	printf("rows %d\n", rows);

// 	ptr =fopen("instructions.txt","r");

// 	StringList *list = string_create(rows);

// 	rows = 0;

// 	while (fgets(list->text[rows],1000, ptr)!=NULL){
// 		printf("%s \n",list->text[rows]);
// 		rows++;
// 	}

// 	fclose(ptr);

// 	return list;
// }

// // reads in lines and polygons from a txt file
// void drawShapes(StringList *list){
// 	int i,j,x;
// 	j=0;
// 	char temp[100];
// 	char *p;
// 	char fileName[50] = "default";

// 	// default image size
// 	Image *src;
// 	src = image_create(500,500);

// 	// ======default color value
// 	Color global;
// 	Color_set(&global,1,1,1);

	
// 	for (i = 0; i < list->lines; i++)
// 	{
// 		char row[50][50];
// 		int pointIter = 2;
// 		strncpy(temp,&list->text[i][0],50);
// 		p = strtok(temp," ");
// 		printf("%s \n",p);
// 		j=0;

// 		while(p){
// 			strncpy(row[j],p,50);
// 			p = strtok(NULL, " ");
// 			printf("%s \n",p);
// 			j++;
// 		}
// 		printf("%s \n",row[0]);

// 		// polygon case
// 		if (strcmp(row[0],"#") == 0 )
// 		{

// 			printf("comment");
// 			continue;

// 		}


// 		// polygon case
// 		if (strcmp(row[0],"polygon") == 0 )
// 		{
// 			printf("polygon \n" );
// 			int pointNum = atoi(row[1]);

// 			Point point[pointNum];

// 			for ( x = 0; x < pointNum; x++)
// 			{
// 				int xVal = atoi(row[pointIter]);
// 				int yVal = atoi(row[pointIter+1]);
// 				printf("%d %d \n",xVal,yVal);
// 				point_set2D(&point[x],xVal,yVal);
// 				point_print(&point[x],stdout);
// 				pointIter+=2;
// 			}
// 			Polygon *poly =polygon_createp(pointNum,point);
// 			polygon_set(poly,pointNum,point);
// 			polygon_draw(poly, src,global);
// 			polygon_free(poly);
// 		}

// // handles colors
// 		if (strcmp(row[0],"color") == 0 )
// 		{
// 			printf("color \n" );
// 			float r = atof(row[1]);
// 			float g = atof(row[2]);
// 			float b = atof(row[3]);

// 			Color_set(&global,r,g,b);
// 		}

// 		// handles images
// 		if (strcmp(row[0],"image") == 0 )
// 		{
// 			printf("color \n" );
// 			int rows = atoi(row[1]);
// 			int cols = atoi(row[2]);

// 			src = image_create(rows,cols);
// 		}

// 		if (strcmp(row[0],"filename") == 0 )
// 		{
// 			// printf("filename %d \n" ,strlen(row[1]));
// 			strncpy(fileName,row[1],strlen(row[1]));
// 			// fileName = row[1];
// 		}





// // polyline case
// 		if (strcmp(row[0],"polyline") == 0 )
// 		{
// 			printf("polyline \n" );
// 			int pointNum = atoi(row[1]);

// 			Point point[pointNum];

// 			for ( x = 0; x < pointNum; x++)
// 			{
// 				int xVal = atoi(row[pointIter]);
// 				int yVal = atoi(row[pointIter+1]);
// 				printf("%d %d \n",xVal,yVal);
// 				point_set2D(&point[x],xVal,yVal);
// 				point_print(&point[x],stdout);
// 				pointIter+=2;
// 			}
// 			Polyline *poly =polyline_createp(pointNum,point);
// 			polyline_draw(poly, src,global);
// 			polyline_free(poly);
// 		}

// // line case
// 		if (strcmp(row[0],"drawline") == 0 )
// 		{
// 			printf("line \n" );

// 			Point point[2];

// 				int xVal1 = atoi(row[1]);
// 				int yVal1 = atoi(row[2]);
// 				int xVal2 = atoi(row[3]);
// 				int yVal2 = atoi(row[4]);

// 				point_set2D(&point[0],xVal1,yVal1);
// 				point_print(&point[0],stdout);

// 				point_set2D(&point[1],xVal2,yVal2);
// 				point_print(&point[1],stdout);

// 			Line line;
// 			line_set(&line,point[0],point[1]);
// 			line_draw(&line, src,global);
// 		}




// 	}
// 	image_write(src,fileName);
// 	image_free(src);

// }


// // currently only works for 2d modules
// void modulesMaker(StringList *list,int modules, int polyNum, int lineNum, int colorNum, int polylinesNum, int pointNum){
// 	int i,j,x,k;
// 	j=0;
// 	char temp[100];
// 	char *p;
// 	Matrix vtm, gtm;
// 	Module *mods[modules];
// 	//View3D view3;
// 	View2D view2;

// 	matrix_identity( &gtm );
// 	matrix_identity( &vtm );

// 	Polygon *polygons[polyNum];
// 	Polyline *polylines[polylinesNum];


// 	DrawState *ds = drawstate_create();
// 	ds->shade = ShadeConstant;

// 	char fileName[50] = "default";

// 	// for ( i = 0; i < polyNum; i++)
// 	// {
// 	// }

// // do not need initialization
// 	Line lines[lineNum];

// 	Color colors[colorNum];

// 	Point points[pointNum];

// 	for (k = 0; k < modules; k++)
// 	{

// 		mods[k]=module_create();
// 	}

// 	// default image size
// 	Image *src;
// 	src = image_create(500,500);

// 	// ======default color value
// 	Color global;
// 	Color_set(&global,1,1,1);
// 		printf("in func \n");

	
// 	for (i = 0; i < list->lines; i++)
// 	{
// 		char row[50][50];
// 		// sets the point iterator to the approprate string
// 		int pointIter = 3;
// 		strncpy(temp,&list->text[i][0],50);
// 		p = strtok(temp," ");
// 		j=0;

// 		while(p){
// 			strncpy(row[j],p,50);
// 			p = strtok(NULL, " ");
// 			// printf("%s \n",p);
// 			j++;
// 		}
// 		printf("this is just a row printout poopy %s \n",row[0]);

// 		// comment case
// 		if (strcmp(row[0],"#") == 0 )
// 		{

// 			printf("comment");
// 			continue;

// 		}

// 		// handles images
// 		if (strcmp(row[0],"image") == 0 )
// 		{
// 			printf("color \n" );
// 			int rows = atoi(row[1]);
// 			int cols = atoi(row[2]);

// 			src = image_create(rows,cols);
// 			continue;
// 		}

// 				// handles clearing modules
// 		if (strcmp(row[0],"module_clear") == 0 )
// 		{
// 			printf("clearing the module \n" );
// 			int moduleIdx = atoi(row[1]);
// 			printf("%d \n",moduleIdx);

// 			module_clear(mods[moduleIdx]);
// 			continue;

// 		}
// // polygon case
// 		if (strcmp(row[0],"polygon") == 0 )
// 		{
// 			printf("polygon \n" );
// 			int polyIdx = atoi(row[1]);
// 			int pointNum = atoi(row[2]);

// 			Point point[pointNum];

// 			for ( x = 0; x < pointNum; x++)
// 			{
// 				int xVal = atoi(row[pointIter]);
// 				int yVal = atoi(row[pointIter+1]);
// 				printf("%d %d \n",xVal,yVal);
// 				point_set2D(&point[x],xVal,yVal);
// 				point_print(&point[x],stdout);
// 				pointIter+=2;
// 			}
// 			polygons[polyIdx] =polygon_createp(pointNum,point);
// 			polygon_print(polygons[polyIdx],stdout);
// 			continue;
// 		}

// // polyline case
// 		if (strcmp(row[0],"polyline") == 0 )
// 		{
// 			printf("polyline \n" );
// 			int polyIdx = atoi(row[1]);
// 			int pointNum = atoi(row[2]);

// 			Point point[pointNum];

// 			for ( x = 0; x < pointNum; x++)
// 			{
// 				int xVal = atoi(row[pointIter]);
// 				int yVal = atoi(row[pointIter+1]);
// 				printf("%d %d \n",xVal,yVal);
// 				point_set2D(&point[x],xVal,yVal);
// 				point_print(&point[x],stdout);
// 				pointIter+=2;
// 			}
// 			polylines[polyIdx] =polyline_createp(pointNum,point);
// 			polyline_print(polylines[polyIdx],stdout);
// 			continue;
// 		}
// // point case!
// 		if (strcmp(row[0],"point") == 0 )
// 		{
// 			printf("point \n" );
// 			int pointIdx = atoi(row[1]);

// 			float xVal = atof(row[2]);
// 			float yVal = atof(row[3]);

// 			point_set2D(&points[pointIdx],xVal,yVal);
// 			point_print(&points[pointIdx],stdout);
// 			continue;
// 		}

// // line case!
// 		if (strcmp(row[0],"line") == 0 )
// 		{
// 			printf("line \n" );
// 			int lineIdx = atoi(row[1]);

// 			float xVal1 = atof(row[2]);
// 			float yVal1 = atof(row[3]);

// 			float xVal2 = atof(row[4]);
// 			float yVal2 = atof(row[5]);

// 			Point temp[2];
// 			point_set2D(&temp[0],xVal1,yVal1);
// 			point_print(&temp[0],stdout);

// 			point_set2D(&temp[1],xVal2,yVal2);
// 			point_print(&temp[1],stdout);

// 			line_set(&lines[lineIdx],temp[0],temp[1]);
// 			continue;

// 		}

// // color
// 		if (strcmp(row[0],"color") == 0 )
// 		{
// 			printf("color \n" );
// 			int colorIdx = atoi(row[1]);

// 			float r = atof(row[2]);
// 			float g = atof(row[3]);
// 			float b = atof(row[4]);
// 			Color_set(&colors[colorIdx],r,g,b);	
// 			continue;
// 		}

// // insert polygon
// 		if (strcmp(row[0],"module_polygon") == 0 )
// 		{
// 			printf("Module_polygon \n" );
// 			int modIdx = atoi(row[1]);
// 			int polIdx = atoi(row[2]);	
// 			module_polygon(mods[modIdx],polygons[polIdx]);
// 			continue;
// 		}

// // insert polyline
// 		if (strcmp(row[0],"module_polyline") == 0 )
// 		{
// 			printf("Module_polyline \n" );
// 			int modIdx = atoi(row[1]);
// 			int polIdx = atoi(row[2]);	
// 			module_polyline(mods[modIdx],polylines[polIdx]);
// 			continue;
// 		}
// // point case
// 		if (strcmp(row[0],"module_point") == 0 )
// 		{
// 			printf("Module_point \n" );
// 			int modIdx = atoi(row[1]);
// 			int pointIdx = atoi(row[2]);	
// 			module_point(mods[modIdx],&points[pointIdx]);
// 			continue;
// 		}

// 		// line case
// 		if (strcmp(row[0],"module_line") == 0 )
// 		{
// 			printf("Module_line in loop\n" );
// 			int modIdx = atoi(row[1]);

// 			int lineIdx = atoi(row[2]);	

// 			module_line(mods[modIdx],&lines[lineIdx]);
// 			continue;
// 		}

// // color case
// 		if (strcmp(row[0],"module_color") == 0 )
// 		{
// 			printf("Module_color in loop\n" );
// 			int modIdx = atoi(row[1]);

// 			int colorIdx = atoi(row[2]);	
// 			printf(" r g b %f, %f, %f \n", colors[colorIdx].rgb[0],colors[colorIdx].rgb[1],colors[colorIdx].rgb[2]);

// 			module_color(mods[modIdx],&colors[colorIdx]);
// 			continue;
// 		}

// 		// identify case
// 		if (strcmp(row[0],"module_identity") == 0 )
// 		{
// 			printf("Module_identity \n" );
// 			int modIdx = atoi(row[1]);
	

// 			module_identity(mods[modIdx]);
// 			continue;
// 		}

// 		if (strcmp(row[0],"module_translate2d") == 0 )
// 		{
// 			printf("Module_translate2d in loop \n" );
// 			int modIdx = atoi(row[1]);

// 			double x  = atof(row[2]);	
// 			double y  = atof(row[3]);	

// 			module_translate2D(mods[modIdx],x,y);
// 			continue;
// 		}

// 		if (strcmp(row[0],"module_scale2d") == 0 )
// 		{
// 			printf("Module_scale2d \n" );
// 			int modIdx = atoi(row[1]);

// 			double x  = atof(row[2]);	
// 			double y  = atof(row[3]);	

// 			module_scale2D(mods[modIdx],x,y);
// 			continue;
// 		}

// 		if (strcmp(row[0],"module_rotateZ") == 0 )
// 		{
// 			printf("Module_rotateZ \n" );
// 			int modIdx = atoi(row[1]);

// 			double cth  = atof(row[2]);	
// 			double sth  = atof(row[3]);	

// 			module_rotateZ(mods[modIdx],cth,sth);
// 			continue;
// 		}

// 		if (strcmp(row[0],"module_shear2D") == 0 )
// 		{
// 			printf("module_shear2D \n" );
// 			int modIdx = atoi(row[1]);

// 			double x  = atof(row[2]);	
// 			double y  = atof(row[3]);	

// 			module_shear2D(mods[modIdx],x,y);
// 			continue;
// 		}


// 		if (strcmp(row[0],"vrp2d") == 0 )
// 		{
// 			printf("vrp \n" );
// 			int x = atof(row[1]);

// 			int y  = atof(row[2]);	


// 			point_set2D(&view2.vrp,x,y);
// 			continue;
// 		}

// 		if (strcmp(row[0],"view2d_dx") == 0 )
// 		{
// 			float val  = atof(row[1]);
// 			printf("view2d_dx in loop  %f \n",val );

// 			view2.dx = val;
// 			continue;
// 		}

// 		if (strcmp(row[0],"view2d_setx") == 0 )
// 		{
// 			printf("view2d_setx \n" );
// 			float x  = atof(row[1]);
// 			float y  = atof(row[2]);
// 			float z  = atof(row[3]);


// 			vector_set( &(view2.x), x, y, z );
// 			continue;
// 		}

// // sets screen size
// 		if (strcmp(row[0],"view2d_screenSize") == 0 )
// 		{
// 			printf("view2d_screenSize \n" );
// 			int screenxVal  = atoi(row[1]);
// 			int screenyVal  = atoi(row[2]);


// 			view2.screenx = screenxVal;
// 			view2.screeny = screenyVal;
// 			printf("view2dset  \n" );
// 			matrix_setView2D(&vtm, &view2);
// 			matrix_print(&vtm,stdout);
// 			continue;
// 		}


// 		if (strcmp(row[0],"module_module") == 0 )
// 		{
// 			printf("module_module \n" );
// 			int modIdx = atoi(row[1]);

// 			int subIdx  = atoi(row[2]);		

// 			module_module(mods[modIdx],mods[subIdx]);
// 			continue;
// 		}

// 		if (strcmp(row[0],"module_draw") == 0 )
// 		{
// 			printf("module_draw\n" );
// 			int modIdx = atoi(row[1]);	
// 			module_draw(mods[modIdx], &vtm, &gtm, ds, src);
// 			continue;
// 		}


// 		if (strcmp(row[0],"filename") == 0 )
// 		{
// 			// printf("filename %d \n" ,strlen(row[1]));
// 			strncpy(fileName,row[1],strlen(row[1]));
// 			// fileName = row[1];
// 		}







// // polyline case


	
// 	}
// 	image_write(src,fileName);
// 	image_free(src);

// }


// // #include <stdlib.h>
// // #include <stdio.h>
// // #include "module.h"
// // #include "drawState.h"
// // #include "line.h"
// // #include "point.h"
// // #include "polyline.h"
// // #include "polygon.h"
// // #include "circle.h"
// // #include "color.h"
// // #include "Image.h"
// // #include "textreader.h"
// // #include "string.h"



// // StringList * string_create( int rows){
// // 	StringList* list = malloc(sizeof(StringList));
// // 	if(NULL == list){exit(-1);}
// // 	string_init(list);
// // 	string_alloc(list,rows);
// // 	return list;
// // }


// // // sets all the fields to null
// // void string_init( StringList* list){
// // 	if(list != NULL){
// // 		list->text = NULL;
// // 		list->lines = 0;
// // 	}
// // }
// // // allocates the memory
// // void string_alloc(StringList* list, int rows){
// // 	if(NULL != list){
// // 		if(rows==0){
// // 			printf("WARNING: rows is 0\n");
// // 		}else{ // if rows and cols are non-zero, deallocate the src first
// // 				string_dealloc(list);
// // 			}

// // 		list->text = malloc(sizeof(char*)*rows); // creating "rows". an array of char pointers
// // 		if(NULL == list->text){exit(-1);}

// // 		int k;
// // 		for(k=0; k<rows; k++){
// // 			list->text[k] = malloc(sizeof(char)*30); // creating "cols". arrays of chars.
// // 			if(NULL == list->text[k]){exit(-1);}
// // 		}
// // 		list->lines = rows;

// // 		// resets data so no garbage
// // 	}else{
// // 		printf("ERROR: text >> text pointer in NULL\n");
// // 		exit(-1);
// // 	}

// // }

// // void string_dealloc(StringList *list){
// // 	if(NULL != list){
// // 		if(NULL != list->text){
// // 			int i;
// // 			for (i=0; i<list->lines; i++){ 
// // 				free(list->text[i]); // free all the data cols
// // 			}
// // 			free(list->text); // free the data rows
// // 			list->text[i] = NULL;
// // 			list->lines = 0;
// // 		}
// // 	}else{
// // 		printf("WARNING: problem with string dealloc is NULL\n");
// // 	}
// // }

// // // reads in the text file line by line
// // StringList* readFile(int rows){
// // 	printf("happening \n");
// // 	StringList *list = string_create(rows);
// // 	FILE * ptr;
// // 		// printf("happening");
// // 	ptr =fopen("instructions.txt","r");
// // 	if (!ptr)
// // 		exit(-1);
// // 	int i = 0;
// // 	// printf("happening");
// // 	while (fgets(list->text[i],1000, ptr)!=NULL){
// // 		// printf("%s \n",list->text[i]);
// // 		i++;
// // 	}

// // 	fclose(ptr);

// // 	return list;
// // }

// // // reads in lines and polygons from a txt file
// // void drawShapes(StringList *list){
// // 	int i,j;
// // 	j=0;
// // 	char temp[100];
// // 	for (i = 0; i < list->lines; i++)
// // 	{
// // 	j=0;
// // 		// while(strcmp(&list->text[i][j],"_") != 0  && j <20){
// // 			printf("%d \n",strcmp(&list->text[i][j],"_"));
// // 			printf("%s \n",(&list->text[0][0]));
// // 			// temp[j] = list->text[i][j];
// // 			// printf("%s \n",&list->text[i][j]);
// // 			// j++;
// // 		// }
// // 			// printf("%s \n",&temp);
// // 		// j++; //move to the next character
// // 		// if(strcmp(&temp[j],"polygon") != 0 ){
// // 		// 	int numPoints = atoi(&list->text[i][j]);
// // 		// 	printf("%d \n",numPoints);
// // 		// 	Point p[numPoints];


// // 		// 	// while(strcmp(&list->text[i][j]," ") != 0 ){
// // 		// 	// 	temp[j] = list->text[i][j];
// // 		// 	// 	j++;
// // 		// 	// }

// // 		// 	// for (int j = i+2; j < numpoints; j++)
// // 		// 	// {
// // 		// 	// 	atof(list->text[i+1][j]);
// // 		// 	// }

// // 		// }


// // 	}

// // }


