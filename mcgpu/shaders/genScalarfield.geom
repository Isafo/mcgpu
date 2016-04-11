#version 450
layout(points) in;
layout(points, max_vertices = 1) out;
layout(invocations = 32) in; // run 256 instances for all slices in the 3D texture

out int gl_Layer;
out int gl_ViewportIndex;
flat out vec3 pos;

uniform sampler3D scalarField;
uniform isampler2D triTable;

const float scale = 0.001;

void main() {
	
	pos = vec3(gl_in[0].gl_Position.xy, gl_InvocationID * scale);
	gl_Position = gl_in[0].gl_Position;
	gl_Layer = gl_InvocationID;

	EmitVertex();
	EndPrimitive();
}