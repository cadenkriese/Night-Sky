#version 330 core

in float vPointSize;

out vec4 fColor;

void main() {
    vec2 circCoord = 2.f * gl_PointCoord - 1.f;
    float radius = length(circCoord);
    // Discard pixels not in the unit circle
    if (radius > 1.f) {
        discard;
    }

    float glow = pow(0.2f / radius, 1.5f);
    glow *= 1.f - smoothstep(0.5f, 1.f, radius);
       
    // Add color
    vec3 luminanceWeights = vec3(0.2126f, 0.7152f, 0.0722f);
    vec3 color = vec3(1.f, 1.f, 1.f);
    color = glow * color / dot(color, luminanceWeights);
	
    // Tonemapping
    color = tanh(color);
    
    fColor = vec4(color, 1.f);
}