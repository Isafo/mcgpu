#version 450
layout(points) in;
layout(points) out;
layout(max_vertices = 1) out;

const uint dim = 32;

void main() {

	gl_PointSize = 1;
	gl_Layer = int(gl_in[0].gl_Position.z);
	gl_Position = vec4((gl_in[0].gl_Position.xy/dim)*2 - vec2(1,1), 0, 1);
	EmitVertex();

}