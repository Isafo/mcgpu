#version 450
layout(location = 0) in vec3 VertexPosition;

uniform vec3 octantPos;

const uint dim = 254;

void main () {

	vec3 vertexPosition = VertexPosition;

	gl_Position = vec4(vertexPosition , 1.0f);
}