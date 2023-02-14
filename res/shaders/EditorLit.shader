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
    w_pos = pos.xyz;
    w_normal = normalize(T_IM * normal).xyz;
    gl_Position = VP * M * pos;
}


#shader FRAGMENT
#version 410

struct DirectionalLight
{
    vec3 dir;
    vec3 color;
};

uniform DirectionalLight Light;
uniform vec3 CameraPos;
uniform vec3 BaseColor = vec3(1, 1, 1);
uniform float Specular = 100;

in vec3 w_normal;
in vec3 w_pos;

out vec4 color;

vec3 diffuseShade(vec3 color, vec3 luminance, vec3 normal, vec3 lightDir)
{
    float d = max(0.0, dot(normal, lightDir));
    return color * luminance * d;
}

vec3 specularShade(vec3 color, vec3 luminance, vec3 normal, vec3 halfDir, float specular)
{
    // blinn phong model
    return luminance * pow(max(dot(normal, halfDir), 0.0), specular);
}

void main()
{
    vec3 normal = normalize(w_normal.xyz);
    vec3 lightDir = normalize(-Light.dir);
    vec3 viewDir = normalize(CameraPos - w_pos);
    vec3 halfDir = normalize(-Light.dir + viewDir);
    vec3 diffuse = diffuseShade(BaseColor, Light.color, normal, lightDir);
    vec3 specular = specularShade(BaseColor, Light.color, normal, halfDir, Specular);
    color = vec4(diffuse + specular + vec3(0.3), 1.0);
}
