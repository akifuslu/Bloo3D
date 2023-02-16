#shader VERTEX
#version 410

layout(location = 0) in vec4 pos;
layout(location = 1) in vec4 normal;

layout(std140) uniform Matrices
{
    mat4 VP; // view projection
};

uniform mat4 M; // model
uniform mat4 T_IM; // transposed inverse model
uniform vec2 ScreenSize = vec2(1024, 768);

uniform float OutlineWidth = 5;

void main()
{
    vec4 clipPos = VP * M * pos;
    vec4 clipNorm = VP * M * normal;

    clipPos.xy += normalize(clipNorm.xy) / ScreenSize * OutlineWidth * clipPos.w * 2;
    gl_Position = clipPos;
}


#shader FRAGMENT
#version 410

uniform vec3 OutlineColor = vec3(0.0, 0.3686, 1.0);
out vec4 color;

void main()
{
    color = vec4(OutlineColor, 1.0);
}
