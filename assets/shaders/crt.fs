#version 330

// Entradas do shader de vértice
in vec2 fragTexCoord;
in vec4 fragColor;

// Saída final (cor do pixel)
out vec4 finalColor;

uniform sampler2D texture0;
uniform vec2 screenSize;   

const float CURVATURE = 0.05;   
const float VIGNETTE = 0.8;     
const float SCANLINE_STRENGTH = 0.75; 
const float SCANLINE_WIDTH = 3.0;     
const float ABERRATION = 0.003; 
vec2 barrelDistortion(vec2 uv, float strength) {
    vec2 centerUv = (uv - 0.5) * 2.0; 
    
    float dist = dot(centerUv, centerUv);
    vec2 warpedUv = centerUv * (1.0 + dist * strength);
    
    return (warpedUv * 0.5) + 0.5; 
}

void main() {
    vec2 warpedUv = barrelDistortion(fragTexCoord, CURVATURE);
    
    if (warpedUv.x < 0.0 || warpedUv.x > 1.0 || warpedUv.y < 0.0 || warpedUv.y > 1.0) {
        finalColor = vec4(0.0, 0.0, 0.0, 1.0);
        return;
    }
    
    float distToCenter = length((fragTexCoord - 0.5) * 2.0);
    float offset = distToCenter * ABERRATION;
    
    float r = texture(texture0, warpedUv + vec2(offset, 0.0)).r;
    float g = texture(texture0, warpedUv).g; 
    float b = texture(texture0, warpedUv - vec2(offset, 0.0)).b;
    
    vec3 texColor = vec3(r, g, b);
    
    float scanline = mod(gl_FragCoord.y, SCANLINE_WIDTH);
    if (scanline < (SCANLINE_WIDTH / 2.0)) {
        texColor *= SCANLINE_STRENGTH;
    }
    
    float vignette = 1.0 - (dot(fragTexCoord - 0.5, fragTexCoord - 0.5) * VIGNETTE);
        vignette = pow(vignette, 1.5); // pow() dá um falloff mais agradável
        
    finalColor = vec4(texColor * vignette, 1.0) * fragColor;
}