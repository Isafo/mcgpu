#version 450

in int vID;

out int frag_color;

void main() {

  frag_color = vID;
}