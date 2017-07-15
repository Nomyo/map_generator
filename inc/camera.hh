#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

class Camera
{

public:
    enum class Camera_movement
    {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
     };

public:
    Camera(const glm::vec3& position = glm::vec3(0.0f, 0.0f, 0.0f),
	   const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f));
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ,
	   float yaw, float pitch);
    Camera(const glm::vec3& position, const glm::vec3& up,
	   float yaw, float pitch);
    virtual ~Camera() = default;

    virtual glm::mat4 get_view_matrix();
    virtual glm::vec3 get_view_pos();
    virtual float get_zoom();
    virtual void boosted(float factor);
    virtual void process_keyboard(Camera_movement dir, float delta_time);
    virtual void process_mouse_scroll(float yoffset);
    virtual void process_mouse_movement(float xoffset, float yoffset,
					GLboolean constrain_pitch = true);
    virtual void print_debug();

protected:
    void update_vectors();

protected:
    glm::vec3 position_;
    glm::vec3 target_;
    glm::vec3 up_;
    glm::vec3 right_;
    glm::vec3 world_up_;

    float yaw_   = -90.0f;
    float pitch_ = 0.0f;

    float movement_speed_     = 2.5f;
    float boost_factor_       = 1.0f;
    float mouse_sensitivity_  = 0.1f;
    float zoom_               = 45.0f;
};
