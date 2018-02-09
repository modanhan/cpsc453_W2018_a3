/*#include "fbo.h"
#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <iostream>
#include <string>

using namespace std;

bool CheckGLErrors(const char* errorLocation)
{
	bool error = false;
	for (GLenum flag = glGetError(); flag != GL_NO_ERROR; flag = glGetError())
	{
		cout << errorLocation;
		switch (flag) {
		case GL_INVALID_ENUM:
			cout << "GL_INVALID_ENUM" << endl; break;
		case GL_INVALID_VALUE:
			cout << "GL_INVALID_VALUE" << endl; break;
		case GL_INVALID_OPERATION:
			cout << "GL_INVALID_OPERATION" << endl; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			cout << "GL_INVALID_FRAMEBUFFER_OPERATION" << endl; break;
		case GL_OUT_OF_MEMORY:
			cout << "GL_OUT_OF_MEMORY" << endl; break;
		default:
			cout << "[unknown error code]" << endl;
		}
		error = true;
	}
	return error;
}

MyFBO::MyFBO() : textureID(0), target(0), width(0), height(0)
	{}


bool InitializeFBO(MyFBO* fbo, GLuint target, MyTexture *rendered) {
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);		//Set alignment to be 1

		fbo->target = target;
		glGenFramebuffers(1, &fbo->textureID);
		glBindFramebuffer(fbo->target, fbo->textureID);
		GLuint format = GL_RGB;
		switch(numComponents)
		{
			case 4:
				format = GL_RGBA;
				break;
			case 3:
				format = GL_RGB;
				break;
			case 2:
				format = GL_RG;
				break;
			case 1:
				format = GL_RED;
				break;
			default:
				cout << "Invalid Texture Format" << endl;
				break;
		};
		
		//GLuint renderedTexture;
		glGenTextures(1, &rendered->target);

		// "Bind" the newly created texture : all future texture functions will modify this texture
		glBindTexture(rendered->target, rendered->textureID);
		
		// Give an empty image to OpenGL ( the last "0" )
		glTexImage2D(texture->target, 0, format, rendered->width, rendered->height, 0, format, GL_UNSIGNED_BYTE, 0);

		// Poor filtering. Needed !
		glTexParameteri(texture->target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(texture->target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(texture->target, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(texture->target, GL_TEXTURE_WRAP_T, GL_REPEAT);
		
		glTexImage2D(texture->target, 0, format, texture->width, texture->height, 0, format, GL_UNSIGNED_BYTE, data);

		// Note: Only wrapping modes supported for GL_TEXTURE_RECTANGLE when defining
		// GL_TEXTURE_WRAP are GL_CLAMP_TO_EDGE or GL_CLAMP_TO_BORDER
		glTexParameteri(texture->target, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(texture->target, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(texture->target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(texture->target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// Clean up
		glBindTexture(texture->target, 0);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);	//Return to default alignment
//		stbi_image_free(data);

//		return !CheckGLErrors( (string("Loading texture: ")+filename).c_str() );
		return true;

}


// deallocate texture-related objects
void DestroyFBO(MyFBO *texture);
*/
