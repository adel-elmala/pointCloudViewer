
// Enable standard input and output via printf(), etc.
// Put this include *after* the includes for glew and GLFW!
#include <stdio.h>

#include "windowMgr.h"
#include "renderer.h"
#include "camera.h"
#include "gui.h"
#include "parser2.h"

int main(int argc, char const *argv[])
{
    /* code */
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    camera cam(cameraPos, cameraFront, cameraUp);

    WindowMgr win("Point Cloud Viewer", 800, 800);

    win.attach_camera(&cam);
    std::string model_path = "C:\\Users\\a.refaat\\projects\\pointCloudViewer\\assets\\dragon.ply";
    std::string shader_path = "C:\\Users\\a.refaat\\projects\\pointCloudViewer\\\pointCloudViewer\\shaders";

    renderer rndr(model_path, shader_path);
    rndr.setup();
   
    glm::mat4 projection = glm::perspective(float(glm::radians(cam.m_fov)), (float)win.m_win_width / (float)win.m_win_height, 0.1f, 1000.0f);
    glm::mat4 view = cam.m_view;
    glm::mat4 model = glm::mat4(1.0f);

    rndr.useProgram();

    rndr.setUniformMat4("view", view);
    rndr.setUniformMat4("proj", projection);
    rndr.setUniformMat4("model", model);
    
    int frameCounter = 0;
    double previousTime = glfwGetTime();

    gui dearGui(&win);
    dearGui.attatch_renderer(&rndr);
    dearGui.setup();

    win.attach_gui(&dearGui);
    double accumDelta = 0;
    while (!win.should_close())
    {

        double currentTime = glfwGetTime();
        frameCounter++;
        win.m_deltaTime = currentTime - previousTime;
        accumDelta += win.m_deltaTime;
        if (frameCounter >= 50)
        {
            accumDelta /= 50.0;
            win.setTitle((std::string("FPS: ") + std::to_string(1.0 / (accumDelta))).c_str());
            frameCounter = 0;
            accumDelta = 0;
        }
        previousTime = currentTime;

        if (win.m_win_resized)
        {
            win.m_win_resized = false;
            projection = glm::perspective(float(glm::radians(cam.m_fov)), (float)win.m_win_width / (float)win.m_win_height, 0.1f, 1000.0f);
            rndr.setUniformMat4("proj", projection);
        }
        cam.update();
        view = cam.m_view;
        rndr.setUniformMat4("view", view);

        rndr.render();
        dearGui.confg();
        dearGui.render();
        glfwSwapBuffers(win.m_win); // Displays what was just rendered (using double buffering).

        // Poll events (key presses, mouse events)
        // glfwWaitEvents();					// Use this if no animation.
        // glfwWaitEventsTimeout(1.0/60.0);	// Use this to animate at 60 frames/sec (timing is NOT reliable)
        glfwPollEvents(); // Use this version when animating as fast as possible
    }

    return 0;
}
