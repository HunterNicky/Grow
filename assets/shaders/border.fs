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

    vec2 offset = vec2(noiseVal - 0.5) * u_spread;

    vec2 jitteredCoord = fragTexCoord + offset;

    float edgeIntensity = texture(u_edges, jitteredCoord).r;

    edgeIntensity *= (0.5 + 0.5 * noiseVal);

    edgeIntensity = smoothstep(0.1, 0.6, edgeIntensity);

    vec3 pencilColor = vec3(1.0 - edgeIntensity);
    vec3 paperColor = vec3(0.98, 0.97, 0.93);
    vec3 finalRender = mix(paperColor, pencilColor, edgeIntensity);

    fragColor = vec4(finalRender, 1.0);
}
