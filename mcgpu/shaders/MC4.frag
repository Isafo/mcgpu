#version 450

flat in int vID;

out int frag_color;

void main() {

  frag_color = vID;
}