#version 130

in vec3 normal;

uniform vec3 ucolor;
uniform vec3 ulightdir;

void main()
{
    float intensity = -dot(ulightdir, normal);
    gl_FragColor = vec4(intensity * ucolor, 1.);
}
