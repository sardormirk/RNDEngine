#version 330 core

layout (location = 3) in vec3 lPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main() {
    gl_Position = proj * view * model * vec4(lPos, 1.0f);
}