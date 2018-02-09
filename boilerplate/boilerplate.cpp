// ==========================================================================
// Barebones OpenGL Core Profile Boilerplate
//    using the GLFW windowing system (http://www.glfw.org)
//
// Loosely based on
//  - Chris Wellons' example (https://github.com/skeeto/opengl-demo) and
//  - Camilla Berglund's example (http://www.glfw.org/docs/latest/quick.html)
//
// Author:  Sonny Chan, University of Calgary
// Co-Authors:
//			Jeremy Hart, University of Calgary
//			John Hall, University of Calgary
// Date:    December 2015
// ==========================================================================

#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <iterator>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "texture.h"
//#include "fbo.h"

using namespace std;
using namespace glm;
// --------------------------------------------------------------------------
// OpenGL utility and support function prototypes

void QueryGLVersion();
bool CheckGLErrors();

string LoadSource(const string &filename);
GLuint CompileShader(GLenum shaderType, const string &source);
GLuint LinkProgram(GLuint vertexShader, GLuint fragmentShader);

// --------------------------------------------------------------------------
// Functions to set up OpenGL shader programs for rendering

// load, compile, and link shaders, returning true if successful
GLuint InitializeShaders()
{
	// load shader source from files
	string vertexSource = LoadSource("shaders/vertex.glsl");
	string fragmentSource = LoadSource("shaders/fragment.glsl");
	if (vertexSource.empty() || fragmentSource.empty()) return false;

	// compile shader source into shader objects
	GLuint vertex = CompileShader(GL_VERTEX_SHADER, vertexSource);
	GLuint fragment = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);

	// link shader program
	GLuint program = LinkProgram(vertex, fragment);

	glDeleteShader(vertex);
	glDeleteShader(fragment);

	// check for OpenGL errors and return false if error occurred
	return program;
}

// --------------------------------------------------------------------------
// Functions to set up OpenGL buffers for storing geometry data

struct Geometry
{
	// OpenGL names for array buffer objects, vertex array object
	GLuint  vertexBuffer;
	GLuint  textureBuffer;
	GLuint  colourBuffer;
	GLuint  vertexArray;
	GLsizei elementCount;
//	GLuint	frameBuffer;

	// initialize object names to zero (OpenGL reserved value)
	Geometry() : vertexBuffer(0), colourBuffer(0), vertexArray(0), elementCount(0)//, frameBuffer(0)
	{}
};
/*
bool InitializeTexGeometry(Geometry *geometry, vec2 *textures, int elementCount){
	glBindVertexArray(geometry->vertexArray);
	const GLuint TEXTURE_INDEX = 2;
	glGenBuffers(1, &geometry->textureBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, geometry->textureBuffer);
	glVertexAttribPointer(
		TEXTURE_INDEX,		//Attribute index 
		2, 					//# of components
		GL_FLOAT, 			//Type of component
		GL_FALSE, 			//Should be normalized?
		sizeof(vec2),		//Stride - can use 0 if tightly packed
		0);					//Offset to first element
	glEnableVertexAttribArray(TEXTURE_INDEX);
	glBindBuffer(GL_ARRAY_BUFFER, geometry->textureBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec2)*geometry->elementCount, textures, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	return !CheckGLErrors();
}
void DestroyTexGeometry(Geometry *geometry) {
	glDeleteBuffers(1, &geometry->textureBuffer);
}*/

