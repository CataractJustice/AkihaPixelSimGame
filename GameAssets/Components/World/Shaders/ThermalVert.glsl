#version 330 core
layout (location = 0) in vec3 vertexPosition;
layout (location = 3) in vec2 vertexUV;

out vec2 TexCoord;

void main()
{
	TexCoord = vertexUV;
	gl_Position = vec4(vertexPosition, 1.0f);
}