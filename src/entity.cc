#include <entity.hh>

Entity::Entity(const Model& model, glm::vec3 position,
	       glm::vec3 rotate, float scale)
    : model_(model)
    , position_(position)
    , rotate_(rotate)
    , scale_(scale)
{ }

Entity::~Entity() = default;

void Entity::increase_position(glm::vec3 p)
{
    position_.x += p.x;
    position_.y += p.y;
    position_.z += p.z;
}

void Entity::decrease_position(glm::vec3 p)
{
    position_.x -= p.x;
    position_.y -= p.y;
    position_.z -= p.z;
}

void Entity::increase_rotation(glm::vec3 r)
{
    rotate_.x += r.x;
    rotate_.y += r.y;
    rotate_.z += r.z;
}

void Entity::decrease_rotation(glm::vec3 r)
{
    rotate_.x -= r.x;
    rotate_.y -= r.y;
    rotate_.z -= r.z;
}

void Entity::set_model(const Model& model)
{
    model_ = model;
}

void Entity::set_position(glm::vec3 position)
{
    position_ = position;
}

void Entity::set_rotation(glm::vec3 rotation)
{
    rotate_ = rotation;
}

void Entity::set_scale(float scale)
{
    scale_ = scale;
}

Model Entity::get_model() const
{
    return model_;
}

glm::vec3 Entity::get_position() const
{
    return position_;
}

glm::vec3 Entity::get_rotation() const
{
    return rotate_;
}

float Entity::get_scale() const
{
    return scale_;
}
