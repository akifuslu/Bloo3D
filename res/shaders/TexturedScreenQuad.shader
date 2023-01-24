#shader VERTEX
#version 410

layout(location = 0) in vec4 pos;
layout(location = 1) in vec2 texcoord;

out vec2 uv;

void main()
{
    uv = texcoord;
    gl_Position = pos;
}


#shader FRAGMENT
#version 410

uniform sampler2D basemap;

in vec2 uv;
out vec4 color;

void main()
{
    //color = vec4(uv.x, uv.y, 0.0, 1.0);
    color = texture(basemap, uv);
}
