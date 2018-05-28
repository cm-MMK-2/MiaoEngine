#version 330 core
in vec2 TexCoords;
in vec4 TintColor;
out vec4 color;

uniform sampler2D mainTexture;

void main()
{    
    color = TintColor * texture(mainTexture, TexCoords);
}