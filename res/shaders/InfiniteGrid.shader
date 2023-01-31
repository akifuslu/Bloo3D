#shader VERTEX
#version 410

layout(location = 0) in vec4 pos;

uniform mat4 InvView;
uniform mat4 InvProj;

vec4 UnProject(float x, float y, float z)
{
    vec4 val = InvView * InvProj * vec4(x, y, z, 1);
    return val / val.w;
}

out vec3 nearPoint;
out vec3 farPoint;

void main()
{
    nearPoint = UnProject(pos.x, pos.y, 0.0).xyz;
    farPoint  = UnProject(pos.x, pos.y, 1.0).xyz;
    gl_Position = pos;
}


#shader FRAGMENT
#version 410

uniform mat4 ViewProj;
uniform float Near;
uniform float Far;
uniform float CameraY;

in vec3 nearPoint;
in vec3 farPoint;

out vec4 color;

float map(float value, float min1, float max1, float min2, float max2) 
{
  return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
}

float linearDepth(vec3 wp) 
{
    vec4 clipSpacePos = ViewProj * vec4(wp.xyz, 1.0);
    float clipSpaceDepth = (clipSpacePos.z / clipSpacePos.w);
    float ld = (2.0 * Near) / (Far + Near - clipSpaceDepth * (Far - Near));    
    return ld;
}

vec4 grid(vec3 wp, float scale)
{
    vec2 coord = wp.xz * scale;
    vec2 dv = fwidth(coord);
    vec2 grid = abs(fract(coord-0.5)-0.5) / dv;
    float line = min(grid.x, grid.y);

    float r = mix(0.2, 1.0, step(abs(coord.y), min(1.0, dv.y)));
    float b = mix(0.2, 1.0, step(abs(coord.x), min(1.0, dv.x)));

    return vec4(r, 0.2, b, 1.0 - min(line, 1.0));
}

vec4 gridAlt(vec3 wp, float scale)
{
    vec2 coord = wp.xz * scale;
    vec2 grid = abs(fract(coord-0.5)-0.5);
    return vec4(step(min(grid.x, grid.y), 0.1));
}

void main()
{
    float t = -nearPoint.y / (farPoint.y - nearPoint.y);
    vec3 wp = nearPoint + t * (farPoint - nearPoint);   
    float y = abs(CameraY) + 0.01;
    float scale = 10.0;
    float gridMix = 0;
    float a, b;
    if(y < 10)
    {
        a = 0;
        b = 10;
        scale = 10.0;
    }
    else if(y < 50)
    {
        a = 10;
        b = 50;
        scale = 1.0;
    }
    else
    {
        a = 50;
        b = 1000;
        scale = 0.1;
    }
    gridMix = map(y, a, b, 0, 1);
    float scale2 = scale / 10.0;
    color = mix(grid(wp, scale), grid(wp, scale2), gridMix) * float(t > 0.0);
    float ld = linearDepth(wp);
    color.a *= smoothstep(0.5, 0.0, ld);
}
