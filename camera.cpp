#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>

const float EPSILON = 0.000001f; // For managing floating point precision

Camera::Camera() : _yaw(0), _pitch(0) {}

glm::mat4 Camera::getView() const {
    // Spherical to cartesian conversion, like in star.vert
    glm::vec3 direction(cos(_yaw)*cos(_pitch), sin(_pitch), sin(_yaw)*cos(_pitch));
    return glm::lookAt(glm::vec3(0.0, 0.0, 0.0), direction, glm::vec3(0.0, 1.0, 0.0));
}

float Camera::getYaw() const {
    return _yaw;
}

void Camera::setYaw(float yaw) {
    _yaw = fmod(yaw, 2.f * (float) M_PI);
}

float Camera::getPitch() const {
    return _pitch;
}

void Camera::setPitch(float pitch) {
    if (pitch >= (float) M_PI_2) {
        pitch = (float) M_PI_2 - EPSILON;
    } else if (pitch <= (float) -M_PI_2) {
        pitch = (float) -M_PI_2 + EPSILON;
    }

    _pitch = pitch;
}