#version 300 es
layout (location=0) in vec3 position;
out vec4 vectexColor;
precision mediump float;

void main() {
    gl_Position=vec4(position.xyz,1.0);
    vectexColor=vec4(0.5f, 0.0f, 0.0f, 1.0f);
}
