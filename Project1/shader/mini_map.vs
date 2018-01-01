#version 330 core
layout (location = 0) in vec3 vertex; // <vec2 pos, vec2 tex>
layout (location = 1) in vec2 aTexCoords;

uniform mat4 projection;

out vec2 TexCoords;

void main()
{
	TexCoords = aTexCoords;
    gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
} 