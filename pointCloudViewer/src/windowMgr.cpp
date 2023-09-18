#include "windowMgr.h"
#include "camera.h"
#include "imgui_impl_glfw.h"
#include "gui.h"

unsigned int WindowMgr::m_win_height = 0;
unsigned int WindowMgr::m_win_width = 0;
bool WindowMgr::m_win_resized = false;
double WindowMgr::m_deltaTime = 0;
bool WindowMgr::m_rightMouseClicked = false;
camera *WindowMgr::m_camera = nullptr;

float WindowMgr::m_yaw = 90.0;
float WindowMgr::m_pitch = 0.0;
bool WindowMgr::m_firstMouse = false;
float WindowMgr::m_lastX = 0;
float WindowMgr::m_lastY = 0;
gui *WindowMgr::m_gui = nullptr;
float WindowMgr::m_camera_speed = 100;

void WindowMgr::error_callback(int error, const char *description)
{
	// Print error
	fputs(description, stderr);
}
WindowMgr::WindowMgr(const std::string &win_title, unsigned int win_width, unsigned int win_height)
{
	m_win_width = win_width;
	m_win_height = win_height;
	m_win_title = win_title;
	m_win_resized = false;
	m_firstMouse = false;

	glfwInit();
#if defined(__APPLE__) || defined(__linux__)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	m_win = glfwCreateWindow(m_win_width, m_win_height, m_win_title.c_str(), NULL, NULL);
	if (m_win == NULL)
	{
		fprintf(stdout, "Failed to create GLFW window!\n");
		glfwTerminate();
	}
	glfwMakeContextCurrent(m_win);
	glfwSwapInterval(0);

	setup_callbacks(m_win);
}
WindowMgr::~WindowMgr()
{
	glfwTerminate();
}
bool WindowMgr::should_close()
{
	return glfwWindowShouldClose(m_win);
}
void WindowMgr::setTitle(const std::string &title)
{
	glfwSetWindowTitle(m_win, title.c_str());
}
void WindowMgr::setup_callbacks(GLFWwindow *win)
{
	// Set callback function for resizing the window
	glfwSetFramebufferSizeCallback(m_win, (GLFWframebuffersizefun)window_size_callback);

	// Set callback for key up/down/repeat events
	glfwSetKeyCallback(m_win, (GLFWkeyfun)key_callback);

	// Set callbacks for mouse movement (cursor position) and mouse botton up/down events.
	glfwSetCursorPosCallback(m_win, (GLFWcursorposfun)mouse_callback);
	glfwSetMouseButtonCallback(m_win, (GLFWmousebuttonfun)mouse_button_callback);
	// glfwSetScrollCallback(m_win,scroll_callback);
}

void WindowMgr::window_size_callback(GLFWwindow *window, int width, int height)
{
	m_win_resized = true;
	glViewport(0, 0, width, height); // Draw into entire window
	m_win_height = height;
	m_win_width = width;
}
void WindowMgr::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_RELEASE)
	{
		return; // Ignore key up (key release) events
	}
	if (key == GLFW_KEY_ESCAPE || key == GLFW_KEY_X)
	{
		glfwSetWindowShouldClose(window, true);
	}

	float cameraSpeed = static_cast<float>(m_camera_speed * m_deltaTime);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		m_camera->m_cameraPos += cameraSpeed * m_camera->m_cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		m_camera->m_cameraPos -= cameraSpeed * m_camera->m_cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		m_camera->m_cameraPos -= glm::normalize(glm::cross(m_camera->m_cameraFront, m_camera->m_cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		m_camera->m_cameraPos += glm::normalize(glm::cross(m_camera->m_cameraFront, m_camera->m_cameraUp)) * cameraSpeed;
}

void WindowMgr::mouse_callback(GLFWwindow *window, double xposIn, double yposIn)
{

	if (!(m_gui->io.WantCaptureMouse))
	{

		float xpos = static_cast<float>(xposIn);
		float ypos = static_cast<float>(yposIn);
		if (m_firstMouse)
		{
			m_lastX = xpos;
			m_lastY = ypos;
			m_firstMouse = false;
		}
		float xoffset = xpos - m_lastX;
		float yoffset = ypos - m_lastY; // reversed since y-coordinates go from bottom to top
		m_lastX = xpos;
		m_lastY = ypos;

		if (m_rightMouseClicked)
		{

			float sensitivity = 0.1f; // change this value to your liking
			xoffset *= sensitivity;
			yoffset *= sensitivity;

			m_yaw += xoffset;
			m_pitch += yoffset;

			// make sure that when pitch is out of bounds, screen doesn't get flipped
			if (m_pitch > 89.0f)
				m_pitch = 89.0f;
			if (m_pitch < -89.0f)
				m_pitch = -89.0f;

			glm::vec3 front;
			front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
			front.y = sin(glm::radians(m_pitch));
			front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
			m_camera->m_cameraFront = glm::normalize(front);
		}
	}
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void WindowMgr::scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
	m_camera->m_fov -= (float)yoffset;
	if (m_camera->m_fov < 1.0f)
		m_camera->m_fov = 1.0f;
	if (m_camera->m_fov > 45.0f)
		m_camera->m_fov = 45.0f;
}
void WindowMgr::mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		m_rightMouseClicked = true;

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
		m_rightMouseClicked = false;
}
void WindowMgr::attach_camera(camera *camera)
{
	m_camera = camera;
}
void WindowMgr::attach_gui(gui *pGui)
{
	if (pGui)
		m_gui = pGui;
}
