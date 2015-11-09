#ifndef EO_GL_MANAGER
#define EO_GL_MANAGER

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "../Physics/physics.h"
#include "../Geometry/geometry.h"

#define VEC_SHADER_FN "vertexShader.glsl"
#define FRAG_SHADER_FN "fragmentShader.glsl"
#define SIZE 800 

/* STRUCTURES */

/* struct Mouse *
 *	collects the mouse poisiton, depression state, (and pixel color?) */
typedef struct {
	float X;// normalized x position
	float Y;// normalized y position
	int pixel_x;// x pixel number
	int pixel_y;// y pixel number
	int state;// GL Mouse State
	int button;// GL Mouse Button
} Mouse;

/* FUNCTION DECLARATIONS */

void CheckGLErrors(void);
void Init_Gs(Vertex * polygon);
void MouseFunction(int button, int state, int x, int y);
void Passive(int x, int y);
void Keyboard(unsigned char k, int x, int y);
void KeyExtras(int k, int x, int y);
void Draw(void);
void DrawLoop(int val);
void GL_Manager_initialize();
void GL_Manager_create_window(char * name, int width, int length);



#endif
