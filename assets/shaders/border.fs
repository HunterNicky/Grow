#version 330

in vec2 fragTexCoord;
out vec4 fragColor;

uniform sampler2D u_dx;
uniform sampler2D u_dy;

void main()
{
    float CONTRASTE_FATOR = 0.2; 
    float LIMIAR = 0.15;
    
    float gx = texture(u_dx, fragTexCoord).r;
    float gy = texture(u_dy, fragTexCoord).r;

    float mag = sqrt(gx * gx + gy * gy); 

    float mag_normalized = clamp(mag * CONTRASTE_FATOR, 0.0, 1.0);
    
    float border_output = step(LIMIAR, mag_normalized); 

    fragColor = vec4(border_output, border_output, border_output, 1.0);
}