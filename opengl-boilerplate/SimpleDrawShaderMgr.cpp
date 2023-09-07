/*
 * SimpleDrawGlShaderMgr.cpp - Version 1.0, September 12, 2020
 *
 * Example program illustrating a simple use
 * of Modern OpenGL to draw points and straight lines, and
 * to draw overlapping triangles in 3D.
 *
 * Author: Sam Buss
 *
 * Software accompanying POSSIBLE SECOND EDITION TO the book
 *		3D Computer Graphics: A Mathematical Introduction with OpenGL,
 *		by S. Buss, Cambridge University Press, 2003.
 *
 * Software is "as-is" and carries no warranty.  It may be used without
 *   restriction, but if you modify it, please change the filenames to
 *   prevent confusion between different versions.
 * Bug reports: Sam Buss, sbuss@ucsd.edu.
 * Web page: http://math.ucsd.edu/~sbuss/MathCG2
 */

// Use space to toggle what image is shown.
// Use Escape or 'X' or 'x' to exit.

// These libraries are needed to link the program (Visual Studio specific)


// Use the static library (so glew32.dll is not needed):
#define GLEW_STATIC
#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GlShaderMgr.h"
bool check_for_opengl_errors();     // Function prototype (should really go in a header file)

// Enable standard input and output via printf(), etc.
// Put this include *after* the includes for glew and GLFW!
#include <stdio.h>
#include "include\parser.h"
// ********************
// Animation controls and state infornation
// ********************

int CurrentMode = 0;	// Controls what is drawn.

// ************************
// General data helping with setting up VAO (Vertex Array Objects)
//    and Vertex Buffer Objects.
// ***********************

const int NumObjects = 1;
float deltaTime = 0.0f;

unsigned int myVBO[NumObjects];  // Vertex Buffer Object - holds an array of data
unsigned int myVAO[NumObjects];  // Vertex Array Object - holds info about an array of vertex data;

// We create one shader program: it consists of a vertex shader and a fragment shader
unsigned int shaderProgram1;
const unsigned int vertPos_loc = 0;   // Corresponds to "location = 0" in the verter shader definition
const unsigned int vertColor_loc = 1; // Corresponds to "location = 1" in the verter shader definition


bool firstMouse = true;
float yaw   = 90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch =  0.0f;
float lastX =  800.0f / 2.0;
float lastY =  600.0 / 2.0;
float fov   =  45.0f;
bool rightMouseClicked = false;
int window_width = 800;
int window_height = 800;


// camera
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, 1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);

glm::mat4 projection; 
glm::mat4 model; 
glm::mat4 view;

bool window_resized = false;

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);


// *************************
// mySetupGeometries defines the scene data, especially vertex  positions and colors.
//    - It also loads all the data into the VAO's (Vertex Array Objects) and
//      into the VBO's (Vertex Buffer Objects).
// This routine is only called once to initialize the data.
// *************************
void mySetupGeometries() {

	// In this simple example, we do not use the Projection or
	//   ModelView matrices. Hence, all x, y, z positions
	//   should be in the range [-1,1].
	parser p(std::string("C:\\Users\\a.refaat\\projects\\pointCloudViewer\\assets\\dragon.ply"));

    // Allocate Vertex Array Objects (VAOs) and Vertex Buffer Objects (VBOs).
    glGenVertexArrays(NumObjects, &myVAO[0]);
    glGenBuffers(NumObjects, &myVBO[0]);

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
	// glBufferData(GL_ARRAY_BUFFER, sizeof(trianglesVerts), trianglesVerts, GL_STATIC_DRAW);
	// glVertexAttribPointer(vertPos_loc, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
	// glEnableVertexAttribArray(vertPos_loc);
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * (p.m_nVerts), p.m_verticies, GL_STATIC_DRAW);
	glVertexAttribPointer(vertPos_loc,3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)(0));
	glEnableVertexAttribArray(vertPos_loc);

	// glVertexAttribPointer(vertColor_loc, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
	// glEnableVertexAttribArray(vertColor_loc);

	// This is optional, but allowed.  The VAO already knows which buffer (VBO) is holding its
	//     vertex data, so it is OK to unbind the VBO here.
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Also optional, but perhaps a good idea so that the last VAO is not used by accident.
	glBindVertexArray(0);

    check_for_opengl_errors();   // Really a great idea to check for errors -- esp. good for debugging!
}
// *************************************
// Main routine for rendering the scene
// myRenderScene() is called every time the scene needs to be redrawn.
// mySetupGeometries() has already created the vertex and buffer objects, etc.
// setup_shaders() has already created the shader programs.
// *************************************
void myRenderScene() {

	// Clear the rendering window
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// This very simple shader program is used for all the items.
	glUseProgram(shaderProgram1);

	// Draw three points
	glBindVertexArray(myVAO[0]);
	// glVertexAttrib3f(vertColor_loc, 1.0f, 0.5f, 0.2f);		// An orange-red color (R, G, B values).
	glDrawArrays(GL_POINTS, 0, 2000000);
    check_for_opengl_errors();   // Really a great idea to check for errors -- esp. good for debugging!
}

