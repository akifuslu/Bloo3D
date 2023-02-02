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

out vec3 w_normal;
out vec3 w_pos;

void main()
{
    w_pos = (M * pos).xyz;
    w_normal = normalize(T_IM * normal).xyz;
    gl_Position = VP * M * pos;
}


#shader FRAGMENT
#version 410

#define MAX_POINT_LIGHT_COUNT 8

struct PointLight
{
    vec3 pos;
    vec3 color;
};

uniform PointLight Light;

in vec3 w_normal;
in vec3 w_pos;

out vec4 color;

vec3 diffuseShade(vec3 color, vec3 luminance, vec3 normal, vec3 lightDir)
{
    float d = max(0.0, dot(normal, lightDir));
    return color * luminance * d;
}

void main()
{
    vec3 lightDir = normalize((Light.pos - w_pos.xyz));
    vec3 normal = normalize(w_normal.xyz);
    float r = distance(w_pos, Light.pos);
    vec3 lum = Light.color / (r * r);

    vec3 diffuse = diffuseShade(vec3(1, 1, 1), lum, normal, lightDir);
    color = vec4(diffuse, 1.0);
}
