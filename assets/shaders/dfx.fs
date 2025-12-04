#version 330

in vec2 fragTexCoord;
out vec4 fragColor;

uniform sampler2D texture0;

void main()
{
    float s00 = textureOffset(texture0, fragTexCoord, ivec2(-1, - 1)).r;
    float s02 = textureOffset(texture0, fragTexCoord, ivec2(1, - 1)).r;
    float s10 = textureOffset(texture0, fragTexCoord, ivec2(-1, 0)).r;
    
    float s12 = textureOffset(texture0, fragTexCoord, ivec2(1, 0)).r;
    float s20 = textureOffset(texture0, fragTexCoord, ivec2(-1, 1)).r;
    float s22 = textureOffset(texture0, fragTexCoord, ivec2(1, 1)).r;
    
    float gx = (s02 + 2.0 * s12 + s22) - (s00 + 2.0 * s10 + s20);
    fragColor = vec4(gx, 0.0, 0.0, 1.0);
}