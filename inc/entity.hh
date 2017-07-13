#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <model.hh>

class Entity
{
public:
    Entity(const Model& model, glm::vec3 position,
	   glm::vec3 rotate, float scale);
    ~Entity();

    void increase_position(glm::vec3 p);
    void decrease_position(glm::vec3 p);
    void increase_rotation(glm::vec3 r);
    void decrease_rotation(glm::vec3 r);

    void set_model(const Model& model);
    void set_position(glm::vec3 position);
    void set_rotation(glm::vec3 rotation);
    void set_scale(float scale);

    Model get_model() const;
    glm::vec3 get_position() const;
    glm::vec3 get_rotation() const;
    float get_scale() const;

private:
    Model model_;
    glm::vec3 position_;
    glm::vec3 rotate_;
    float scale_;
};
