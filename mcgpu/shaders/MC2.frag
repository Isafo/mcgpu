#version 450

in vec3 neededEdges;

out vec4 frag_color;

void main() {

  frag_color = vec4(neededEdges, 1.0);
  //frag_color = vec4(255.0, 255.0, 255.0, 255.0);
}