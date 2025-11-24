#version 330

in vec2 fragTexCoord;
out vec4 fragColor;

uniform sampler2D texture0;
uniform float threshold;

void main()
{
    vec4 color = texture(texture0, fragTexCoord);

    float intensity = color.r; 

    float final_intensity = step(threshold, intensity);

    fragColor = vec4(final_intensity, final_intensity, final_intensity, 1.0);
}