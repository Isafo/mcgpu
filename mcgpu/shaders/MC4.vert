#version 450
layout(location = 0) in vec3 VertexPosition;

uniform vec3 octantPos;

const uint dim = 32;

out int vertexID;

void main () {

	vertexID = gl_VertexID;
	gl_Position = vec4(VertexPosition , 1.0f);
}