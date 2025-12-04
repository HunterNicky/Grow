#version 330
in vec2 fragTexCoord;
out vec4 fragColor;

uniform sampler2D texture0;

void main()
{
    vec4 color = texture(texture0, fragTexCoord);
    float luminance = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
    fragColor = vec4(luminance, luminance, luminance, color.a);
}