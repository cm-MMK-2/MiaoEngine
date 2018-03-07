#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D mainTexture;
uniform vec3 mainColor;

void main()
{    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(mainTexture, TexCoords).r);
    color = vec4(mainColor, 1.0) * sampled;
}  