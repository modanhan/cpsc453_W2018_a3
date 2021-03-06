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

MyTexture::MyTexture() : textureID(0), target(0), width(0), height(0)
	{}


bool InitializeTexture(MyTexture* texture, const char* filename, GLuint target)
{
	int numComponents;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load(filename, &texture->width, &texture->height, &numComponents, 0);
	if (data != 0)
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);		//Set alignment to be 1

		texture->target = target;
		glGenTextures(1, &texture->textureID);
		glBindTexture(texture->target, texture->textureID);
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
		glTexImage2D(texture->target, 0, format, texture->width, texture->height, 0, format, GL_UNSIGNED_BYTE, data);

		// Note: Only wrapping modes supported for GL_TEXTURE_RECTANGLE when defining
		// GL_TEXTURE_WRAP are GL_CLAMP_TO_EDGE or GL_CLAMP_TO_BORDER
		glTexParameteri(texture->target, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(texture->target, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(texture->target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(texture->target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Clean up
		glBindTexture(texture->target, 0);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);	//Return to default alignment
		stbi_image_free(data);



		return !CheckGLErrors( (string("Loading texture: ")+filename).c_str() );
	}
	return true; //error
}

bool InitializeFBO(MyTexture* texture, GLuint target)
{
//	int numComponents;
//	texture->textureID = fbName;
	glGenFramebuffers(1, &texture->fboID);
	glBindFramebuffer(GL_FRAMEBUFFER, texture->fboID);
	glGenTextures(1, &texture->textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, texture->textureID);
texture->width=480;
texture->height=1000;
	// Give an empty image to OpenGL ( the last "0" )
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, 512, 512, 0,GL_RGB, GL_UNSIGNED_BYTE, 0);

	// Poor filtering. Needed !
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
//	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->textureID, 0);
//	GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
//	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
//	glBindTexture(GL_TEXTURE_2D, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return false;
//		return !CheckGLErrors( (string("Loading texture: ")+filename).c_str() );
//	}
	return !CheckGLErrors( ("fbo error") ); //error
}


// deallocate texture-related objects
void DestroyTexture(MyTexture *texture)
{
	glBindTexture(texture->target, 0);
	glDeleteTextures(1, &texture->textureID);
}

