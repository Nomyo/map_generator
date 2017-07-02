#include <freefly.hh>

#include <cmath>
#include <GL/glu.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mouse.h>
#include <iostream>

FreeFlyCamera::FreeFlyCamera(const glm::vec3& position)
{
    _position = position;
    _phi = 0;
    _theta = 0;
    VectorsFromAngles();

    _speed = 0.01;
    _sensivity = 0.2;
    _verticalMotionActive = false;
    _keyconf["forward"] = SDLK_z;
    _keyconf["backward"] = SDLK_s;
    _keyconf["strafe_left"] = SDLK_q;
    _keyconf["strafe_right"] = SDLK_d;
    _keyconf["boost"] = SDLK_LSHIFT;
    _keystates[_keyconf["forward"]] = false;
    _keystates[_keyconf["backward"]] = false;
    _keystates[_keyconf["strafe_left"]] = false;
    _keystates[_keyconf["strafe_right"]] = false;
    _keystates[_keyconf["boost"]] = false;

    SDL_SetRelativeMouseMode(SDL_TRUE);
    SDL_ShowCursor(SDL_DISABLE);
}

void FreeFlyCamera::OnMouseMotion(const SDL_MouseMotionEvent& event)
{
    _theta -= event.xrel*_sensivity;
    _phi -= event.yrel*_sensivity;
    VectorsFromAngles();
}

void FreeFlyCamera::OnMouseWheel(const SDL_MouseWheelEvent& event)
{
    if (event.y >= 0)
    {
        _verticalMotionActive = true;
        _timeBeforeStoppingVerticalMotion = 250;
        _verticalMotionDirection = 1;

    }
    else if (event.y < 0)
    {
        _verticalMotionActive = true;
        _timeBeforeStoppingVerticalMotion = 250;
        _verticalMotionDirection = -1;
    }
}

void FreeFlyCamera::OnKeyboard(const SDL_KeyboardEvent& event)
{
    for (auto it = _keystates.begin(); it != _keystates.end(); it++)
    {
        if (event.keysym.sym == it->first)
        {
            it->second = (event.type == SDL_KEYDOWN);
            break;
        }
    }
}

void FreeFlyCamera::animate(Uint32 timestep)
{
    float realspeed = (_keystates[_keyconf["boost"]])?10*_speed:_speed;
    if (_keystates[_keyconf["forward"]])
        _position += _forward * (realspeed * timestep);
    if (_keystates[_keyconf["backward"]])
        _position -= _forward * (realspeed * timestep);
    if (_keystates[_keyconf["strafe_left"]])
        _position += _left * (realspeed * timestep);
    if (_keystates[_keyconf["strafe_right"]])
        _position -= _left * (realspeed * timestep);
    if (_verticalMotionActive)
    {
        if (timestep > _timeBeforeStoppingVerticalMotion)
            _verticalMotionActive = false;
        else
            _timeBeforeStoppingVerticalMotion -= timestep;
        _position += glm::vec3(0,0,_verticalMotionDirection*realspeed*timestep);
    }

    _target = _position + _forward;
}

void FreeFlyCamera::setSpeed(float speed)
{
    _speed = speed;
}

void FreeFlyCamera::setSensivity(float sensivity)
{
    _sensivity = sensivity;
}

void FreeFlyCamera::setPosition(const glm::vec3& position)
{
    _position = position;
    _target = _position + _forward;
}

void FreeFlyCamera::VectorsFromAngles()
{
    static const glm::vec3 global_up(0,0,1);

    if (_phi > 89)
        _phi = 89;
    else if (_phi < -89)
        _phi = -89;
    float r_temp = cos(_phi*M_PI/180);
    _forward.z = sin(_phi*M_PI/180);
    _forward.x = r_temp*cos(_theta*M_PI/180);
    _forward.y = r_temp*sin(_theta*M_PI/180);

    _left = glm::normalize(glm::cross(global_up, _forward));
    _target = _position + _forward;
}

void FreeFlyCamera::look()
{
    gluLookAt(_position.x,_position.y,_position.z,
              _target.x,_target.y,_target.z,
              0, 0, 1);
}

void FreeFlyCamera::debug()
{
    std::cout << "position : " << std::endl;
    std::cout << "x = " << _position.x << std::endl;
    std::cout << "y = " << _position.y << std::endl;
    std::cout << "z = " << _position.z << std::endl;

    std::cout << "target : " << std::endl;
    std::cout << "x = " << _target.x << std::endl;
    std::cout << "y = " << _target.y << std::endl;
    std::cout << "z = " << _target.z << std::endl;
}

FreeFlyCamera::~FreeFlyCamera()
{
    SDL_SetRelativeMouseMode(SDL_FALSE);
}
