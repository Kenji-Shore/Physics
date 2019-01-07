#version 330

out vec3 color;
//in vec3 fragmentColor;
in vec2 UV;
in vec3 normal;
uniform sampler2D textureSampler;
uniform vec3 Colors[3];
uniform mat4 ModelRotation;
uniform vec3 Ambient;
uniform vec3 LightDirection;

void main() {
    vec3 rgb = texture(textureSampler, UV).rgb;
    color = mix(Colors[0] * rgb[0] + Colors[1] * rgb[1] + Colors[2] * rgb[2], Ambient, dot(vec3(ModelRotation * vec4(normal, 0)), LightDirection));
}