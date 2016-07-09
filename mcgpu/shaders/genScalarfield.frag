#version 450

in float scalarValue;

out vec4 frag_color;

void main() {

	frag_color = vec4(scalarValue, 0.0, 0.0, 1.0);
	//frag_color = vec4(1.0, 0.0, 0.0, 1.0);
}