#version 330

in vec2 fragTexCoord;
out vec4 fragColor;

uniform sampler2D u_angle_mag;
uniform sampler2D u_non_maximum;
uniform sampler2D u_noise;
uniform float u_drop_prob;

const float PI = 3.14159265;
const float TWO_PI = PI * 2.0; // 2π
const float PI_2 = PI * 0.5; // π/2
const float PI_4 = PI * 0.25; // π/4
const float PI_8 = PI * 0.125; // π/8

const float DIR_0_MAX = PI_8; // 0° - 45°
const float DIR_45_MAX = 3.0 * PI_8; // 45° - 90°
const float DIR_90_MAX = 5.0 * PI_8; // 90° - 135°

const float SQRT_32 = 5.656854249; // sqrt(32)

vec3 HUEtoRGB(float H) {
    float R = abs(H * 6.0 - 3.0) - 1.0;
    float G = 2.0 - abs(H * 6.0 - 2.0);
    float B = 2.0 - abs(H * 6.0 - 4.0);
    return clamp(vec3(R, G, B), 0, 1);
}

const float MIN_THRESHOLD = 0.2;
const float MAX_THRESHOLD = 0.5;

void main()
{
    float edge = texture(u_non_maximum, fragTexCoord).r;
    float rnd = texture(u_noise, fragTexCoord).r;

    if (rnd < u_drop_prob) {
        edge = 0.0;
    }

    fragColor = vec4(edge, edge, edge, 1.0);
}
