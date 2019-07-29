#version 300 es

//4维
layout(location = 0) in vec3 vPosition;
precision mediump float;

void main()
{
    gl_Position = vec4(vPosition.x, vPosition.y, vPosition.z, 1.0);
}