bool InitializeVAO(Geometry *geometry){

	const GLuint VERTEX_INDEX = 0;
	const GLuint COLOUR_INDEX = 1;
	const GLuint TEXTURE_INDEX = 2;

	//Generate Vertex Buffer Objects
	// create an array buffer object for storing our vertices
	glGenBuffers(1, &geometry->vertexBuffer);

	// create another one for storing our colours
	glGenBuffers(1, &geometry->colourBuffer);
	
	glGenBuffers(1, &geometry->textureBuffer);
//	glGenFrameBuffers(1, &geometry->frameBuffer);

	//Set up Vertex Array Object
	// create a vertex array object encapsulating all our vertex attributes
	glGenVertexArrays(1, &geometry->vertexArray);
	glBindVertexArray(geometry->vertexArray);

	// associate the position array with the vertex array object
	glBindBuffer(GL_ARRAY_BUFFER, geometry->vertexBuffer);
	glVertexAttribPointer(
		VERTEX_INDEX,		//Attribute index 
		2, 					//# of components
		GL_FLOAT, 			//Type of component
		GL_FALSE, 			//Should be normalized?
		sizeof(vec2),		//Stride - can use 0 if tightly packed
		0);					//Offset to first element
	glEnableVertexAttribArray(VERTEX_INDEX);

	// associate the colour array with the vertex array object
	glBindBuffer(GL_ARRAY_BUFFER, geometry->colourBuffer);
	glVertexAttribPointer(
		COLOUR_INDEX,		//Attribute index 
		3, 					//# of components
		GL_FLOAT, 			//Type of component
		GL_FALSE, 			//Should be normalized?
		sizeof(vec3), 		//Stride - can use 0 if tightly packed
		0);					//Offset to first element
	glEnableVertexAttribArray(COLOUR_INDEX);

	glBindBuffer(GL_ARRAY_BUFFER, geometry->textureBuffer);
	glVertexAttribPointer(
		TEXTURE_INDEX,		//Attribute index 
		2, 					//# of components
		GL_FLOAT, 			//Type of component
		GL_FALSE, 			//Should be normalized?
		sizeof(vec2),		//Stride - can use 0 if tightly packed
		0);					//Offset to first element
	glEnableVertexAttribArray(TEXTURE_INDEX);

//	glBindFramebuffer(GL_FRAMEBUFFER, geometry->frameBuffer);

	// unbind our buffers, resetting to default state
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return !CheckGLErrors();
}

// create buffers and fill with geometry data, returning true if successful
bool LoadGeometry(Geometry *geometry, vec2 *vertices, vec3 *colours, vec2 *textures, int elementCount)
{
	geometry->elementCount = elementCount;

	// create an array buffer object for storing our vertices
	glBindBuffer(GL_ARRAY_BUFFER, geometry->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec2)*geometry->elementCount, vertices, GL_STATIC_DRAW);

	// create another one for storing our colours
	glBindBuffer(GL_ARRAY_BUFFER, geometry->colourBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*geometry->elementCount, colours, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, geometry->textureBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec2)*geometry->elementCount, textures, GL_STATIC_DRAW);
	//Unbind buffer to reset to default state
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// check for OpenGL errors and return false if error occurred
	return !CheckGLErrors();
}

// deallocate geometry-related objects
void DestroyGeometry(Geometry *geometry)
{
	// unbind and destroy our vertex array object and associated buffers
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &geometry->vertexArray);
	glDeleteBuffers(1, &geometry->vertexBuffer);
	glDeleteBuffers(1, &geometry->colourBuffer);
	glDeleteBuffers(1, &geometry->textureBuffer);
}

// --------------------------------------------------------------------------
// Rendering function that draws our scene to the frame buffer

GLuint program;

float corners[8] = {0,0,0,0,0,0,0,0};
char* pics[6] = { "shimakaze.png", "image1-mandrill.png", "image2-uclogo.png",
					"image3-aerial.jpg", "image4-thirsk.jpg", "image5-pattern.png"};
MyTexture texs[6];//, oldText[6];
MyTexture oldText;
MyTexture border;
int pic = 0;
int mode = 0;
int filt = 0;
int gaus = 3;
int limit = 0;
int level = 1;
bool lclickdown = false;
bool rclickdown = false;
double dx = 0, dy = 0, th = 0;
double xclick = 0, yclick = 0;
float scalar = 1.0;
bool rel = false;

