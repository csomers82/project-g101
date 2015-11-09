#ifndef EO_COLLISION
#define EO_COLLISION

#include "../GL_Manager/gl_manager.h"
#include "../Geometry/geometry.h" 



int collision_mouse_convex_polygon(Mouse * pos, Vertex * POLYGON, int NUMBEROFVERTICES);
int collision_mouse_circle(Mouse * pos, float CIRCLEXPOSITION, float CIRCLEYPOSITION, float RADIUS);




#endif




