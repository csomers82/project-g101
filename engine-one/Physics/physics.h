#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <assert.h>



#ifndef EO_PHYSICS
#define EO_PHYSICS

typedef struct {
	float X;
	float Y;
	float Z;
} Vector;


Vector * vector_create(float x, float y, float z);

void vector_destroy(Vector * vic);



#endif






