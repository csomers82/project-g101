#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include <GL/glew.h>
#include <GL/freeglut.h>



#ifndef EO_GEOMETRY
#define EO_GEOMETRY

typedef struct {
	float X;
	float Y;
	float Z;
	float R;
	float G;
	float B;
	float A;
} Vertex;



void init_Vertex(Vertex * V, float x, float y, float r, float g, float b) ;
struct Vertex * create_regular_polygon(float orig_x, float orig_y, float radius);
void init_Triangle(Vertex * T, float r, float g, float b);



#endif




