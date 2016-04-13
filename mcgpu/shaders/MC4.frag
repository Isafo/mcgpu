#version 450

in int vertexID;

out vec4 frag_color;

void main() {

  frag_color = vec4(vec3(vertexID), 1.0);
}