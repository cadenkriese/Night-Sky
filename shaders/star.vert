#version 330 core

layout (location = 0) in vec2 position; // Altitude (θ), Azimuth (φ)
layout (location = 1) in float magnitude;
layout (location = 2) in float temperature;

out float vTemperature;
out float vPointSize;

uniform mat4 view;
uniform mat4 projection;

const float REFERENCE_MAGNITUDE = 6.f;
const float REFERENCE_SIZE = 2.f;

void main() {
    // Spherical to cartesian conversion, except I measure altitude from the horizon
    // So I use cos(θ) instead of sin(θ) (which is how these formulas are usually written)
    // Because sin(90-θ) = cos(θ)
    // x = r * cos(θ) * cos(φ)
    // y = r * cos(θ)
    // z = r * cos(θ) * sin(φ)
    // Where θ is altitude and φ is azimuth. I set r = 1 for convenience.
    // Also, this follows the glm coordinate system where z is up.
    gl_Position = projection * view * vec4(
        cos(position[0])*cos(position[1]), // x
        sin(position[0]),                  // y
        cos(position[0])*sin(position[1]), // z
        1.0                                // w
    );

    float size = REFERENCE_SIZE * pow(10.f, -0.2*(magnitude-REFERENCE_MAGNITUDE));
    gl_PointSize = max(size, 4.f);

    vTemperature = temperature;
    vPointSize = gl_PointSize;
}