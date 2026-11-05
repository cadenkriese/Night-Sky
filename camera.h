#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

class Camera {
public:
    Camera();

    /**
     * @brief calculates the view matrix for the current camera state and returns it
     */
    glm::mat4 getView() const;

    float getYaw() const;

    /**
     * @brief sets the yaw in the range of 0 to 2π using modulus
     */
    void setYaw(float);

    float getPitch() const;

    /**
     * @brief sets the pitch, the values are clamped to the range -π/2 to π/2
     */
    void setPitch(float);
private:
    float _yaw;
    float _pitch;
};

#endif // CAMERA_H