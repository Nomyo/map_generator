#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

class Light
{
public:
    Light(glm::vec3 position, glm::vec3 color);
    ~Light() = default;

    glm::vec3 get_position();
    glm::vec3 get_color();
    void set_position(glm::vec3 position);
    void set_color(glm::vec3 color);

private:
    glm::vec3 position_;
    glm::vec3 color_;
};
