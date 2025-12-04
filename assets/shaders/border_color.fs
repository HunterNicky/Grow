#version 330

in vec2 fragTexCoord;
out vec4 fragColor;

uniform sampler2D texture0;

vec3 HUEtoRGB(float H)  {
    float R = abs(H * 6.0 - 3.0) - 1.0;
    float G = 2.0 - abs(H * 6.0 - 2.0);
    float B = 2.0 - abs(H * 6.0 - 4.0);
    return clamp(vec3(R,G,B),0,1);
}

void main() {
    float s00 = textureOffset(texture0, fragTexCoord, ivec2(-1, -1)).r;
    float s01 = textureOffset(texture0, fragTexCoord, ivec2( 0, -1)).r;
    float s02 = textureOffset(texture0, fragTexCoord, ivec2( 1, -1)).r;
    float s10 = textureOffset(texture0, fragTexCoord, ivec2(-1,  0)).r;
    float s12 = textureOffset(texture0, fragTexCoord, ivec2( 1,  0)).r;
    float s20 = textureOffset(texture0, fragTexCoord, ivec2(-1,  1)).r;
    float s21 = textureOffset(texture0, fragTexCoord, ivec2( 0,  1)).r;
    float s22 = textureOffset(texture0, fragTexCoord, ivec2( 1,  1)).r;

    float gx = (s02 + 2.0 * s12 + s22) - (s00 + 2.0 * s10 + s20);
    float gy = (s20 + 2.0 * s21 + s22) - (s00 + 2.0 * s01 + s02);

    float magnitude = length(vec2(gx, gy));

    float angle = atan(gy, gx);

    float normalizedAngle = (angle + 3.14159) / (2.0 * 3.14159);

    fragColor = vec4(HUEtoRGB(normalizedAngle) * magnitude, 1.0);
}