#version 330

in vec2 fragTexCoord;
out vec4 fragColor;

uniform sampler2D u_original;
uniform sampler2D u_borders;

void main()
{
    vec4 original = texture(u_original, fragTexCoord);
    vec4 borders = texture(u_borders, fragTexCoord);
    
    float border_intensity = 1.0 - borders.r;
    
    vec3 result = mix(original.rgb, borders.rgb, border_intensity);
    
    fragColor = vec4(result, 1.0);
}
