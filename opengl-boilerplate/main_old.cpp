#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "include/parser.h"
#include "include/GlShaderMgr.h"

using namespace glm;

#define GLSL(src) #src

volatile GLuint myVBO[1];  // Vertex Buffer Object - holds an array of data
volatile GLuint myVAO[1];  // Vertex Array Object - holds info about an array of vertex data;
// GLuint myVBO= 0;  // Vertex Buffer Object - holds an array of data
// unsigned int myVAO = 0;  // Vertex Array Object - holds info about an array of vertex data;

// We create one shader program: it consists of a vertex shader and a fragment shader
unsigned int shaderProgram1;
const unsigned int vertPos_loc = 0;   // Corresponds to "location = 0" in the verter shader definition
const unsigned int vertColor_loc = 1; // Corresponds to "location = 1" in the verter shader definition

void mySetupGeometries();
void my_setup_OpenGL();
void my_setup_SceneData();
bool check_for_opengl_errors();

static void error_callback(int error, const char *description) {
  fputs(description, stderr);
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
}

static void render(GLFWwindow *window) {
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_ACCUM_BUFFER_BIT);
  // This very simple shader program is used for all the items.
	// glUseProgram(shaderProgram1);
  // // Draw three points
	// 	glBindVertexArray(myVAO[0]);
	// 	glVertexAttrib3f(vertColor_loc, 1.0f, 0.5f, 0.2f);		// An orange-red color (R, G, B values).
	// 	glDrawArrays(GL_POINTS, 0, 3);
  //   check_for_opengl_errors();   // Really a great idea to check for errors -- esp. good for debugging!
}

