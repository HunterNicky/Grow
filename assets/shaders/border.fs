#version 330

in vec2 fragTexCoord;
out vec4 fragColor;

uniform sampler2D u_dx;
uniform sampler2D u_dy;

vec3 HUEtoRGB(float H)  {
    float R = abs(H * 6.0 - 3.0) - 1.0;
    float G = 2.0 - abs(H * 6.0 - 2.0);
    float B = 2.0 - abs(H * 6.0 - 4.0);
    return clamp(vec3(R,G,B),0,1);
}

void main()
{
    float gx = texture(u_dx, fragTexCoord).r;
    float gy = texture(u_dy, fragTexCoord).r;

    float magnitude = length(vec2(gx, gy));
    float angle = atan(gy, gx);

    float normalizedAngle = (angle + 3.14159) / (2.0 * 3.14159);

    fragColor = vec4(HUEtoRGB(normalizedAngle) * magnitude, 1.0);
}
