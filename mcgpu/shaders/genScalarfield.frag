#version 450

flat in vec3 pos;

const float dim = 32;

out vec4 frag_color;

void main() {

	float scalarValue;

	if (pos.y < (dim / 2) + sin(100.0f * float(pos.x / dim)) + sin( 100.0f * float(pos.z / dim)) || 
		( mod(pos.x, 6)  == 0 && mod(pos.z, 6)  == 0 && pos.y < (dim / 2) + 10 ))
		scalarValue = 255;
	else
		scalarValue = 0;

	scalarValue = 0;

	frag_color = vec4(scalarValue, 0.0, 0.0, 1.0);
}