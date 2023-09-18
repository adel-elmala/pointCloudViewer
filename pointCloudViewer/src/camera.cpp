#include "camera.h"

camera::camera(glm::vec3 eye, glm::vec3 front, glm::vec3 up) : m_cameraPos(eye), m_cameraFront(front), m_cameraUp(up), m_fov(70.0f)
{
    m_view = glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);
}

camera::~camera()
{
}
void camera::update()
{
    m_view = glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);
}