void RenderTexture(Geometry *geometry)
{
	// clear screen to a dark grey colour
//	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
//	glClear(GL_COLOR_BUFFER_BIT);

	// bind our shader program and the vertex array object containing our
	// scene geometry, then tell OpenGL to draw our geometry
//	glBindTexture(GL_TEXTURE_2D, tex->textureID);
//	glBindFramebuffer(GL_FRAMEBUFFER, oldText[pic].textureID);
	glBindFramebuffer(GL_FRAMEBUFFER, oldText.textureID);

	
	glUseProgram(program);
	glUniform1i(glGetUniformLocation(program,"mode"), mode);
	glUniform1i(glGetUniformLocation(program,"filt"), filt);
//	glUniform1i(glGetUniformLocation(program,"w"), oldText[pic].width);
	glUniform1i(glGetUniformLocation(program,"w"), oldText.width);
	glUniform1i(glGetUniformLocation(program,"w"), oldText.width);
//	glUniform1i(glGetUniformLocation(program,"h"), oldText[pic].height);
	glUniform1i(glGetUniformLocation(program,"h"), oldText.height);
	glUniform1i(glGetUniformLocation(program,"gSize"), gaus);
	glUniform1i(glGetUniformLocation(program,"level"), level);
	glUniform1i(glGetUniformLocation(program,"hori"), 1);
	glBindVertexArray(geometry->vertexArray);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, oldText.textureID, 0);	
	glDrawArrays(GL_TRIANGLES, 0, 6);
	
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);

// Set "renderedTexture" as our colour attachement #0
/*	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, oldText.textureID, 0);

// Set the list of draw buffers.
	GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, DrawBuffers); // "1" is the size o
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	cout<<"fsadf"<<endl;*/

	// reset state to default (no shader or geometry bound)
	glBindVertexArray(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(0);
	
	// check for an report any OpenGL errors
	CheckGLErrors();
}


void RenderScene(Geometry *geometry, MyTexture *tex, GLuint program)
{
	// clear screen to a dark grey colour
//	if (mode !=2 && filt !=5) {
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// bind our shader program and the vertex array object containing our
	// scene geometry, then tell OpenGL to draw our geometry
//	if (mode == 3) {
//		RenderTexture(geometry);
//		glBindTexture(GL_TEXTURE_2D, oldText[pic].textureID);
//		glBindTexture(GL_TEXTURE_2D, oldText.textureID);
//	}
//	else
	glBindTexture(GL_TEXTURE_2D, tex->textureID);
//	glBindFramebuffer(GL_FRAMEBUFFER, oldText.textureID);
	
	glUseProgram(program);
	glUniform1i(glGetUniformLocation(program,"mode"), mode);
	glUniform1i(glGetUniformLocation(program,"filt"), filt);
	glUniform1i(glGetUniformLocation(program,"w"), tex->width);
	glUniform1i(glGetUniformLocation(program,"h"), tex->height);
	glUniform1i(glGetUniformLocation(program,"gSize"), gaus);
	glUniform1i(glGetUniformLocation(program,"level"), level);
	glUniform1i(glGetUniformLocation(program,"hori"), 0);
	glBindVertexArray(geometry->vertexArray);

	
	glDrawArrays(GL_TRIANGLES, 0, geometry->elementCount);
//	cout << geometry->elementCount << endl;
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// reset state to default (no shader or geometry bound)
	glBindVertexArray(0);
	glUseProgram(0);



	// check for an report any OpenGL errors
	CheckGLErrors();
}

// --------------------------------------------------------------------------
// GLFW callback functions


// reports GLFW errors
void ErrorCallback(int error, const char* description)
{
	cout << "GLFW ERROR " << error << ":" << endl;
	cout << description << endl;
}

