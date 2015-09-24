#version 330
//uniform sampler2D textureDiffuse;// for texture use

in vec4 f_color;


void main()
{

	gl_FragColor = f_color; //vec4(1.0f, 0.0f, 0.0f, 1.0f);

}
