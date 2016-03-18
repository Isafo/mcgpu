#version 450
layout(points) in;
layout(points, max_vertices = 1) out;
layout(invocations = 256) in; // run 256 instances for all slices in the 3D texture

out int gl_Layer;
out int gl_ViewportIndex;

uniform sampler3D scalarField;
uniform isampler2D triTable;

const float stepLength = 0.5f;
const uint dim = 256;

void main() {
	


	gl_Layer = gl_InstancedID;	
	
}