int main(void) {
  GLFWwindow *window;
  glfwSetErrorCallback(error_callback);

  if (!glfwInit())
    exit(EXIT_FAILURE);

   
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  window = glfwCreateWindow(640, 480, "Point Cloud Viewer", NULL, NULL);
  if (!window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glewExperimental = GL_TRUE;
  glewInit();

  glfwMakeContextCurrent(window);

  glfwSetKeyCallback(window, key_callback);
  // parse the model file and extract the verticies from the assimp's data structure.
  parser p(std::string("C:\\Users\\a.refaat\\projects\\pointCloudViewer\\assets\\fragment.ply"));
  my_setup_OpenGL();
  my_setup_SceneData();
  // load the vertecies to the gbu 
  while (!glfwWindowShouldClose(window)) {
    render(window);

    glfwSwapBuffers(window);
    glfwPollEvents();
    //glfwWaitEvents();
  }

  glfwDestroyWindow(window);

  glfwTerminate();
  exit(EXIT_SUCCESS);
}

void mySetupGeometries() {

	// In this simple example, we do not use the Projection or
	//   ModelView matrices. Hence, all x, y, z positions
	//   should be in the range [-1,1].

  // Allocate Vertex Array Objects (VAOs) and Vertex Buffer Objects (VBOs).
  glGenVertexArrays(1,(GLuint*) &myVAO[0]);
  glGenBuffers(1, (GLuint*)&myVBO[0]);

  // THIRD GEOMETRY: Three triangles
	// Specify nine vertices that will be used to form triangles.
	// The vertices are listed in counterclockwise order.
	// Will render with GL_TRIANGLES
	float trianglesVerts[] = {
		// x,y,z coordinates	// R,G,B colors
		0.7f, -0.42f, 0.0f,		1.0f, 0.8f, 0.8f, // First triangle
		0.7f, -0.18f, 0.0f,		1.0f, 0.8f, 0.8f,
		-0.7f, -0.3f, 0.5f,		1.0f, 0.0f, 0.0f,
	
		-0.25f, 0.7f, 0.0f,		0.8f, 1.0f, 0.8f, // Second triangle
		-0.40f, 0.55f, 0.0f,	0.8f, 1.0f, 0.8f,
		0.5f, -0.6f, 0.5f,		0.0f, 1.0f, 0.0f,
	
		-0.57f, -0.53f, 0.0f,	0.8f,  0.8f, 1.0f,	// Third triangle
		-0.43f, -0.67f, 0.0f,	0.8f,  0.8f, 1.0f,
		0.32f, 0.62f, 0.5f,		0.0f,  0.0f, 1.0f,
	};

	// Do similarly to above, now for the three triangles.
    // This time, both vertex positions AND vertex colors are loaded into the VBO.
	// Note that glVertexAttribPointer needs a non-zero "stride" parameter.
	// Also note that the second call to glVertexAttribPointer uses 
	//        3*sizeof(float) to specify where the color information is.
	glBindVertexArray(myVAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, myVBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(trianglesVerts), trianglesVerts, GL_STATIC_DRAW);
	glVertexAttribPointer(vertPos_loc, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
	glEnableVertexAttribArray(vertPos_loc);
	glVertexAttribPointer(vertColor_loc, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(vertColor_loc);

	// This is optional, but allowed.  The VAO already knows which buffer (VBO) is holding its
	//     vertex data, so it is OK to unbind the VBO here.
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Also optional, but perhaps a good idea so that the last VAO is not used by accident.
	glBindVertexArray(0);

    check_for_opengl_errors();   // Really a great idea to check for errors -- esp. good for debugging!
}
void my_setup_OpenGL() {
	
	glEnable(GL_DEPTH_TEST);	// Enable depth buffering
	glDepthFunc(GL_LEQUAL);		// Useful for multipass shaders

	// Set polygon drawing mode for front and back of each triangle
	// glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

	// Disable backface culling to render both sides of triangles
	// glDisable(GL_CULL_FACE);

// TRY IT OUT: How do the results look different if you disable the next block
// of code. (By changing "#if 1" to "#if 0"
#if 1
	// The following commands should induce OpenGL to create round points and 
	//	antialias points and lines.  (This is implementation dependent unfortunately.)
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);	// Make round points, not square points
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);		// Antialias the lines
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#endif

	// Specify the diameter of points, and the width of lines. (Implementation dependent.)
	//   Permissible values are 1 and greater.
	// TRY IT OUT: Experiment with increasing and decreasing these values.
	glPointSize(8);
	glLineWidth(5);
	
}

void my_setup_SceneData() {
    mySetupGeometries();

    GlShaderMgr::LoadShaderSource("SimpleDraw.glsl");

    // Any of the next three options will work for SimpleDrawModern
#if 1
    shaderProgram1 = GlShaderMgr::CompileAndLinkAll();
#elif 0
    shaderProgram1 = GlShaderMgr::CompileAndLinkProgram("vertexShader_PosColorOnly", "fragmentShader_ColorOnly");
#else
    unsigned int vertexShader = GlShaderMgr::CompileShader("vertexShader_PosColorOnly");
    unsigned int fragmentShader = GlShaderMgr::CompileShader("fragmentShader_ColorOnly");
    unsigned int shaders[2] = { vertexShader, fragmentShader };
    shaderProgram1 = GlShaderMgr::LinkShaderProgram(2, shaders);
#endif

    // Free up shader compilation resources that are no longer needed.
    GlShaderMgr::FinalizeCompileAndLink();
     
    check_for_opengl_errors();   // Really a great idea to check for errors -- esp. good for debugging!
}

// If an error is found, it could have been caused by any command since the
//   previous call to check_for_opengl_errors()
// To find what generated the error, you can try adding more calls to
//   check_for_opengl_errors().
char errNames[8][36] = {
    "Unknown OpenGL error",
    "GL_INVALID_ENUM", "GL_INVALID_VALUE", "GL_INVALID_OPERATION",
    "GL_INVALID_FRAMEBUFFER_OPERATION", "GL_OUT_OF_MEMORY",
    "GL_STACK_UNDERFLOW", "GL_STACK_OVERFLOW" };
bool check_for_opengl_errors() {
    int numErrors = 0;
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        numErrors++;
        int errNum = 0;
        switch (err) {
        case GL_INVALID_ENUM:
            errNum = 1;
            break;
        case GL_INVALID_VALUE:
            errNum = 2;
            break;
        case GL_INVALID_OPERATION:
            errNum = 3;
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            errNum = 4;
            break;
        case GL_OUT_OF_MEMORY:
            errNum = 5;
            break;
        case GL_STACK_UNDERFLOW:
            errNum = 6;
            break;
        case GL_STACK_OVERFLOW:
            errNum = 7;
            break;
        }
        printf("OpenGL ERROR: %s.\n", errNames[errNum]);
    }
    return (numErrors != 0);
}