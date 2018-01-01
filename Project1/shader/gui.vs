#version 330 core
layout (location = 0) in vec3 vertex; // <vec2 pos, vec2 tex>
layout (location = 1) in float v_circle; // <vec2 pos, vec2 tex>

out float f_circle;

uniform mat4 projection;
uniform float mask;

void main()
{
	f_circle = v_circle;
    gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
} 