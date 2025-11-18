#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

uniform sampler2D texture0;
uniform vec2 u_resolution;

uniform int u_radius;
out vec4 finalColor;

const float PI = 3.14159265359;

void main() {
    vec2 texelSize = 1.0 / u_resolution;
    
    if (u_radius <= 0) {
        finalColor = texture(texture0, fragTexCoord);
        return;
    }

    float sigma = float(u_radius) / 2.0;
    if (sigma < 1.0) sigma = 1.0; 
    
    float twoSigmaSq = 2.0 * sigma * sigma;
    float sigmaRoot = sqrt(twoSigmaSq * PI);

    vec4 colorSum = vec4(0.0);
    float weightSum = 0.0;

    for (int x = -u_radius; x <= u_radius; x++) {
        for (int y = -u_radius; y <= u_radius; y++) {
            
            float r = sqrt(float(x*x + y*y));
            float weight = exp(-(r*r) / twoSigmaSq) / sigmaRoot;

            vec2 offset = vec2(float(x), float(y)) * texelSize;
            colorSum += texture(texture0, fragTexCoord + offset) * weight;
            
            weightSum += weight;
        }
    }

    finalColor = colorSum / weightSum;
}