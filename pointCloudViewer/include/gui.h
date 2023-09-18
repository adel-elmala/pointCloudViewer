#ifndef GUI_H
#define GUI_H

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#define GL_SILENCE_DEPRECATION

#include <glm/glm.hpp>
#include "windowMgr.h"
#include "renderer.h"

// class WindowMgr;
class gui
{
private:
    /* data */

public:
    gui(WindowMgr *window_mgr);
    ~gui();
    void setup();
    void confg();
    void render();
    void attatch_renderer(renderer *rndrer);

    ImGuiIO io;
    WindowMgr *m_winMgr;
    renderer *m_renderer;
    
    ImVec4 m_color;
    float m_cameraSpeed;
};

#endif