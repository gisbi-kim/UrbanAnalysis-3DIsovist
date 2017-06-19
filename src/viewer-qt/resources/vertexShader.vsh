#version 450

//! [0]
uniform mat4 mvpMatrix;

in vec4 vertex;
in vec4 color;

out VS_OUT
{
    vec4 color;

} vs_out;

void main(void)
{
    vs_out.color = color;
    gl_Position = mvpMatrix * vertex;
}
//! [0]
