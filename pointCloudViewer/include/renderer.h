#ifndef RENDERER_H
#define RENDERER_H

// #include "parser.h"
#include "parser2.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class renderer
{
private:
    /* data */
    bool check_for_opengl_errors();
    parser2 *m_parser;
    void loadScene();

public:
    renderer(const std::string &shader_path);
    ~renderer();
    void setup();
    void useProgram();
    void render();
    void attach_shaderProg_SSB(unsigned int SSBId_pos,unsigned int SBBId_col);
    void attach_shaderProg_num_elms(unsigned int num_elms,unsigned int num_cols);
    void setUniformMat4(const std::string &name, glm::mat4 &matrix);
    void setUniformVec3(const std::string &name, glm::vec3 &vect);
    void setFloat(const std::string &name, float &value);

    unsigned int myVBO[1]; // Vertex Buffer Object - holds an array of data
    unsigned int myVAO[1]; // Vertex Array Object - holds info about an array of vertex data;

    // We create one shader program: it consists of a vertex shader and a fragment shader
    unsigned int shaderProgram;
    const unsigned int vertPos_loc = 0;   // Corresponds to "location = 0" in the verter shader definition
    const unsigned int vertColor_loc = 1; // Corresponds to "location = 1" in the verter shader definition
    unsigned int m_SSBid_pos;
    unsigned int m_SSBid_col;
    unsigned int m_num_elms;
    unsigned int m_num_cols;
    std::string m_shaderPath;
};

#endif