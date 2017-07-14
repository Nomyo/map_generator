#include <light.hh>

Light::Light(glm::vec3 position, glm::vec3 color)
    : position_(position)
    , color_(color)
{}

glm::vec3 Light::get_position()
{
    return position_;
}

glm::vec3 Light::get_color()
{
    return color_;
}

void Light::set_position(glm::vec3 position)
{
    position_ = position;
}


void Light::set_color(glm::vec3 color)
{
    color_ = color;
}
