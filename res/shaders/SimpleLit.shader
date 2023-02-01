#shader VERTEX
#version 410

layout(location = 0) in vec4 pos;
layout(location = 1) in vec4 normal;

uniform mat4 MVP;

out vec3 w_normal;
out vec3 w_pos;

void main()
{
    w_pos = pos.xyz;
    w_normal = normal.xyz;
    gl_Position = MVP * pos;
}


#shader FRAGMENT
#version 410

in vec3 w_normal;
in vec3 w_pos;

out vec4 color;

void main()
{
    vec3 LightPos = vec3(0, 2, -2);
    vec3 lightDir = normalize((LightPos - w_pos.xyz));
    vec3 norm = normalize(w_normal.xyz);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * vec3(1.0);
    vec3 ambient = vec3(0.451, 0.5765, 0.8118);
    color = vec4(diffuse + ambient, 1.0);
}
