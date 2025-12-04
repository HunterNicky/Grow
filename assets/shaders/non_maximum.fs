#version 330

in vec2 fragTexCoord;
out vec4 fragColor;

uniform sampler2D u_angle_mag;

const float PI = 3.14159265;
const float PI_8 = PI * 0.125; // π/8

const float DIR_0_MAX = PI_8; // 0° - 45°
const float DIR_45_MAX = 3.0 * PI_8; // 45° - 90°
const float DIR_90_MAX = 5.0 * PI_8; // 90° - 135°

const float MIN_THRESHOLD = 0.15;
const float MAX_THRESHOLD = 0.30;

void main()
{
    vec4 angle_mag = texture(u_angle_mag, fragTexCoord);
    float angle = angle_mag.r;
    float magnitude = angle_mag.g;
    
    ivec2 offset_1 = ivec2(0);
    ivec2 offset_2 = ivec2(0);
    if (angle < DIR_0_MAX) {
        offset_1 = ivec2(1, 0);
        offset_2 = ivec2(-1, 0);
    }else if (angle < DIR_45_MAX) {
        offset_1 = ivec2(1, 1);
        offset_2 = ivec2(-1, - 1);
    }else if (angle < DIR_90_MAX) {
        offset_1 = ivec2(0, 1);
        offset_2 = ivec2(0, - 1);
    }else {
        offset_1 = ivec2(-1, 1);
        offset_2 = ivec2(1, - 1);
    }
    
    float mag1 = textureOffset(u_angle_mag, fragTexCoord, offset_1).g;
    float mag2 = textureOffset(u_angle_mag, fragTexCoord, offset_2).g;
    float outMag = (magnitude >= mag1 && magnitude >= mag2) ? magnitude : 0.0;
    
    vec4 outColor = vec4(0.0, 0.0, 0.0, 1.0);
    if (outMag >= MAX_THRESHOLD) {
        outColor = vec4(1.0, 0.0, 0.0, 1.0);
    } else if (outMag >= MIN_THRESHOLD) {
        outColor = vec4(0.0, 1.0, 0.0, 1.0);
    }
    
    fragColor = outColor;
}
