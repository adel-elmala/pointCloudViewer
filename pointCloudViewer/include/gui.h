#ifndef GUI_H
#define GUI_H

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#define GL_SILENCE_DEPRECATION
class gui
{
private:
    /* data */
    void * m_window;
    ImGuiIO io;
public:
    gui(void * window);
    ~gui();
    void setup();
    void confg();
    void render();
};




#endif