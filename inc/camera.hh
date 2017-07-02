#pragma once

#include <SDL2/SDL.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <map>
#include <string>

class Camera
{
public:
    Camera(const glm::vec3& position);

    virtual void on_mouse_motion(const SDL_MouseMotionEvent& event);
    virtual void on_mouse_wheel(const SDL_MouseWheelEvent& event);
    virtual void on_keyboard(const SDL_KeyboardEvent& event);

    virtual void animate(Uint32 timestep);
    virtual void set_speed(float speed);
    virtual void set_sensivity(float sensivity);
    virtual void set_position(const glm::vec3& position);

    virtual void look(void);
    virtual void debug(void);

    virtual ~Camera();

protected:
    using KeyStates = std::map<SDL_Keycode, bool>;
    using KeyConf = std::map<std::string, SDL_Keycode>;

    float speed_;
    float sensivity_;

    //vertical motion stuffs
    Uint32 timeout_vertical_motion_;
    bool vertical_motion_active_;
    int vertical_motion_direction_;

    KeyStates keystates_;
    KeyConf keyconf_;

    glm::vec3 position_;
    glm::vec3 target_;
    glm::vec3 forward_;
    glm::vec3 left_;

    float theta_;
    float phi_;

    void compute_vect_from_angles(void);
};
