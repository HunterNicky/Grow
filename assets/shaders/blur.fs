#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

uniform sampler2D texture0;
uniform vec2 u_resolution;

uniform vec2 u_direction; 

out vec4 finalColor;

const float PI = 3.14159265359;

void main() {
    vec2 texelSize = 1.0 / u_resolution;

    int u_radius = 10;
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

    colorSum += texture(texture0, fragTexCoord) * (1.0 / sigmaRoot);
    weightSum += (1.0 / sigmaRoot);

    for (int i = 1; i <= u_radius; i++) {
        float weight = exp(-(float(i * i)) / twoSigmaSq) / sigmaRoot;

        vec2 offset = vec2(float(i)) * texelSize * u_direction;
        
        vec4 sample1 = texture(texture0, fragTexCoord + offset);
        vec4 sample2 = texture(texture0, fragTexCoord - offset);

        colorSum += (sample1 + sample2) * weight;
        weightSum += 2.0 * weight;
    }

    finalColor = colorSum / weightSum;
}