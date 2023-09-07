#ifndef WINDOWMGR_H
#define WINDOWMGR_H

#include <string>

#define GLEW_STATIC
#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include "camera.h"
class WindowMgr
{
    public:
    
        WindowMgr(const std::string& win_title ,const unsigned int win_width = 800,const unsigned int win_height = 800);
        ~WindowMgr();
        void setTitle(const std::string& title);
        void attach_camera(camera* camera);
        bool should_close();

    private:
        void setup_callbacks(GLFWwindow* window);
        
       void error_callback(int error, const char* description);
       static void window_size_callback(GLFWwindow* window, int width, int height);
       static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
       static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
       static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
       static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
        
   


    public:
        // data
        GLFWwindow* m_win;
        static unsigned int m_win_height;
        static unsigned int m_win_width;
        static bool m_win_resized;
        static float m_deltaTime;
        static bool m_rightMouseClicked;
        static camera* m_camera;

        std::string m_win_title;
        static bool m_firstMouse;
        static float m_lastX,m_lastY;

        static float m_yaw;	
        static float m_pitch;
};


#endif