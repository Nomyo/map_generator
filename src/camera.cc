#include <camera.hh>

Camera::Camera(const glm::vec3& position, const glm::vec3& up)
    : position_(position)
    , target_(glm::vec3(0.0f, 0.0f, -1.0f))
    , up_(up)
{
    world_up_ = glm::vec3(0.0f, 1.0f, 0.0f);
    update_vectors();
}

Camera::Camera(const glm::vec3& position, const glm::vec3& up,
	       float yaw, float pitch)
    : position_(position)
    , target_(glm::vec3(0.0f, 0.0f, -1.0f))
    , up_(up)
    , yaw_(yaw)
    , pitch_(pitch)
{
    world_up_ = glm::vec3(0.0f, 1.0f, 0.0f);
    update_vectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY,
	       float upZ, float yaw, float pitch)
    : target_(glm::vec3(0.0f, 0.0f, -1.0f))
    , yaw_(yaw)
    , pitch_(pitch)
{
    position_ = glm::vec3(posX, posY, posZ);
    world_up_ = glm::vec3(upX, upY, upZ);
    update_vectors();
}

glm::mat4 Camera::get_view_matrix()
{
    return glm::lookAt(position_, position_ + target_, up_);
}
float Camera::get_zoom()
{
    return zoom_;
}

void Camera::boosted(float factor)
{
    boost_factor_ = factor;
}

void Camera::process_keyboard(Camera_movement dir, float delta_time)
{
    float velocity = movement_speed_ * boost_factor_ * delta_time;

    if (dir == Camera_movement::FORWARD)
	position_ += target_ * velocity;
    if (dir == Camera_movement::BACKWARD)
	position_ -= target_ * velocity;
    if (dir == Camera_movement::LEFT)
	position_ -= right_ * velocity;
    if (dir == Camera_movement::RIGHT)
	position_ += right_ * velocity;
    if (dir == Camera_movement::UP)
	position_ += world_up_ * velocity;
    if (dir == Camera_movement::DOWN)
	position_ -= world_up_ * velocity;
}

void Camera::process_mouse_scroll(float yoffset)
{
    if (zoom_ >= 1.0f && zoom_ <= 45.0f)
	zoom_ -= yoffset;
    else if (zoom_ < 1.0f)
	zoom_ = 1.0f;
    else if (zoom_ > 45.0f)
	zoom_ = 45.0f;
}

void Camera::process_mouse_movement(float xoffset, float yoffset,
				    GLboolean constrain_pitch)
{
    xoffset *= mouse_sensitivity_;
    yoffset *= mouse_sensitivity_;

    yaw_   += xoffset;
    pitch_ += yoffset;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrain_pitch)
    {
	if (pitch_ > 89.0f)
	    pitch_ = 89.0f;
	else if (pitch_ < -89.0f)
	    pitch_ = -89.0f;
    }

    update_vectors();
}

void Camera::update_vectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front.y = sin(glm::radians(pitch_));
    front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    target_ = glm::normalize(front);

    right_ = glm::normalize(glm::cross(target_, world_up_));

    // Normalize the vectors, because their length gets closer to 0
    // the more you look up or down which results in slower movement.
    up_ = glm::normalize(glm::cross(right_, target_));
}

void Camera::print_debug()
{
    std::cout << "position: " << position_.x << " "
	      << position_.y << " "
	      << position_.z << " "
	      << std::endl;

    std::cout << "target: " << target_.x << " "
	      << target_.y << " "
	      << target_.z << " "
	      << std::endl;

    std::cout << "up: " << up_.x << " "
	      << up_.y << " "
	      << up_.z << " "
	      << std::endl;

    std::cout << "right: " << right_.x << " "
	      << right_.y << " "
	      << right_.z << " "
	      << std::endl;
}
