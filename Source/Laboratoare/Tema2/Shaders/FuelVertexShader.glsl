#version 330

// TODO: get vertex attributes from each location
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 Color;

// TODO: output values to fragment shader
out vec3 frag_color;
out vec3 frag_normal;
out vec2 frag_texture;
out vec3 frag_position;

void main()
{
	// TODO: send output to fragment shader
	frag_color = Color;

	// TODO: compute gl_Position
	gl_Position = Model * vec4(v_position, 1.0);
}
