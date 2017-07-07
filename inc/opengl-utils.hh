#pragma once

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/**
 * Whenever the window size change this callback function executes
 */
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
