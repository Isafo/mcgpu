#version 450
layout(points) in;
layout(points) out;
layout(max_vertices = 15) out;

uniform sampler3D scalarField;
uniform isampler2D triTable;
uniform isampler2D edgeTable;

const float isoValue = 0.5;

out uint indecies;

const float stepLength = 0.5f;
const uint dim = 32;

void main() {
	
	// get the position of the scalarvalues in the 3Dtexture
	vec3 xyz[8];
	xyz[0] = gl_in[0].gl_Position.xyz + vec3(-stepLength, -stepLength, -stepLength );
	xyz[1] = gl_in[0].gl_Position.xyz + vec3(stepLength, -stepLength, -stepLength );
	xyz[2] = gl_in[0].gl_Position.xyz + vec3(stepLength, -stepLength, stepLength );
	xyz[3] = gl_in[0].gl_Position.xyz + vec3(-stepLength, -stepLength, stepLength );

	xyz[4] = gl_in[0].gl_Position.xyz + vec3(-stepLength, stepLength, -stepLength );
	xyz[5] = gl_in[0].gl_Position.xyz + vec3(stepLength, stepLength, -stepLength );
	xyz[6] = gl_in[0].gl_Position.xyz + vec3(stepLength, stepLength, stepLength );
	xyz[7] = gl_in[0].gl_Position.xyz + vec3(-stepLength, stepLength, stepLength );

	vec3 scale = 1.0 / vec3(dim, dim, dim);

	float scalarValue[8];
	// get the scalar values from the 3D texture
	scalarValue[0] = texture(scalarField, xyz[0] * scale).r;
	scalarValue[1] = texture(scalarField, xyz[1] * scale).r;
	scalarValue[2] = texture(scalarField, xyz[2] * scale).r;
	scalarValue[3] = texture(scalarField, xyz[3] * scale).r;

	scalarValue[4] = texture(scalarField, xyz[4] * scale).r;
	scalarValue[5] = texture(scalarField, xyz[5] * scale).r;
	scalarValue[6] = texture(scalarField, xyz[6] * scale).r;
	scalarValue[7] = texture(scalarField, xyz[7] * scale).r;

	// calculate the case index in the triangle table
	int cubeIndex = 0;
	cubeIndex += int(scalarValue[0] >= isoValue) * 1; 
	cubeIndex += int(scalarValue[1] >= isoValue) * 2; 
	cubeIndex += int(scalarValue[2] >= isoValue) * 4; 
	cubeIndex += int(scalarValue[3] >= isoValue) * 8; 
	cubeIndex += int(scalarValue[4] >= isoValue) * 16; 
	cubeIndex += int(scalarValue[5] >= isoValue) * 32; 
	cubeIndex += int(scalarValue[6] >= isoValue) * 64; 
	cubeIndex += int(scalarValue[7] >= isoValue) * 128;
	
    // Issue triangles
	int i = 0;
	while (true) {
		// get vertex indecies in edgeVert
		int ti = texelFetch(triTable, ivec2(i, cubeIndex), 0).a;
		if (ti == -1) break;
		
		int ti1 = texelFetch(triTable, ivec2(i + 1, cubeIndex), 0).a;
		int ti2 = texelFetch(triTable, ivec2(i + 2, cubeIndex), 0).a;

        // lookup the vertice indices from the splatted texture
        // vertex index on edge index 5, 6, 10 (paul bourke) are
        // stored in this voxel, texCoord * 3 + n. n = 0 for edge 5,
        // 1 for edge 6 and 2 for edge 10

        EndPrimitive();
		
        EndPrimitive();

		EndPrimitive();
		i += 3;
	}

	gl_PointSize = 1;
	gl_Layer = int(gl_in[0].gl_Position.z);
	gl_Position = vec4((gl_in[0].gl_Position.xy/dim)*2 - vec2(1,1), 0, 1);
	EmitVertex();

}