#include <input.hh>
#include <iostream>

float Input::lastX_ = SCR_WIDTH / 2.0f;
float Input::lastY_ = SCR_HEIGHT / 2.0f;
float Input::deltaTime = 0.0f;
float Input::lastFrame = 0.0f;
bool Input::first_mouse_ = true;

void Input::init(Camera *camera)
{
    camera_ = camera;
}

Camera *Input::get_camera()
{
    return camera_;
}

Input& Input::get_instance()
{
    static Input instance;
    return instance;
}

void Input::process_input(GLFWwindow *window)
{
    auto camera = get_instance().get_camera();

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	camera->boosted(5.0f);
    else
	camera->boosted(1.0);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	camera->process_keyboard(Camera::Camera_movement::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	camera->process_keyboard(Camera::Camera_movement::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	camera->process_keyboard(Camera::Camera_movement::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	camera->process_keyboard(Camera::Camera_movement::RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
	camera->process_keyboard(Camera::Camera_movement::DOWN, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	camera->process_keyboard(Camera::Camera_movement::UP, deltaTime);
}

void Input::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    auto camera = get_instance().get_camera();

    if (first_mouse_)
    {
	lastX_ = xpos;
	lastY_ = ypos;
        first_mouse_ = false;
    }

    float xoffset = xpos - lastX_;
    float yoffset = lastY_ - ypos;
    lastX_ = xpos;
    lastY_ = ypos;

    camera->process_mouse_movement(xoffset, yoffset);
}

void Input::scroll_callback(GLFWwindow* window, double xoffset,
			    double yoffset)
{
    auto camera = get_instance().get_camera();
    std::cout << "scroll" << std::endl;
    camera->process_mouse_scroll(yoffset);
}
