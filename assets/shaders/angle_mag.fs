#version 330

in vec2 fragTexCoord;
out vec4 fragColor;

uniform sampler2D u_dx;
uniform sampler2D u_dy;

void main()
{
    float gx = texture(u_dx, fragTexCoord).r;
    float gy = texture(u_dy, fragTexCoord).r;
    
    float angle = atan(gy, gx);
    float magnitude = length(vec2(gx, gy));

    fragColor = vec4(angle, magnitude, 0.0, 1.0);
}