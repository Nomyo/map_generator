#include <camera.hh>

#include <cmath>
#include <GL/glu.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mouse.h>
#include <iostream>

Camera::Camera(const glm::vec3& position)
{
    position_ = position;
    phi_ = 0;
    theta_ = 0;
    compute_vect_from_angles();

    speed_ = 0.01;
    sensivity_ = 0.2;
    vertical_motion_active_ = false;

    keyconf_["forward"] = SDLK_z;
    keyconf_["backward"] = SDLK_s;
    keyconf_["strafe_left"] = SDLK_q;
    keyconf_["strafe_right"] = SDLK_d;
    keyconf_["boost"] = SDLK_LSHIFT;

    keystates_[keyconf_["forward"]] = false;
    keystates_[keyconf_["backward"]] = false;
    keystates_[keyconf_["strafe_left"]] = false;
    keystates_[keyconf_["strafe_right"]] = false;
    keystates_[keyconf_["boost"]] = false;

    SDL_SetRelativeMouseMode(SDL_TRUE);
    SDL_ShowCursor(SDL_DISABLE);
}

void Camera::on_mouse_motion(const SDL_MouseMotionEvent& event)
{
    theta_ -= event.xrel * sensivity_;
    phi_ -= event.yrel * sensivity_;
    compute_vect_from_angles();
}

void Camera::on_mouse_wheel(const SDL_MouseWheelEvent& event)
{
    if (event.y >= 0)
    {
        vertical_motion_active_ = true;
        timeout_vertical_motion_ = 250;
        vertical_motion_direction_ = 1;

    }
    else if (event.y < 0)
    {
        vertical_motion_active_ = true;
        timeout_vertical_motion_ = 250;
        vertical_motion_direction_ = -1;
    }
}

void Camera::on_keyboard(const SDL_KeyboardEvent& event)
{
    for (auto it = keystates_.begin(); it != keystates_.end(); it++)
    {
        if (event.keysym.sym == it->first)
        {
            it->second = (event.type == SDL_KEYDOWN);
            break;
        }
    }
}

void Camera::animate(Uint32 timestep)
{
    float realspeed = (keystates_[keyconf_["boost"]])?10*speed_:speed_;
    if (keystates_[keyconf_["forward"]])
        position_ += forward_ * (realspeed * timestep);
    if (keystates_[keyconf_["backward"]])
        position_ -= forward_ * (realspeed * timestep);
    if (keystates_[keyconf_["strafe_left"]])
        position_ += left_ * (realspeed * timestep);
    if (keystates_[keyconf_["strafe_right"]])
        position_ -= left_ * (realspeed * timestep);
    if (vertical_motion_active_)
    {
        if (timestep > timeout_vertical_motion_)
            vertical_motion_active_ = false;
        else
            timeout_vertical_motion_ -= timestep;
        position_ += glm::vec3(0,0,
			       vertical_motion_direction_ *  realspeed * timestep);
    }

    target_ = position_ + forward_;
}

void Camera::set_speed(float speed)
{
    speed_ = speed;
}

void Camera::set_sensivity(float sensivity)
{
    sensivity_ = sensivity;
}

void Camera::set_position(const glm::vec3& position)
{
    position_ = position;
    target_ = position_ + forward_;
}

void Camera::compute_vect_from_angles()
{
    static const glm::vec3 global_up(0,0,1);

    if (phi_ > 89)
        phi_ = 89;
    else if (phi_ < -89)
        phi_ = -89;
    float r_temp = cos(phi_*M_PI/180);
    forward_.z = sin(phi_*M_PI/180);
    forward_.x = r_temp*cos(theta_*M_PI/180);
    forward_.y = r_temp*sin(theta_*M_PI/180);

    left_ = glm::normalize(glm::cross(global_up, forward_));
    target_ = position_ + forward_;
}

void Camera::look()
{
    gluLookAt(position_.x,position_.y,position_.z,
              target_.x,target_.y,target_.z,
              0, 0, 1);
}

void Camera::debug()
{
    std::cout << "position : " << std::endl;
    std::cout << "x = " << position_.x << std::endl;
    std::cout << "y = " << position_.y << std::endl;
    std::cout << "z = " << position_.z << std::endl;

    std::cout << "target : " << std::endl;
    std::cout << "x = " << target_.x << std::endl;
    std::cout << "y = " << target_.y << std::endl;
    std::cout << "z = " << target_.z << std::endl;
}

Camera::~Camera()
{
    SDL_SetRelativeMouseMode(SDL_FALSE);
}
