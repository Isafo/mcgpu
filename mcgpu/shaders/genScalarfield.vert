#version 450
layout(location = 0) in vec3 VertexPosition;

uniform vec3 octantPos;

const uint dim = 32;

out vec3 offsetPos;

void main () {
	// generate points for the first slice in the 3Dtexture
	vec3 vertexPosition = vec3(mod(gl_InstanceID, dim),
						  (int(mod(gl_InstanceID, dim*dim)) / dim),
						  0);

	offsetPos = octantPos;

	gl_Position = vec4(vertexPosition , 1.0f);
}