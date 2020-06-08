#version 330

layout(location = 0) in vec2 vertex;
layout(location = 1) in vec3 color;

out vec3 vert_color;

void main(void)
{
  gl_Position  = vec4(vertex,0.0,1.0);

  vert_color = color;
}
