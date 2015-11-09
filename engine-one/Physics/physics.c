#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include "physics.h"
#include "../Mathematics/mathematics.h"
#include "../Geometry/geomtry.h"


Vector * vector_create(float x, float y, float z)
{
	Vector * vector = malloc(sizeof(Vector));
	vector.X = x;
	vector.Y = y;
	vector.Z = z;
	return(vector);
}

void vector_destroy(Vector * vic)
{	
	free(vic);
}





