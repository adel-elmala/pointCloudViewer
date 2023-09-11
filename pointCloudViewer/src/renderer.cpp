#include "renderer.h"
// #include "GlShaderMgr.h"
#include "shaderMgr.h"

renderer::renderer(const std::string& scene_path,const std::string& shader_path):m_scenePath(scene_path),m_shaderPath(shader_path)
{
    m_parser = new parser(scene_path);
    glewInit();
}

renderer::~renderer()
{
    delete m_parser;


}
void renderer::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// This very simple shader program is used for all the items.
	glUseProgram(shaderProgram);

	// Draw three points
	glBindVertexArray(myVAO[0]);
	// glVertexAttrib3f(vertColor_loc, 1.0f, 0.5f, 0.2f);		// An orange-red color (R, G, B values).
	glDrawArrays(GL_POINTS, 0, m_parser->m_nVerts);
    check_for_opengl_errors();   // Really a great idea to check for errors -- esp. good for debugging!
}
void renderer::loadScene()
{
    // Allocate Vertex Array Objects (VAOs) and Vertex Buffer Objects (VBOs).
    glGenVertexArrays(1, &myVAO[0]);
    glGenBuffers(1, &myVBO[0]);

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
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * (m_parser->m_nVerts), m_parser->m_verticies, GL_STATIC_DRAW);
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

void renderer::setup()
{
    loadScene();

    // GlShaderMgr::LoadShaderSource(m_shaderPath.c_str());
    ShaderMgr shdrmgr;
    shdrmgr.addVertexShader("C:\\Users\\a.refaat\\projects\\pointCloudViewer\\pointCloudViewer\\shaders\\vertex.vs");
    shdrmgr.addFragmentShader("C:\\Users\\a.refaat\\projects\\pointCloudViewer\\pointCloudViewer\\shaders\\fragment.fs");
    // shdrmgr.addComputeShader("cshader.cs");

    shaderProgram = shdrmgr.ID;
//     // Any of the next three options will work for SimpleDrawModern
// #if 1
//     shaderProgram = GlShaderMgr::CompileAndLinkAll();
// #elif 0
//     shaderProgram = GlShaderMgr::CompileAndLinkProgram("vertexShader_PosColorOnly", "fragmentShader_ColorOnly");
// #else
//     unsigned int vertexShader = GlShaderMgr::CompileShader("vertexShader_PosColorOnly");
//     unsigned int fragmentShader = GlShaderMgr::CompileShader("fragmentShader_ColorOnly");
//     unsigned int shaders[2] = { vertexShader, fragmentShader };
//     shaderProgram = GlShaderMgr::LinkShaderProgram(2, shaders);
// #endif

    // Free up shader compilation resources that are no longer needed.
    // GlShaderMgr::FinalizeCompileAndLink();
     
    check_for_opengl_errors();   // Really a great idea to check for errors -- esp. good for debugging!
}

void renderer::useProgram()
{
    glUseProgram(shaderProgram);
}
void renderer::setUniformMat4(const std::string& name,glm::mat4& matrix)
{
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram,name.c_str()),1,GL_FALSE,&matrix[0][0]);

}
char errNames[8][36] = {
    "Unknown OpenGL error",
    "GL_INVALID_ENUM", "GL_INVALID_VALUE", "GL_INVALID_OPERATION",
    "GL_INVALID_FRAMEBUFFER_OPERATION", "GL_OUT_OF_MEMORY",
    "GL_STACK_UNDERFLOW", "GL_STACK_OVERFLOW" };
bool renderer::check_for_opengl_errors() {
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