#version 330
in vec2 fragTexCoord;
out vec4 fragColor;

uniform sampler2D u_edges;
uniform sampler2D u_noise;
uniform vec2 u_resolution;
uniform float u_spread;

void main()
{
    float noiseVal = texture(u_noise, fragTexCoord).r;
    
    vec2 offset1 = vec2(noiseVal - 0.5) * u_spread;
    vec2 offset2 = vec2(noiseVal - 0.5) * u_spread * 0.6;
    vec2 offset3 = vec2(texture(u_noise, fragTexCoord * 1.3).r - 0.5) * u_spread * 0.8;
    
    float edge1 = texture(u_edges, fragTexCoord + offset1).r;
    float edge2 = texture(u_edges, fragTexCoord + offset2).r;
    float edge3 = texture(u_edges, fragTexCoord + offset3).r;
    
    float edgeIntensity = max(edge1, max(edge2 * 0.7, edge3 * 0.5));
    edgeIntensity *= (0.6 + 0.4 * noiseVal);
    edgeIntensity = smoothstep(0.15, 0.55, edgeIntensity);

    vec3 pencilColor = vec3(0.25, 0.25, 0.27);
    vec3 paperColor = vec3(0.97, 0.97, 0.96);
    vec3 finalRender = mix(paperColor, pencilColor, edgeIntensity);
    
    fragColor = vec4(finalRender, 1.0);
}