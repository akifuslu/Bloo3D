#shader VERTEX
#version 410

layout(location = 0) in vec4 pos;

uniform mat4 MVP;

void main()
{
    gl_Position = MVP * pos;
}


#shader FRAGMENT
#version 410

out vec4 color;

void main()
{
    color = vec4(1.0);
}
