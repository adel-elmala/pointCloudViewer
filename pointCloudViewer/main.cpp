
#include <stdio.h>

#include "windowMgr.h"
#include "renderer.h"
#include "camera.h"
#include "gui.h"
#include "parser2.h"

#include "myParser.h"

#include "compute.h"

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
    std::string shader_path = "C:\\Users\\a.refaat\\projects\\pointCloudViewer\\pointCloudViewer\\shaders\\compute";
    std::string model_path = "C:\\Users\\a.refaat\\projects\\pointCloudViewer\\assets\\dragon.ply";

    /* load shaders and buffer the data to opengl */

    // test the compute shader stuff here

    compute computeProg(model_path,shader_path);

    renderer screenQuad(shader_path);
    screenQuad.setup();

 

    /* init the gui manger and attach other modules it needs to operate*/
    gui dearGui(&win);
    dearGui.attatch_renderer(&screenQuad);
    dearGui.setup();
    /* attach the gui back to the window so it can dispatch events back to the gui*/
    win.attach_gui(&dearGui);

    computeProg.setup();
    computeProg.useProgram();

    glm::mat4 projection = glm::perspective(float(glm::radians(cam.m_fov)), (float)win.m_win_width / (float)win.m_win_height, 0.1f, 1000.0f);
    glm::mat4 view = cam.m_view;
    glm::mat4 model = glm::mat4(1.0f);
    computeProg.setUniformMat4("model", model);

    computeProg.setUniformMat4("view", view);

    computeProg.setUniformMat4("projection", projection);

    /* attatch the gui back to the window so it can dispatch events back to the gui*/

    int frameCounter = 0;
    double previousTime = glfwGetTime();
    double accumDelta = 0;


    screenQuad.attach_shaderProg_SSB(computeProg.posSSbo_output);
    screenQuad.attach_shaderProg_num_elms(computeProg.num_elms);

    /* rendering loop */
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
        computeProg.check_for_opengl_errors();

        computeProg.useProgram();
        /* update the projection matrix when the window is resized */
        if (win.m_win_resized)
        {
            win.m_win_resized = false;
            projection = glm::perspective(float(glm::radians(cam.m_fov)), (float)win.m_win_width / (float)win.m_win_height, 0.1f, 1000.0f);
            computeProg.setUniformMat4("projection", projection);
        }

        computeProg.check_for_opengl_errors(); 

        // compRender.setFloat("t", currentTime);
        cam.update();
        view = cam.m_view;
        computeProg.setUniformMat4("view", view);
        
        computeProg.dispatch();        

        // render image to quad
        screenQuad.useProgram();
        /* -------------------- */
        screenQuad.render();
        
        dearGui.confg();
        dearGui.render();
        
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



#if 0
int main(int argc, char const* argv[])
{
    
    std::string model_path = "C:\\Users\\a.refaat\\projects\\pointCloudViewer\\assets\\Villa85M.ply";

    //parser2 p;
    //p.read_file(model_path);
    myParser parser(model_path);
    parser.read_file();
}
#endif 

/*db 8a 8b c0 5d ed d5 c1 20 81 d6 c2 7c 31 28 c3 4a ec ce c2 40 33 13 c3 c7 09 28 c3 51 7a cf c2 50 2d 13 c3 0a 07 28 c3 f1 43 cf c2 20 41 13 c3 20 d3 27 c3 f7 d5  ÛŠ.À]íÕÁ .ÖÂ|1(ÃJìÎÂ@3.ÃÇ.(ÃQzÏÂP-.Ã..(ÃñCÏÂ A.Ã Ó'Ã÷Õ
0x0000024646B850B6  cf c2 f8 42 13 c3 b7 e1 27 c3 5b 02 d0 c2 59 26 13 c3 67 b4 27 c3 c2 57 d0 c2 20 43 13 c3 c8 a8 27 c3 fb 9c cf c2 ba 59 13 c3 55 30 84 c0 6c 78 d7 c1 d1 42 d6 c2  ÏÂøB.Ã·á'Ã[.ÐÂY&.Ãg´'ÃÂWÐÂ C.ÃÈ¨'ÃûœÏÂºY.ÃU0.Àlx×ÁÑBÖÂ
0x0000024646B850EC  6e 54 28 c3 44 29 ce c2 bf 3d 13 c3 02 5c 28 c3 a6 5b ce c2 03 29 13 c3 3b 01 83 c0 e8 d9 da c1 1f 25 d6 c2 3b 10 28 c3 4e 82 cf c2 79 d9 12 c3 22 0d 28 c3 e1 7a  nT(ÃD)ÎÂ¿=.Ã.\(Ã¦[ÎÂ.).Ã;.ƒÀèÙÚÁ.%ÖÂ;.(ÃN.ÏÂyÙ.Ã".(Ãáz
0x0000024646B85122  cf c2 ac 8c 12 c3 b7 42 28 c3 f5 db ce c2 df 3f 12 c3 7b 64 28 c3 36 6d ce c2 79 d9 12 c3 39 64 28 c3 8b 6c ce c2 df 3f 12 c3 af 85 28 c3 5d cd cd c2 df 3*/