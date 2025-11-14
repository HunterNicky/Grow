#version 330

in vec2 fragTexCoord;
in vec4 fragColor;
out vec4 finalColor;

uniform sampler2D texture0;
uniform float health;      // Vida atual (ex: 80.0)
uniform float max_health;  // Vida máxima (ex: 100.0)
uniform float time;

void main() {
    float normalizedHealth = clamp(health / max(0.0001, max_health), 0.0, 1.0);

    vec4 baseColor = texture(texture0, fragTexCoord);

    float gray = dot(baseColor.rgb, vec3(0.299, 0.587, 0.114));
    vec3 grayscale = vec3(gray);

    vec3 colorMix = mix(grayscale, baseColor.rgb, normalizedHealth);

    float dist = distance(fragTexCoord, vec2(0.5, 0.5));
    float vignette = smoothstep(0.3, 0.7, dist);

    float pulse = 0.5 + 0.5 * sin(time * 6.0);

    float intensity = (1.0 - normalizedHealth) * vignette * (0.4 + 0.6 * pulse);

    vec3 redOverlay = vec3(1.0, 0.0, 0.0);
    vec3 finalRGB = mix(colorMix, redOverlay, intensity);

    finalColor = vec4(finalRGB, baseColor.a);
}