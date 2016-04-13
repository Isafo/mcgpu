#version 450

in vec3 neededEdges;

out vec3 frag_color;

void main() {

	//frag_color = vec4(neededEdges, 1.0);
	frag_color = vec3(neededEdges);
}