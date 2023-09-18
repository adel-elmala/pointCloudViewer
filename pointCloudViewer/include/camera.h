#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class camera
{
private:
public:
    camera(glm::vec3 eye, glm::vec3 front, glm::vec3 up);
    ~camera();
    void update();
    /* data */
    glm::vec3 m_cameraPos;
    glm::vec3 m_cameraFront;
    glm::vec3 m_cameraUp;
    glm::mat4 m_view;
    float m_fov;
};

#endif