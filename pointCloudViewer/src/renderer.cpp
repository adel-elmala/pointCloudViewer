#include "renderer.h"
#include "shaderMgr.h"

renderer::renderer(const std::string &scene_path, const std::string &shader_path,bool use_compute_shader) : m_scenePath(scene_path), m_shaderPath(shader_path), m_use_compute_shader(use_compute_shader)
{
    glewInit();
}

renderer::~renderer()
{
    delete m_parser;
}
void renderer::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderProgram);

    glBindVertexArray(myVAO[0]);
    glDrawArrays(GL_POINTS, 0, m_parser->verts.size());
    check_for_opengl_errors(); // Really a great idea to check for errors -- esp. good for debugging!
}
void renderer::loadScene()
{
    m_parser = new parser2();
    m_parser->read_file(m_scenePath);
    
    // Allocate Vertex Array Objects (VAOs) and Vertex Buffer Objects (VBOs).
    glGenVertexArrays(1, &myVAO[0]);
    glGenBuffers(1, &myVBO[0]);

    /*  VVV use this triangle for debugging if needed VVV */
    // float trianglesVerts[] = {
    // 	// x,y,z coordinates	// R,G,B colors
    // 	0.7f, -0.42f, 0.0f,		1.0f, 0.8f, 0.8f, // First triangle
    // 	0.7f, -0.18f, 0.0f,		1.0f, 0.8f, 0.8f,
    // 	-0.7f, -0.3f, 0.5f,		1.0f, 0.0f, 0.0f,
    // 	-0.25f, 0.7f, 0.0f,		0.8f, 1.0f, 0.8f, // Second triangle
    // 	-0.40f, 0.55f, 0.0f,	0.8f, 1.0f, 0.8f,
    // 	0.5f, -0.6f, 0.5f,		0.0f, 1.0f, 0.0f,
    // 	-0.57f, -0.53f, 0.0f,	0.8f,  0.8f, 1.0f,	// Third triangle
    // 	-0.43f, -0.67f, 0.0f,	0.8f,  0.8f, 1.0f,
    // 	0.32f, 0.62f, 0.5f,		0.0f,  0.0f, 1.0f,
    // };

    glBindVertexArray(myVAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, myVBO[0]);
    
    /* buffer the parsed vertices */
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * (m_parser->verts.size()), &(m_parser->verts[0]), GL_STATIC_DRAW);
    glVertexAttribPointer(vertPos_loc, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)(0));
    glEnableVertexAttribArray(vertPos_loc);

    // optional, but perhaps a good idea so that the last VAO is not used by accident.
    glBindVertexArray(0);

    check_for_opengl_errors(); // Really a great idea to check for errors -- esp. good for debugging!
}

void renderer::setup()
{
    loadScene();

    ShaderMgr shdrmgr;
    if(!m_use_compute_shader)
    {
        shdrmgr.addVertexShader((m_shaderPath + "\\vertex.vs").c_str());
        shdrmgr.addFragmentShader((m_shaderPath + "\\fragment.fs").c_str());
    }
    else 
    {
        shdrmgr.addComputeShader((m_shaderPath + "\\compute.comp").c_str());
    }
    shaderProgram = shdrmgr.ID;

    check_for_opengl_errors(); // Really a great idea to check for errors -- esp. good for debugging!
}

void renderer::useProgram()
{
    glUseProgram(shaderProgram);
}
void renderer::setUniformMat4(const std::string &name, glm::mat4 &matrix)
{
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, &matrix[0][0]);
}
void renderer::setUniformVec3(const std::string &name, glm::vec3 &vect)
{
    glUniform3fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, &vect[0]);
}
char errNames[8][36] = {
    "Unknown OpenGL error",
    "GL_INVALID_ENUM", "GL_INVALID_VALUE", "GL_INVALID_OPERATION",
    "GL_INVALID_FRAMEBUFFER_OPERATION", "GL_OUT_OF_MEMORY",
    "GL_STACK_UNDERFLOW", "GL_STACK_OVERFLOW"};
bool renderer::check_for_opengl_errors()
{
    int numErrors = 0;
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR)
    {
        numErrors++;
        int errNum = 0;
        switch (err)
        {
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