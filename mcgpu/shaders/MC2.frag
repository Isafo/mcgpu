#version 450

in vec3 neededEdges;
in vec2 voxelPosXY;

out vec4 frag_color;

void main() {

  frag_color = vec4(neededEdges, 1.0);
}