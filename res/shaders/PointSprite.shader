#shader VERTEX
#version 410

layout(location = 0) in vec4 pos;

layout(std140) uniform Matrices
{
    mat4 VP; // view projection
};

uniform mat4 M; // model

void main()
{
    gl_Position = VP * M * pos;
}


#shader FRAGMENT
#version 410

uniform sampler2D Tex;
uniform vec3 BaseColor = vec3(1.0);

out vec4 color;

void main()
{
    color = vec4(BaseColor, 1.0) * texture(Tex, gl_PointCoord);    
    if(color.a < 0.1)
    {
        discard;
    }
}