// handles keyboard input events
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_1 && action == GLFW_PRESS && mode != 0) {
		mode = 0;
		filt = 0;
		gaus = 3;
	}
	if (key == GLFW_KEY_2 && action == GLFW_PRESS && mode != 1) {
		mode = 1;
		filt = 0;
		gaus = 3;
	}
	if (key == GLFW_KEY_3 && action == GLFW_PRESS && mode != 2) {
		mode = 2;
		filt = 0;
		gaus = 3;
	}
	if (key == GLFW_KEY_4 && action == GLFW_PRESS && mode != 3) {
		mode = 3; 
		filt = 3;
		gaus = 7;
	}
	if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
		if (mode == 0) {
			if (filt == 5) 
				filt = 0;
			else
				filt++;
		}
		if (mode == 1) {
			if (filt == 3) 
				filt = 0;
			else
				filt++;
		}
		if (mode == 2) {
			if (filt == 3) {			
				filt = 0;
			}
			else {
				filt++;
			}
			if (filt == 1) 
				gaus = 3;
			else if (filt == 2) 
				gaus = 5;
			else if (filt == 3) 
				gaus = 7;
			else
				gaus = 0;
		}
		if (mode == 3) {
			level = 1;
			if (filt == 128) {
				filt = 3;
				gaus = 7;
			}
			else {
				filt = filt + 5;
				gaus = gaus + 10;
			}
/*			if (limit == 25) 
				limit = 0;
			else
				limit++;
			gaus2 = 1;*/
//			RenderTexture();
		}
	}
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
		if (mode == 0) {
			if (filt == 0) 
				filt = 5;
			else
				filt--;
		}
		if (mode == 1) {
			if (filt == 0) 
				filt = 3;
			else
				filt--;
		}
		if (mode == 2) {
			if (filt == 0) {
				filt = 3;
			}
			else {
				filt--;
			}
			if (filt == 1) 
				gaus = 3;
			else if (filt == 2) 
				gaus = 5;
			else if (filt == 3) 
				gaus = 7;
			else
				gaus = 0;
		}
		if (mode == 3) {
			level = -1;
			if (filt == 3) {
				filt = 128;
				gaus = 257;
			}
			else {
				filt = filt - 5;
				gaus = gaus - 10;
			}
/*			if (limit == 0) { 
				limit = 25;
			}
			else {
				limit--;
				gaus2 = -1;
			}*/
//			RenderTexture();
		}
	}
	
	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
		if (pic == 5)
			pic = 0;
		else
			pic++;
//		if (mode == 3)
//			if (!InitializeTexture(&oldText, pics[pic], GL_TEXTURE_2D, true));
//				cout << "failed to init oldTexture" << endl;
		float img_h = (float)texs[pic].height/2;
		float img_w = (float)texs[pic].width/2;
		float coordx;
		float coordy;
		if (img_h >= img_w) {
			coordx = (texs[pic].width - img_w)/img_h;
			coordy = (texs[pic].height - img_h)/img_h;
		}
		else {
			coordx = (texs[pic].width - img_w)/img_w;
			coordy = (texs[pic].height - img_h)/img_w;
		}
		corners[0] = -coordx, corners[1] = -coordy,
		corners[2] =  coordx, corners[3] =  coordy,
		corners[4] = -coordx, corners[5] =  coordy,
		corners[6] =  coordx, corners[7] = -coordy;
		th=0;
		scalar=1;
		filt=0;
	}
	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
		if (pic == 0)
			pic = 5;
		else
			pic--;
//		if (mode == 3)
//			if (!InitializeTexture(&oldText, pics[pic], GL_TEXTURE_2D, true));
//				cout << "failed to init oldTexture" << endl;
		float img_h = (float)texs[pic].height/2;
		float img_w = (float)texs[pic].width/2;
		float coordx;
		float coordy;
		if (img_h >= img_w) {
			coordx = (texs[pic].width - img_w)/img_h;
			coordy = (texs[pic].height - img_h)/img_h;
		}
		else {
			coordx = (texs[pic].width - img_w)/img_w;
			coordy = (texs[pic].height - img_h)/img_w;
		}
		corners[0] = -coordx, corners[1] = -coordy,
		corners[2] =  coordx, corners[3] =  coordy,
		corners[4] = -coordx, corners[5] =  coordy,
		corners[6] =  coordx, corners[7] = -coordy;
		th=0;
		scalar=1;
		filt=0;
	}
