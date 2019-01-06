#version 330

out vec3 color;
//in vec3 fragmentColor;
in vec2 UV;
uniform sampler2D textureSampler;
uniform vec3 Colors[3];

void main() {
    vec3 rgb = texture(textureSampler, UV).rgb;
    color = Colors[0] * rgb[0] + Colors[1] * rgb[1] + Colors[2] * rgb[2];
}