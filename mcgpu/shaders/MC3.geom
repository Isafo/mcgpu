#version 450
layout(points) in;
layout(points) out;
layout(max_vertices = 3) out;

in vec4 vert1[];
in vec4 vert2[];
in vec4 vert3[];

in vec3 norm[];

out vec3 vertexPosition;
out vec3 vertexNormal;

void main() {
	
	if(vert1[0].a == 1.0) {
		vertexPosition = vec3(vert1[0]);
		vertexNormal = norm[0];
		EmitVertex();
		EndPrimitive();
	}
	if(vert2[0].a == 1.0) {	
		vertexPosition = vec3(vert2[0]);
		vertexNormal = norm[0];
		EmitVertex();
		EndPrimitive();
	}

	if(vert3[0].a == 1.0) {
		vertexPosition = vec3(vert3[0]);
		vertexNormal = norm[0];
		EmitVertex();
		EndPrimitive();
	}
}