
#include <stdio.h>

#include "windowMgr.h"
#include "renderer.h"
#include "camera.h"
#include "gui.h"
#include "parser2.h"

int main(int argc, char const *argv[])
{
    /* set up the camera eye and pose */
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    camera cam(cameraPos, cameraFront, cameraUp);

    /* init the window */
    WindowMgr win("Point Cloud Viewer", 800, 800);

    win.attach_camera(&cam);
    std::string model_path = "C:\\Users\\a.refaat\\projects\\pointCloudViewer\\assets\\dragon.ply";
    std::string shader_path = "C:\\Users\\a.refaat\\projects\\pointCloudViewer\\\pointCloudViewer\\shaders";

    /* load shaders and buffer the data to opengl */
    renderer rndr(model_path, shader_path);

    rndr.setup();
   
    glm::mat4 projection = glm::perspective(float(glm::radians(cam.m_fov)), (float)win.m_win_width / (float)win.m_win_height, 0.1f, 1000.0f);
    glm::mat4 view = cam.m_view;
    glm::mat4 model = glm::mat4(1.0f);

    rndr.useProgram();
    /* set the shader uniforms variables */
    rndr.setUniformMat4("view", view);
    rndr.setUniformMat4("proj", projection);
    rndr.setUniformMat4("model", model);
    
    /* init the gui manger and attacht other modules it needs to operate*/
    gui dearGui(&win);
    dearGui.attatch_renderer(&rndr);
    dearGui.setup();
    /* attatch the gui bacj to the window so it can dispatch events back to the gui*/
    win.attach_gui(&dearGui);

    int frameCounter = 0;
    double previousTime = glfwGetTime();
    double accumDelta = 0;

    while (!win.should_close())
    {

        /* calculate the frame rate */
        // TODO: delete the fps calculations as it's already calculated by the gui.
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
        /* update the projection matrix when the window is resized */
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

        /* Poll events (key presses, mouse events) */ 
        glfwPollEvents(); // Use this version when animating as fast as possible
    }

    return 0;
}
