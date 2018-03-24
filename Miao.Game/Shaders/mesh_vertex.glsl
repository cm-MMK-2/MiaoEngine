#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>
layout (location = 1) in vec4 inColor; // <rgba>

out vec2 TexCoords;
out vec4 TintColor;

uniform mat4 projection;

void main()
{
    TexCoords = vertex.zw;
	TintColor = inColor;
    gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
}