#version 330

in vec2 fragTexCoord;
out vec4 fragColor;

uniform sampler2D texture0;

void main()
{
    float s00 = textureOffset(texture0, fragTexCoord, ivec2(-1, -1)).r;
    float s01 = textureOffset(texture0, fragTexCoord, ivec2( 0, -1)).r;
    float s02 = textureOffset(texture0, fragTexCoord, ivec2( 1, -1)).r;

    float s20 = textureOffset(texture0, fragTexCoord, ivec2(-1,  1)).r;
    float s21 = textureOffset(texture0, fragTexCoord, ivec2( 0,  1)).r;
    float s22 = textureOffset(texture0, fragTexCoord, ivec2( 1,  1)).r;

    float gy = (s20 + 2.0 * s21 + s22) - (s00 + 2.0 * s01 + s02);
    fragColor = vec4(gy, 0.0, 0.0, 1.0);
}