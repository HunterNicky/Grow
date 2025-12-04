#version 330

in vec2 fragTexCoord;
out vec4 fragColor;

uniform sampler2D texture0;
uniform int u_radius;

void main()
{
    vec2 texSize = vec2(textureSize(texture0, 0));
    vec2 texel = 1.0 / texSize;

    float m = 0.0;
    for (int y = -u_radius; y <= u_radius; ++y) {
        for (int x = -u_radius; x <= u_radius; ++x) {
            vec2 offset = vec2(x, y) * texel;
            float v = texture(texture0, fragTexCoord + offset).r;
            m = max(m, v);
        }
    }

    fragColor = vec4(m, m, m, 1.0);
}
