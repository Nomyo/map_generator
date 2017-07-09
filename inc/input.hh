#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <camera.hh>

class Input
{

public:
    static Input& get_instance();
    static void process_input(GLFWwindow *window);
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    static void scroll_callback(GLFWwindow* window, double xoffset,
				double yoffset);
    void init(Camera *camera);
    Camera *get_camera();

private:
    Input() { };
    Input(Input const&) = delete;
    void operator=(const Input&) = delete;

    Camera *camera_;

public:
    // FIXME SHOULD BE HANDLE IN OTHER CLASS
    static constexpr unsigned int SCR_WIDTH = 1600;
    static constexpr unsigned int SCR_HEIGHT = 900;

    static float lastX_;
    static float lastY_;
    static float deltaTime;
    static float lastFrame;
    static bool first_mouse_;
};
