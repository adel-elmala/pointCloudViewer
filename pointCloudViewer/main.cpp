
// Enable standard input and output via printf(), etc.
// Put this include *after* the includes for glew and GLFW!
#include <stdio.h>

#include "windowMgr.h"
#include "renderer.h"
#include "camera.h"



int main(int argc, char const *argv[])
{
    /* code */
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    camera cam(cameraPos, cameraFront, cameraUp);

    WindowMgr win("Point Cloud Viewer",800,800);

    win.attach_camera(&cam);

    renderer rndr("C:\\Users\\a.refaat\\projects\\pointCloudViewer\\assets\\dragon.ply", "C:\\Users\\a.refaat\\projects\\pointCloudViewer\\opengl-boilerplate\\SimpleDraw.glsl");
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
    while (!win.should_close())
    {

        double currentTime = glfwGetTime();
		frameCounter++;
		if(frameCounter >= 50)
		{
			frameCounter = 0;
			win.m_deltaTime = currentTime - previousTime;
            win.setTitle((std::string("FPS: ") + std::to_string(1.0/(win.m_deltaTime))).c_str());
		}
        previousTime = currentTime;

		if(win.m_win_resized)
		{
			win.m_win_resized = false;
			projection = glm::perspective(float(glm::radians(cam.m_fov)), (float)win.m_win_width / (float)win.m_win_height, 0.1f, 1000.0f);
            rndr.setUniformMat4("proj",projection);
		}
        cam.update();
        view = cam.m_view;
		glUniformMatrix4fv(glGetUniformLocation(rndr.shaderProgram,"view"),1,GL_FALSE,&view[0][0]);


        rndr.render();
        glfwSwapBuffers(win.m_win);		// Displays what was just rendered (using double buffering).
		
		// Poll events (key presses, mouse events)
		// glfwWaitEvents();					// Use this if no animation.
		// glfwWaitEventsTimeout(1.0/60.0);	// Use this to animate at 60 frames/sec (timing is NOT reliable)
		glfwPollEvents();				// Use this version when animating as fast as possible
    }


    return 0;
}







