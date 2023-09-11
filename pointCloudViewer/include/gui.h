#ifndef GUI_H
#define GUI_H

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#define GL_SILENCE_DEPRECATION

#include "windowMgr.h"
//class WindowMgr;
class gui
{
private:
    /* data */
    // void * m_window;

public:
    gui(WindowMgr * window_mgr);
    ~gui();
    void setup();
    void confg();
    void render();
    ImGuiIO io;
    WindowMgr *m_winMgr;

};




#endif