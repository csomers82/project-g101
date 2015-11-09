#include "collision.h"
#include "../GL_Manager/gl_manager.h"
#include "../Physics/physics.h"
#include "../Mathematics/mathematics.h"

int collision_mouse_convex_polygon(Mouse * pos, Vertex * polygon, int n_vert)
{
	Vertex * pivot = NULL;// a ptr to a vertex in question 
	Vertex * next = NULL;
	Vector * at_mouse;// a vector from pivot to mouse
	Vector * at_next;// a vector from pivot to next vertex
	Vector * cross_p;// cross product
	int count_neg;// count of negative cross product values
	int i;// index for iterating through each vertex
	float ask_x = pos->X;// normalized X position in query
	float ask_y = pos->Y;// normalized Y position in query
	pivot = &polygon[0];
	for(i = 0; i < n_vert; ++i)
	{
		/* check to see if the cross product of the point in 
		 * question and vertex is positive */
		next = &(polygon[(i + 1) % n_vert]);
		at_mouse = vector_create(ask_x - pivot->X,	ask_y - pivot->Y,	0.0f - pivot->Z);
		at_next = vector_create(next->X - pivot->X, next->Y - pivot->Y, next->Z - pivot->Z); 
		cross_p = cross_product(*at_next, *at_mouse);
		count_neg += (cross_p->Z < 0)? 1: 0; 
		free(pivot);
		free(cross_p);
		pivot = next;
	}
	free(pivot);
	if(count_neg == n_vert || count_neg == 0)
	{
		return(1);
	}
	return(0);
}

int collision_mouse_circle(Mouse * pos, float cvx, float cvy, float cr)
{
	float distance_squared = (pow(pos->X - cvx, 2) + pow(pos->Y - cvy, 2));
	float radius_squared = pow(cr, 2);
	//printf("  radius - distance\n%8.5f - %-8.5f\n", radius_squared, distance_squared);
	if(distance_squared == radius_squared) return(2);
	else if(distance_squared < radius_squared) return(1);
	return(0);
}

