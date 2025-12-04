#version 330

in vec2 fragTexCoord;
out vec4 fragColor;

uniform float u_seed;

float hash(vec2 p)
{
    float h = dot(p, vec2(127.1, 311.7));
    return fract(sin(h + u_seed) * 43758.5453123);
}

void main()
{
    float n = hash(fragTexCoord);
    fragColor = vec4(n, n, n, 1.0);
}
