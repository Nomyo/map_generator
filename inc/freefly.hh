#pragma once

#include <SDL2/SDL.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <map>
#include <string>

class FreeFlyCamera
{
public:
    FreeFlyCamera(const glm::vec3& position);

    virtual void OnMouseMotion(const SDL_MouseMotionEvent& event);
    virtual void OnMouseWheel(const SDL_MouseWheelEvent& event);
    virtual void OnKeyboard(const SDL_KeyboardEvent& event);

    virtual void animate(Uint32 timestep);
    virtual void setSpeed(float speed);
    virtual void setSensivity(float sensivity);
    virtual void setPosition(const glm::vec3& position);

    virtual void look();
    virtual void debug();

    virtual ~FreeFlyCamera();

protected:
    using KeyStates = std::map<SDL_Keycode, bool>;
    using KeyConf = std::map<std::string, SDL_Keycode>;

    float _speed;
    float _sensivity;

    //vertical motion stuffs
    Uint32 _timeBeforeStoppingVerticalMotion;
    bool _verticalMotionActive;
    int _verticalMotionDirection;

    KeyStates _keystates;
    KeyConf _keyconf;

    glm::vec3 _position;
    glm::vec3 _target;
    glm::vec3 _forward;
    glm::vec3 _left;

    float _theta;
    float _phi;

    void VectorsFromAngles();
};
