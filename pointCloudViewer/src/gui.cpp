#include "windowMgr.h"
#include "gui.h"
gui::gui(WindowMgr *window_mgr) : m_winMgr(window_mgr)
{
}

gui::~gui()
{
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void gui::setup()
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(m_winMgr->m_win, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void gui::confg()
{
    io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls

    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos; // Enable mouse Controls

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    {
        static int counter = 0;
        ImGui::Begin("Point Cloud Viewer!"); // Create a window called "Hello, world!" and append into it.

        // ImGui::Text("This is some useful text.");          // Display some text (you can use a format strings too)
        // ImGui::Checkbox("Demo Window", &show_demo_window); // Edit bools storing our window open/close state
        ImGui::SliderFloat("Camera Speed", &(m_winMgr->m_camera_speed), 0.0f, 10000.0f); // Edit 1 float using a slider from 0.0f to 1.0f
        if (ImGui::ColorEdit3("clear color", (float *)&m_color))                         // Edit 3 floats representing a color
        {
            m_renderer->setUniformVec3("color", glm::vec3(m_color.x, m_color.y, m_color.z));
        }

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();
    }
}

void gui::render()
{
    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void gui::attatch_renderer(renderer *rendrer)
{
    if (rendrer)
        m_renderer = rendrer;
}