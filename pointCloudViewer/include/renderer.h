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

public:
    renderer(const std::string &scene_path, const std::string &shader_path, bool use_compute_shader = false);
    ~renderer();
    void loadScene();
    void setup();
    void useProgram();
    void render();
    void setUniformMat4(const std::string &name, glm::mat4 &matrix);
    void setUniformVec3(const std::string &name, glm::vec3 &vect);

    unsigned int myVBO[1]; // Vertex Buffer Object - holds an array of data
    unsigned int myVAO[1]; // Vertex Array Object - holds info about an array of vertex data;

    // We create one shader program: it consists of a vertex shader and a fragment shader
    unsigned int shaderProgram;
    const unsigned int vertPos_loc = 0;   // Corresponds to "location = 0" in the verter shader definition
    const unsigned int vertColor_loc = 1; // Corresponds to "location = 1" in the verter shader definition
    bool m_use_compute_shader;

    glm::mat4 m_projection;
    glm::mat4 m_model;
    glm::mat4 m_view;
    std::string m_scenePath;
    std::string m_shaderPath;
};

#endif