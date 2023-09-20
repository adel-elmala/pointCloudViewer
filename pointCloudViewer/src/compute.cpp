#include "compute.h"
#include "shaderMgr.h"

compute::compute(const std::string &scene_path, const std::string &shader_path) : m_scenePath(scene_path), m_shaderPath(shader_path),m_parser(nullptr)
{
    glewInit();
}

compute::~compute()
{
    if (m_parser != nullptr)
        delete m_parser;
}
void compute::dispatch()
{
    check_for_opengl_errors(); // Really a great idea to check for errors -- esp. good for debugging!

    glDispatchCompute(num_elms / 10, 1, 1);
    // make sure writing to the SSB has finished before read
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    check_for_opengl_errors(); // Really a great idea to check for errors -- esp. good for debugging!
}
void compute::loadScene()
{
    /* short circuit if the model is not fed to the renderer*/
    if (m_scenePath.empty())
        return;
    m_parser = new parser2();
    m_parser->read_file(m_scenePath);

    typedef struct vertex
    {
        float x, y, z, w;
    } vertex_t;

    num_elms = m_parser->verts.size();
    unsigned int buffer_size = sizeof(float) * 4 * num_elms;

    glGenBuffers(1, &posSSbo_output);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, posSSbo_output);
    glBufferData(GL_SHADER_STORAGE_BUFFER, buffer_size, NULL, GL_STATIC_DRAW);

    glGenBuffers(1, &posSSbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, posSSbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, buffer_size, NULL, GL_STATIC_DRAW);
    GLint bufMask = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT; // the invalidate makes a big difference when re-writing

    vertex_t *points = (vertex_t *)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, buffer_size, bufMask);
    for (int i = 0; i < num_elms; i++)
    {
        points[i].x = m_parser->verts[i].x;
        points[i].y = m_parser->verts[i].y;
        points[i].z = m_parser->verts[i].z;
        points[i].w = 1;
    }
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, posSSbo);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, posSSbo_output);
    check_for_opengl_errors(); // Really a great idea to check for errors -- esp. good for debugging!
}

void compute::setup()
{
    loadScene();

    ShaderMgr shdrmgr;
    
    shdrmgr.addComputeShader((m_shaderPath + "\\compute.comp").c_str());
    
    computeProgram = shdrmgr.ID;

    check_for_opengl_errors(); // Really a great idea to check for errors -- esp. good for debugging!
}

void compute::useProgram()
{
    glUseProgram(computeProgram);
}
void compute::setFloat(const std::string &name, float &value)
{
    glUniform1f(glGetUniformLocation(computeProgram, name.c_str()), value);
}
void compute::setUniformMat4(const std::string &name, glm::mat4 &matrix)
{
    glUniformMatrix4fv(glGetUniformLocation(computeProgram, name.c_str()), 1, GL_FALSE, &matrix[0][0]);
}
void compute::setUniformVec3(const std::string &name, glm::vec3 &vect)
{
    glUniform3fv(glGetUniformLocation(computeProgram, name.c_str()), 1, &vect[0]);
}
static char errNames[8][36] = {
    "Unknown OpenGL error",
    "GL_INVALID_ENUM", "GL_INVALID_VALUE", "GL_INVALID_OPERATION",
    "GL_INVALID_FRAMEBUFFER_OPERATION", "GL_OUT_OF_MEMORY",
    "GL_STACK_UNDERFLOW", "GL_STACK_OVERFLOW"};
bool compute::check_for_opengl_errors()
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