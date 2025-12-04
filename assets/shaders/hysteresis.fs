#version 330

in vec2 fragTexCoord;
out vec4 fragColor;

uniform sampler2D texture0;

void main()
{
    vec4 pixel = texture(texture0, fragTexCoord);
    float strong = pixel.r;
    float weak = pixel.g;

    if (strong > 0.5) {
        fragColor = vec4(1.0, 0.0, 0.0, 1.0);
        return;
    }

    if (weak <= 0.1) {
        fragColor = vec4(0.0, 0.0, 0.0, 1.0);
        return;
    }

    ivec2 texSize = textureSize(texture0, 0);
    vec2 texel = 1.0 / vec2(texSize);

    bool connected = false;

    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            if (x == 0 && y == 0) continue;
            vec2 coord = clamp(fragTexCoord + vec2(x, y) * texel, vec2(0.0), vec2(1.0));
            float neighborStrong = texture(texture0, coord).r;
            if (neighborStrong > 0.5) {
                connected = true;
                break;
            }
        }
        if (connected) break;
    }

    if (connected) {
        fragColor = vec4(1.0, 0.0, 0.0, 1.0);
    } else {
        fragColor = vec4(0.0, 1.0, 0.0, 1.0);
    }
}
