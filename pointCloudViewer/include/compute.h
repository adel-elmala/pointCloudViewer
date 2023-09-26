#ifndef COMPUTE_H
#define COMPUTE_H

// #include "parser.h"
//#include "parser2.h"
#include "myParser.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class compute
{
private:
    /* data */
    //parser2* m_parser;
    myParser *m_parser;
    void loadScene();

public:
    compute(const std::string &scene_path, const std::string &shader_path);
    ~compute();
    void setup();
    void useProgram();
    void dispatch();
    void setUniformMat4(const std::string &name, glm::mat4 &matrix);
    void setUniformVec3(const std::string &name, glm::vec3 &vect);
    void setFloat(const std::string &name, float &value);
    bool check_for_opengl_errors();


    // We create one shader program: it consists of a compute shader
    unsigned int computeProgram;

    glm::mat4 m_projection;
    glm::mat4 m_model;
    glm::mat4 m_view;

    unsigned int posSSbo,posSSbo_output;
    unsigned int colSSbo,colSSbo_output;
    unsigned int num_elms; 
    unsigned int num_col; 
    std::string m_scenePath;
    std::string m_shaderPath;
};

#endif