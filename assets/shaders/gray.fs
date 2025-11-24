#version 330
in vec2 fragTexCoord;
out vec4 fragColor;

uniform sampler2D texture0;

void main()
{
    // 1. Amostra (lê) a cor original do pixel na textura
    vec4 color = texture(texture0, fragTexCoord);

    // 2. Calcula a Luminosidade (Escala de Cinza) usando a Ponderação Rec. 709:
    // O canal Verde (G) contribui mais para o brilho percebido.
    float luminance = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;

    // 3. Define a cor de saída:
    // O valor 'luminance' é replicado para os canais R, G e B,
    // garantindo que a cor seja um tom de cinza. O canal alfa (w) é mantido.
    fragColor = vec4(luminance, luminance, luminance, color.a);
}