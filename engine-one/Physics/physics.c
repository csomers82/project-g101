#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <assert.h>





struct Vector * vector_create(float x, float y, float z)
{
	struct Vector * vector = malloc(sizeof(struct Vector));
	vector.X = x;
	vector.Y = y;
	vector.Z = z;
	return(vector);
}

void vector_destroy(struct Vector * vic)
{	
	free(vic);
}





