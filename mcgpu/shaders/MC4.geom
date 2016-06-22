#version 450
layout(points) in;
layout(points) out;
layout(max_vertices = 1) out;

const uint dim = 32;

uniform sampler3D neededEdges;

in int vertexID[];
out int vID;

void main() {

	gl_PointSize = 1;
	gl_Layer = int(gl_in[0].gl_Position.z);
	vID = vertexID[0];

	// saved edges 1, 2, 10 (nvidia pic) maybe save 0, 3, 8 instead to get floor on all?
	vec3 xyz = vec3(ceil(gl_in[0].gl_Position.x), 
					ceil(gl_in[0].gl_Position.y), 
					floor(gl_in[0].gl_Position.z));

	vec3 texCoord = vec3((xyz/dim)*2 - vec3(1.0, 1.0, 1.0));
	//vec3 edges = texture3D(neededEdges, texCoord);

	vec diff = xyz - gl_in[0].gl_Position.xyz;

	// if edge 5 diff.z != 0
	if(diff.z != 0) 
		texCoord.x = texCoord.x * 3;
	// if edge 6 diff.x != 0
	else if (diff.x != 0){
		texCoord.x = texCoord.x * 3 + 1;
	}
	// if edge 10 diff.y != 0
	else if(diff.y != 0) {
		texCoord.x = texCoord.x * 3 + 2;
	}

	gl_Position = vec4(texCoord.xy, 0, 1);
	EmitVertex();
}