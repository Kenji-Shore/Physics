#version 330

out vec3 color;
in vec3 fragmentColor;
in vec2 UV;
uniform sampler2D textureSampler;

void main() {
    color = fragmentColor + texture(textureSampler, UV).rgb;
}