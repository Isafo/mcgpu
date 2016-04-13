#version 450
layout(points) in;
layout(points, max_vertices = 1) out;
layout(invocations = 32) in; // run 32 instances for all slices in the 3D texture

out float scalarValue;

const int dim = 32;

void main() {
	// calculate the scalarvalue
	/*
	if (gl_in[0].gl_Position.y < (dim / 2) + sin(100.0f * float(gl_in[0].gl_Position.x / dim)) + sin( 100.0f * float(gl_InvocationID / dim)) || 
		( mod(gl_in[0].gl_Position.x, 6)  == 0 && mod(gl_InvocationID, 6)  == 0 && gl_in[0].gl_Position.y < (dim / 2) + 10 ))
		scalarValue = 255;
	else
		scalarValue = 0;
	*/

	if (gl_in[0].gl_Position.y <= 12)
		scalarValue = 1; // nomalized scalarvalue
	else
		scalarValue = 0;

	// get the position in screenspace coordinates
	gl_Position = vec4((gl_in[0].gl_Position.xy/dim)*2 - vec2(1.0, 1.0), 0, 1);
	gl_PointSize = 1.0;

	// render to the invocation layer
	gl_Layer = gl_InvocationID;
	
	EmitVertex();
	EndPrimitive();
}