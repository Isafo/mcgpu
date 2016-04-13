#version 450
layout(location = 0) in vec3 VertexPosition;

uniform vec3 octantPos;

const uint dim = 31;

void main () {

	vec3 vertexPosition = vec3(mod(gl_InstanceID, dim),
							(int(mod(gl_InstanceID, dim*dim)) / dim),
							 (gl_InstanceID / (dim * dim)));


	//TODO: this is weird, is it a must?
	//vertexPosition.x += VertexPosition.x; // this is unnecessarily necessary

	gl_Position = vec4(vertexPosition , 1.0f);
}