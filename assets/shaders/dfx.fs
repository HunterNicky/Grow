#version 330

in vec2 fragTexCoord;
out vec4 fragColor;

uniform sampler2D texture0;

void main()
{
    vec2 texel = vec2(1.0) / vec2(textureSize(texture0, 0));
    
    float s00 = texture(texture0, fragTexCoord + texel * vec2(-1, -1)).r;
    float s02 = texture(texture0, fragTexCoord + texel * vec2( 1, -1)).r;

    float s10 = texture(texture0, fragTexCoord + texel * vec2(-1,  0)).r;
    float s12 = texture(texture0, fragTexCoord + texel * vec2( 1,  0)).r;

    float s20 = texture(texture0, fragTexCoord + texel * vec2(-1,  1)).r;
    float s22 = texture(texture0, fragTexCoord + texel * vec2( 1,  1)).r;

    float gx =
        (s02 + 2.0 * s12 + s22) -
        (s00 + 2.0 * s10 + s20);

    float gx_normalized = (gx / 8.0) + 0.5; 

    fragColor = vec4(gx_normalized, gx_normalized, gx_normalized, 1.0);
}