//	if (key == GLFW_KEY_UP && action == GLFW_PRESS)
//		if (mode == 3)
//			rel = true;
//	cout << filt << "-" << gaus << endl; 
}


void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
	cout.precision(8);
	double x_raw, y_raw, x_ogl, y_ogl, dx0,dy0;
	if (lclickdown) {	
		glfwGetCursorPos(window, &x_raw, &y_raw);
		x_ogl = (-256+x_raw)/256;
		y_ogl = (256-y_raw)/256;
		dx0 = x_ogl-xclick;
		dy0 = y_ogl-yclick;
		dx = (cos(-th)*(dx0)-sin(-th)*(dy0));
		dy = (sin(-th)*(dx0)+cos(-th)*(dy0));
	}	
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	double x, y;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{	
		glfwGetCursorPos(window, &x, &y);
		lclickdown = true;
		xclick = (-256+x)/256;
		yclick = (256-y)/256;
		dx = 0;
		dy = 0;
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		lclickdown = false;
		for (int i=0; i<8; i++) {
			if (i%2 == 0)
				corners[i] = corners[i]*scalar+dx;
			else
				corners[i] = corners[i]*scalar+dy;
		}
		dx=0;
		dy=0;
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		glfwGetCursorPos(window, &x, &y);	
		rclickdown = true;
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
	{
		rclickdown = false;
		for (int i=0; i<8; i++) {
			if (i%2 == 0)
				corners[i] = corners[i]*scalar+dx;
			else
				corners[i] = corners[i]*scalar+dy;
		}
	}
}
double scaleLimit = 1;
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	double temp;
	if (rclickdown) {
		th = th + yoffset*(M_PI/60);
	}
	else {
		temp= scaleLimit + yoffset*0.025;
		if (temp >= 0) {
			scaleLimit = temp;
			scalar = pow(1.05,yoffset);
			for (int i=0; i<8; i++) 
				corners[i] = corners[i]*scalar;
		}
		else
			scalar = 1;
	}
	scalar = 1;
}



// ==========================================================================
// PROGRAM ENTRY POINT



int main(int argc, char *argv[])
{
	// initialize the GLFW windowing system
	if (!glfwInit()) {
		cout << "ERROR: GLFW failed to initialize, TERMINATING" << endl;
		return -1;
	}
	glfwSetErrorCallback(ErrorCallback);

	// attempt to create a window with an OpenGL 4.1 core profile context
	GLFWwindow *window = 0;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	int width = 512, height = 512;
	window = glfwCreateWindow(width, height, "CPSC 453 OpenGL Boilerplate", 0, 0);
	if (!window) {
		cout << "Program failed to create GLFW window, TERMINATING" << endl;
		glfwTerminate();
		return -1;
	}

	// set keyboard callback function and make our context current (active)
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwMakeContextCurrent(window);
	
	glfwSetCursorPos(window, 256,256);

	//Intialize GLAD
	if (!gladLoadGL())
	{
		cout << "GLAD init failed" << endl;
		return -1;
	}

	// query and print out information about our OpenGL environment
	QueryGLVersion();

	// call function to load and compile shader programs
	program = InitializeShaders();
	if (program == 0) {
		cout << "Program could not initialize shaders, TERMINATING" << endl;
		return -1;
	}
//	GLuint fbName = 0;
//	oldText.textureID = fbName;
//	glGenFramebuffers(1, &fbName);
//	glBindFramebuffer(GL_FRAMEBUFFER, fbName);
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);


//	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, oldText.textureID, 0);
//	GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
//	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
//	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
//		cout << "dfddf" <<endl;

	for (int i=0; i<6; i++) {
		if (!InitializeTexture(&texs[i], pics[i], GL_TEXTURE_2D))
			cout << "Program failed to initialize texture" << endl;
//		if (!InitializeTexture(&oldText[i], pics[i], GL_TEXTURE_2D, true))
//			cout << "Program failed to initialize texture" << endl;
		//		else cout<<"good"<<endl;
	}

	if (!InitializeTexture(&border, "blood2.png", GL_TEXTURE_2D))
			cout << "Program failed to initialize texture" << endl;

