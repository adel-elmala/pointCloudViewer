
#include <stdio.h>

#include "windowMgr.h"
#include "renderer.h"
#include "camera.h"
#include "gui.h"
#include "parser2.h"

void setTexture();
void renderQuad();

#if 0
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
    std::string shader_path = "C:\\Users\\a.refaat\\projects\\pointCloudViewer\\\pointCloudViewer\\shaders\\nonCompute";

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
#endif
#if 1
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
    std::string shader_path = "C:\\Users\\a.refaat\\projects\\pointCloudViewer\\\pointCloudViewer\\shaders\\compute";

    /* load shaders and buffer the data to opengl */

    // test the compute shader stuff here
    renderer compRender(std::string(""), shader_path, true);
    compRender.setup();
    
    renderer screenQuad(std::string(""), shader_path);
    screenQuad.setup();


    /* set the shader uniforms variables */

    /* attatch the gui bacj to the window so it can dispatch events back to the gui*/

    int frameCounter = 0;
    double previousTime = glfwGetTime();
    double accumDelta = 0;
    setTexture();
    while (!win.should_close())
    {

        /* calculate the frame rate */
        // TODO: delete the fps calculations as it's already calculated by the gui.
        float currentTime = glfwGetTime();
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
        /* -------------------- */
        const unsigned int TEXTURE_WIDTH = 1000, TEXTURE_HEIGHT = 1000;

        compRender.useProgram();
        compRender.setFloat("t", currentTime);
        glDispatchCompute((unsigned int)TEXTURE_WIDTH / 10, (unsigned int)TEXTURE_HEIGHT / 10, 1);

        // make sure writing to image has finished before read
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

        // render image to quad
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        screenQuad.useProgram();
        renderQuad();

        /* -------------------- */

        glfwSwapBuffers(win.m_win); // Displays what was just rendered (using double buffering).

        /* Poll events (key presses, mouse events) */
        glfwPollEvents(); // Use this version when animating as fast as possible
    }

    return 0;
}
unsigned int texture;
void setTexture()
{
    // Create texture for opengl operation
    // -----------------------------------
    const unsigned int TEXTURE_WIDTH = 1000, TEXTURE_HEIGHT = 1000;

    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);

    glBindImageTexture(0, texture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
}

unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
    if (quadVAO == 0)
    {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,
            1.0f,
            0.0f,
            0.0f,
            1.0f,
            -1.0f,
            -1.0f,
            0.0f,
            0.0f,
            0.0f,
            1.0f,
            1.0f,
            0.0f,
            1.0f,
            1.0f,
            1.0f,
            -1.0f,
            0.0f,
            1.0f,
            0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

#endif