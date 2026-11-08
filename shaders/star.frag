#version 330 core

in float vPointSize;

out vec4 fColor;

void main() {
    vec2 circCoord = 2.0 * gl_PointCoord - 1.0;
    float radiusSquared = dot(circCoord, circCoord);

    if (radiusSquared > 1.0) {
        discard;
    }

    float intensity = exp(-2.0 * radiusSquared);
    fColor = vec4(vec3(1.0) * intensity, 1.0);
}