/*	if (!InitializeFBO(&oldText, GL_TEXTURE_2D))
		cout << "Program failed to initialize rendered texture" << endl;
	else
		cout << "good fbo" << endl;
*/

	float img_h = (float)texs[pic].height/2;
	float img_w = (float)texs[pic].width/2;
	float coordx = (texs[pic].width - img_w)/img_h;
	float coordy = (texs[pic].height - img_h)/img_h;
	corners[0] = -coordx, corners[1] = -coordy,
	corners[2] =  coordx, corners[3] =  coordy,
	corners[4] = -coordx, corners[5] =  coordy,
	corners[6] =  coordx, corners[7] = -coordy;
	// three vertex positions and assocated colours of a triangle
	vec2 vertices[] = {
		vec2( corners[0], corners[1] ),
		vec2( corners[2], corners[3] ),
		vec2( corners[4], corners[5] ),
		vec2( corners[0], corners[1] ),
		vec2( corners[2], corners[3] ),
		vec2( corners[6], corners[7] )
	};
	vec3 colours[] = {
		vec3( 1.0f, 0.0f, 0.0f ),
		vec3( 0.0f, 1.0f, 0.0f ),
		vec3( 0.0f, 0.0f, 1.0f ),
		vec3( 1.0f, 0.0f, 0.0f ),
		vec3( 0.0f, 1.0f, 0.0f ),
		vec3( 0.0f, 0.0f, 1.0f )
	};
	vec2 textures[] = {
		vec2( .0f, .0f ),
		vec2( 1.0f,  1.0f ),
		vec2( .0f, 1.0f ),
		vec2( .0f, .0f ),
		vec2( 1.0f,  1.0f ),
		vec2( 1.0f, .0f )
	};

	// call function to create and fill buffers with geometry data
	Geometry geometry;
	if (!InitializeVAO(&geometry))
		cout << "Program failed to intialize geometry!" << endl;

	if(!LoadGeometry(&geometry, vertices, colours, textures, 6))
		cout << "Failed to load geometry" << endl;

	glUseProgram(program);

	glUniform1i(glGetUniformLocation(program, "ourTexture"), 0);
	glUniform1i(glGetUniformLocation(program, "borderTexture"), 1);
	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, texs[pic].textureID);
	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, border.textureID);
	
	while (!glfwWindowShouldClose(window))
	{
//		GLuint fbName = 0;
//		oldText[pic].textureID = fbName;
//		oldText.textureID = fbName;
//		glGenFramebuffers(1, &fbName);
//		glBindFramebuffer(GL_FRAMEBUFFER, fbName);
//		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
	vertices[0] = vec2( cos(th)*(corners[0]*scalar+dx)-sin(th)*(corners[1]*scalar+dy), sin(th)*(corners[0]*scalar+dx)+cos(th)*(corners[1]*scalar+dy) );
	vertices[1] = vec2( cos(th)*(corners[2]*scalar+dx)-sin(th)*(corners[3]*scalar+dy), sin(th)*(corners[2]*scalar+dx)+cos(th)*(corners[3]*scalar+dy) );
	vertices[2] = vec2( cos(th)*(corners[4]*scalar+dx)-sin(th)*(corners[5]*scalar+dy), sin(th)*(corners[4]*scalar+dx)+cos(th)*(corners[5]*scalar+dy) );
	vertices[3] = vec2( cos(th)*(corners[0]*scalar+dx)-sin(th)*(corners[1]*scalar+dy), sin(th)*(corners[0]*scalar+dx)+cos(th)*(corners[1]*scalar+dy) );
	vertices[4] = vec2( cos(th)*(corners[2]*scalar+dx)-sin(th)*(corners[3]*scalar+dy), sin(th)*(corners[2]*scalar+dx)+cos(th)*(corners[3]*scalar+dy) );
	vertices[5] = vec2( cos(th)*(corners[6]*scalar+dx)-sin(th)*(corners[7]*scalar+dy), sin(th)*(corners[6]*scalar+dx)+cos(th)*(corners[7]*scalar+dy) );
	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, texs[pic].textureID);		
	if(!LoadGeometry(&geometry, vertices, colours, textures, 6))
		cout << "Failed to load geometry" << endl;

	// call function to draw our scene
