#version 130
attribute vec3 apos;

uniform mat4 umodel;
uniform mat4 uview;
uniform mat4 uproj;

out vec3 normal;

void main()
{
    vec4 pos = umodel * vec4(apos, 1.0);

    normal = normalize(-pos.xyz);

    gl_Position = uproj * uview * pos;
}
