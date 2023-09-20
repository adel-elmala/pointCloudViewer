#include "renderer.h"
#include "shaderMgr.h"

renderer::renderer(const std::string &shader_path) : m_shaderPath(shader_path)
{
    glewInit();
}

renderer::~renderer()
{
}
void renderer::attach_shaderProg_SSB(unsigned int SBBId)
{
    m_SSBid = SBBId;
}
void renderer::attach_shaderProg_num_elms(unsigned int num_elms)
{
    m_num_elms = num_elms;
}

void renderer::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    useProgram();
    
    glBindBuffer( GL_ARRAY_BUFFER, m_SSBid);
    glVertexPointer( 4, GL_FLOAT, 0, (void *)0 );
    glEnableClientState( GL_VERTEX_ARRAY );
    glDrawArrays( GL_POINTS, 0, m_num_elms);
    glDisableClientState( GL_VERTEX_ARRAY );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    
    check_for_opengl_errors(); // Really a great idea to check for errors -- esp. good for debugging!
}
void renderer::loadScene()
{

    // Allocate Vertex Array Objects (VAOs) and Vertex Buffer Objects (VBOs).

    check_for_opengl_errors(); // Really a great idea to check for errors -- esp. good for debugging!
}

void renderer::setup()
{
    ShaderMgr shdrmgr;

    shdrmgr.addVertexShader((m_shaderPath + "\\vertex.vs").c_str());
    shdrmgr.addFragmentShader((m_shaderPath + "\\fragment.fs").c_str());

    shaderProgram = shdrmgr.ID;

    check_for_opengl_errors(); // Really a great idea to check for errors -- esp. good for debugging!
}

void renderer::useProgram()
{
    glUseProgram(shaderProgram);
}
void renderer::setFloat(const std::string &name, float &value)
{
    glUniform1f(glGetUniformLocation(shaderProgram, name.c_str()), value);
}
void renderer::setUniformMat4(const std::string &name, glm::mat4 &matrix)
{
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, &matrix[0][0]);
}
void renderer::setUniformVec3(const std::string &name, glm::vec3 &vect)
{
    glUniform3fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, &vect[0]);
}
static char errNames[8][36] = {
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