//		if (mode == 3)
//			RenderTexture();
		

//		RenderScene(&geometry, &currentText, program);
		//if (rel) {
//			RenderTexture(&geometry);
//			cout << "click" << endl;
//		}
		RenderScene(&geometry, &texs[pic], program);
//		if (mode ==2 && filt ==5) 
//			RenderScene(&geometry, &border, program);
		glfwSwapBuffers(window);

		glfwPollEvents();

//		rel = false;
	}

	// clean up allocated resources before exit
	DestroyGeometry(&geometry);
//	DestroyTexGeometry(&geometry);
	glUseProgram(0);
	glDeleteProgram(program);
	glfwDestroyWindow(window);
	glfwTerminate();

	cout << "Goodbye!" << endl;
	return 0;
}

// ==========================================================================
// SUPPORT FUNCTION DEFINITIONS

// --------------------------------------------------------------------------
// OpenGL utility functions

void QueryGLVersion()
{
	// query opengl version and renderer information
	string version = reinterpret_cast<const char *>(glGetString(GL_VERSION));
	string glslver = reinterpret_cast<const char *>(glGetString(GL_SHADING_LANGUAGE_VERSION));
	string renderer = reinterpret_cast<const char *>(glGetString(GL_RENDERER));

	cout << "OpenGL [ " << version << " ] "
		<< "with GLSL [ " << glslver << " ] "
		<< "on renderer [ " << renderer << " ]" << endl;
}

bool CheckGLErrors()
{
	bool error = false;
	for (GLenum flag = glGetError(); flag != GL_NO_ERROR; flag = glGetError())
	{
		cout << "OpenGL ERROR:  ";
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

// --------------------------------------------------------------------------
// OpenGL shader support functions

// reads a text file with the given name into a string
string LoadSource(const string &filename)
{
	string source;

	ifstream input(filename.c_str());
	if (input) {
		copy(istreambuf_iterator<char>(input),
			istreambuf_iterator<char>(),
			back_inserter(source));
		input.close();
	}
	else {
		cout << "ERROR: Could not load shader source from file "
			<< filename << endl;
	}

	return source;
}

// creates and returns a shader object compiled from the given source
GLuint CompileShader(GLenum shaderType, const string &source)
{
	// allocate shader object name
	GLuint shaderObject = glCreateShader(shaderType);

	// try compiling the source as a shader of the given type
	const GLchar *source_ptr = source.c_str();
	glShaderSource(shaderObject, 1, &source_ptr, 0);
	glCompileShader(shaderObject);

	// retrieve compile status
	GLint status;
	glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint length;
		glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &length);
		string info(length, ' ');
		glGetShaderInfoLog(shaderObject, info.length(), &length, &info[0]);
		cout << "ERROR compiling shader:" << endl << endl;
		cout << source << endl;
		cout << info << endl;
	}

	return shaderObject;
}

// creates and returns a program object linked from vertex and fragment shaders
GLuint LinkProgram(GLuint vertexShader, GLuint fragmentShader)
{
	// allocate program object name
	GLuint programObject = glCreateProgram();

	// attach provided shader objects to this program
	if (vertexShader)   glAttachShader(programObject, vertexShader);
	if (fragmentShader) glAttachShader(programObject, fragmentShader);

	// try linking the program with given attachments
	glLinkProgram(programObject);

	// retrieve link status
	GLint status;
	glGetProgramiv(programObject, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint length;
		glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &length);
		string info(length, ' ');
		glGetProgramInfoLog(programObject, info.length(), &length, &info[0]);
		cout << "ERROR linking shader program:" << endl;
		cout << info << endl;
	}

	return programObject;
}