void my_setup_SceneData() {
    mySetupGeometries();

    GlShaderMgr::LoadShaderSource("C:\\Users\\a.refaat\\projects\\pointCloudViewer\\opengl-boilerplate\\SimpleDraw.glsl");

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

// *******************************************************
// Process all key press events.
// This routine is called each time a key is pressed or released.
// *******************************************************
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_RELEASE) {
		return;			// Ignore key up (key release) events
	}
	if (key == GLFW_KEY_ESCAPE || key == GLFW_KEY_X) {
		glfwSetWindowShouldClose(window, true);
	}
	
    float cameraSpeed = static_cast<float>(10000.0 * deltaTime);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}


// *************************************************
// This function is called with the graphics window is first created,
//    and again whenever it is resized.
// The Projection View Matrix is typically set here.
//    But this program does not use any transformations or matrices.
// *************************************************
void window_size_callback(GLFWwindow* window, int width, int height) {
	window_resized = true;
	glViewport(0, 0, width, height);		// Draw into entire window
	window_height = height;
	window_width = width;
	projection = glm::perspective(float(glm::radians(fov)), (float)window_width / (float)window_height, 0.1f, 1000.0f);

}

void my_setup_OpenGL() {
	
	glEnable(GL_DEPTH_TEST);	// Enable depth buffering
	glDepthFunc(GL_LEQUAL);		// Useful for multipass shaders

	// Set polygon drawing mode for front and back of each triangle
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

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
	// glPointSize(8);
	// glLineWidth(5);
	
}

void error_callback(int error, const char* description)
{
	// Print error
	fputs(description, stderr);
}

void setup_callbacks(GLFWwindow* window) {
	// Set callback function for resizing the window
	glfwSetFramebufferSizeCallback(window, window_size_callback);

	// Set callback for key up/down/repeat events
	glfwSetKeyCallback(window, key_callback);

	// Set callbacks for mouse movement (cursor position) and mouse botton up/down events.
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
}

// **********************
// Here is the main program
// **********************

int main() {
	glfwSetErrorCallback(error_callback);	// Supposed to be called in event of errors. (doesn't work?)
	glfwInit();
#if defined(__APPLE__) || defined(__linux__)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	// const int initWidth = 800;
	// const int initHeight = 600;
	GLFWwindow* window = glfwCreateWindow(window_width, window_height, "Point Cloud Viewer", NULL, NULL);
	if (window == NULL) {
		fprintf(stdout, "Failed to create GLFW window!\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);
	glewInit();

	// Print info of GPU and supported OpenGL version
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("OpenGL version supported %s\n", glGetString(GL_VERSION));
#ifdef GL_SHADING_LANGUAGE_VERSION
	printf("Supported GLSL version is %s.\n", (char *)glGetString(GL_SHADING_LANGUAGE_VERSION));
#endif
    printf("Using GLEW version %s.\n", glewGetString(GLEW_VERSION));

	printf("------------------------------\n");
	printf("Press space bar to cycle thru images.\n");
	printf("Press ESCAPE or 'X' or 'x' to exit.\n");
	
    setup_callbacks(window);
    window_size_callback(window, window_width, window_height);

	// Initialize OpenGL, the scene and the shaders
    my_setup_OpenGL();
	my_setup_SceneData();
	double previousTime = glfwGetTime();
	int frameCounter = 0;

	projection = glm::perspective(float(glm::radians(fov)), (float)window_width / (float)window_height, 0.1f, 1000.0f);
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	model = glm::mat4(1.0f);
	
	glUseProgram(shaderProgram1);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram1,"view"),1,GL_FALSE,&view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram1,"proj"),1,GL_FALSE,&projection[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram1,"model"),1,GL_FALSE,&model[0][0]);

    // Loop while program is not terminated.
	while (!glfwWindowShouldClose(window)) {
		double currentTime = glfwGetTime();
		frameCounter++;
		if(frameCounter >= 50)
		{
			frameCounter = 0;
			deltaTime = currentTime - previousTime;
			glfwSetWindowTitle(window, (std::string("FPS: ") + std::to_string(1.0/(deltaTime))).c_str());
		}
		previousTime = currentTime;
		if(window_resized)
		{
			window_resized = false;
			projection = glm::perspective(float(glm::radians(fov)), (float)window_width / (float)window_height, 0.1f, 1000.0f);
			glUniformMatrix4fv(glGetUniformLocation(shaderProgram1,"proj"),1,GL_FALSE,&projection[0][0]);
		}
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram1,"view"),1,GL_FALSE,&view[0][0]);
		myRenderScene();				// Render into the current buffer
		glfwSwapBuffers(window);		// Displays what was just rendered (using double buffering).
		
		// Poll events (key presses, mouse events)
		// glfwWaitEvents();					// Use this if no animation.
		// glfwWaitEventsTimeout(1.0/60.0);	// Use this to animate at 60 frames/sec (timing is NOT reliable)
		glfwPollEvents();				// Use this version when animating as fast as possible
		glUniform1f(glGetUniformLocation(shaderProgram1,"time"),currentTime);
	}

	glfwTerminate();
	return 0;
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
// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
	if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}
	float xoffset = xpos - lastX;
	float yoffset = ypos - lastY; // reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;

	if (rightMouseClicked)
	{
	
		// float xoffset = xpos - (window_width/2);
		// float yoffset = (window_height/2) - ypos; // reversed since y-coordinates go from bottom to top
		float sensitivity = 0.01f; // change this value to your liking
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		yaw += xoffset;
		pitch += yoffset;

		// make sure that when pitch is out of bounds, screen doesn't get flipped
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		glm::vec3 front;
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraFront = glm::normalize(front);
	}
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        rightMouseClicked = true;
	
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
        rightMouseClicked = false;
}