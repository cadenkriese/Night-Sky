#version 330 core

layout (location = 0) in vec2 starPos; // (altitude, azimuth)

uniform mat4 view;
uniform mat4 projection;

void main() {
    // Spherical to cartesian conversion, except I measure altitude from the horizon
    // So I use cos instead of sin because sin(90-θ) = cos(θ)
    // x = r * cos(θ) * cos(φ)
    // y = r * cos(θ)
    // z = r * cos(θ) * sin(φ)
    // Where θ is altitude and φ is azimuth. I set r = 1 for convenience.
    // Also, this follows the glm coordinate system of z being up.

    gl_Position = projection * view * vec4(cos(starPos[0])*cos(starPos[1]), sin(starPos[0]), cos(starPos[0])*sin(starPos[1]), 1.0);
}