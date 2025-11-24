#version 330

in vec2 fragTexCoord;
out vec4 fragColor;

uniform sampler2D texture0;

void main()
{
    vec2 texel = vec2(1.0) / vec2(textureSize(texture0, 0));

    float s00 = texture(texture0, fragTexCoord + texel * vec2(-1, -1)).r;
    float s01 = texture(texture0, fragTexCoord + texel * vec2( 0, -1)).r;
    float s02 = texture(texture0, fragTexCoord + texel * vec2( 1, -1)).r;

    float s20 = texture(texture0, fragTexCoord + texel * vec2(-1,  1)).r;
    float s21 = texture(texture0, fragTexCoord + texel * vec2( 0,  1)).r;
    float s22 = texture(texture0, fragTexCoord + texel * vec2( 1,  1)).r;

    float gy =
        (s20 + 2.0 * s21 + s22) -
        (s00 + 2.0 * s01 + s02);

    float gy_normalized = (gy / 8.0) + 0.5; 

    fragColor = vec4(gy_normalized, gy_normalized, gy_normalized, 1.0);
}
