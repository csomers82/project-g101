#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include <GL/glew.h>
#include <GL/freeglut.h>







int collision_mouse_convex_polygon(struct Vertex * polygon, int n_vert)
{
	struct Vertex * v;// 
	int i;//
	
	return(0);
}

int collision_mouse_circle(float cvx, float cvy, float cr)
{
	float d2 = (pow(norm_X - cvx, 2) + pow(norm_Y - cvy, 2));
	float r2 = pow(cr, 2);
	printf("  radius - distance\n%8.5f - %-8.5f\n", r2, d2);
	if(d2 == r2) return(2);
	else if(d2 < r2) return(1);
	return(0);
}

