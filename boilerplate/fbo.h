/*#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>


struct MyFBO
{
	GLuint textureID;
	GLuint target;
	int width;
	int height;

	// initialize object names to zero (OpenGL reserved value)
	MyFBO();
};

bool InitializeFBO(MyFBO* fbo, GLuint target = GL_TEXTURE_2D, MyTexture* rendered);

// deallocate texture-related objects
void DestroyFBO(MyFBO *fbo);
*/
