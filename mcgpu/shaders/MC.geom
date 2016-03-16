#version 450
layout(points) in;
layout(triangle_strip) out;
layout(max_vertices = 15) out;

uniform sampler3D scalarField;
uniform isampler2D triTable;
//uniform isampler2D edgeTable;

const uint isoValue = 128;

out vec4 vertexPosition;

const float stepLength = 0.5f;
const uint dim = 16;
vec3 scale;

vec4 lerp(vec3 first, vec3 second, float firstI, float secondI) {
	float dVal = (isoValue - firstI) / (secondI - firstI);
	return vec4(first + dVal*(second - first), 1.0f);
}

void main() {

	vec3 xyz[8];
	xyz[0] = gl_in[0].gl_Position.xyz + vec3(-stepLength, -stepLength, -stepLength );
	xyz[1] = gl_in[0].gl_Position.xyz + vec3(stepLength, -stepLength, -stepLength );
	xyz[2] = gl_in[0].gl_Position.xyz + vec3(stepLength, -stepLength, stepLength );
	xyz[3] = gl_in[0].gl_Position.xyz + vec3(-stepLength, -stepLength, stepLength );

	xyz[4] = gl_in[0].gl_Position.xyz + vec3(-stepLength, stepLength, -stepLength );
	xyz[5] = gl_in[0].gl_Position.xyz + vec3(stepLength, stepLength, -stepLength );
	xyz[6] = gl_in[0].gl_Position.xyz + vec3(stepLength, stepLength, stepLength );
	xyz[7] = gl_in[0].gl_Position.xyz + vec3(-stepLength, stepLength, stepLength );
	
	scale = 1.0 / vec3(dim, dim, dim);

	float scalarValue[8];
	scalarValue[0] = texture(scalarField, vec3(0.5, 0.5, 0.5)).r;
	scalarValue[1] = texture(scalarField, xyz[1] * scale).r;
	scalarValue[2] = texture(scalarField, xyz[2] * scale).r;
	scalarValue[3] = texture(scalarField, xyz[3] * scale).r;

	scalarValue[4] = texture(scalarField, xyz[4] * scale).r;
	scalarValue[5] = texture(scalarField, xyz[5] * scale).r;
	scalarValue[6] = texture(scalarField, xyz[6] * scale).r;
	scalarValue[7] = texture(scalarField, xyz[7] * scale).r;

	int cubeIndex = 0;
	cubeIndex += int(scalarValue[0] >= isoValue) * 1; 
	cubeIndex += int(scalarValue[1] >= isoValue) * 2; 
	cubeIndex += int(scalarValue[2] >= isoValue) * 4; 
	cubeIndex += int(scalarValue[3] >= isoValue) * 8; 
	cubeIndex += int(scalarValue[4] >= isoValue) * 16; 
	cubeIndex += int(scalarValue[5] >= isoValue) * 32; 
	cubeIndex += int(scalarValue[6] >= isoValue) * 64; 
	cubeIndex += int(scalarValue[7] >= isoValue) * 128;

	vertexPosition = vec4(scalarValue[0], scalarValue[1], scalarValue[2], 1.0);
	gl_Position = vertexPosition;
	EmitVertex();
	vertexPosition = vec4(scalarValue[3], scalarValue[4], scalarValue[5],  1.0);
	gl_Position = vertexPosition;
	EmitVertex();
	vertexPosition = vec4(scalarValue[6], scalarValue[7], cubeIndex, 1.0);
	gl_Position = vertexPosition;
	EmitVertex();
	EndPrimitive();

	/*

	vec4 edgeVert[12];

	edgeVert[0] = lerp(xyz[0] * scale, xyz[1] * scale, scalarValue[0], scalarValue[1]);
	edgeVert[1] = lerp(xyz[1] * scale, xyz[2] * scale, scalarValue[1], scalarValue[2]);
	edgeVert[2] = lerp(xyz[2] * scale, xyz[3] * scale, scalarValue[2], scalarValue[3]);
	edgeVert[3] = lerp(xyz[3] * scale, xyz[0] * scale, scalarValue[3], scalarValue[0]);

	edgeVert[4] = lerp(xyz[4] * scale, xyz[5] * scale, scalarValue[4], scalarValue[5]);
	edgeVert[5] = lerp(xyz[5] * scale, xyz[6] * scale, scalarValue[5], scalarValue[6]);
	edgeVert[6] = lerp(xyz[6] * scale, xyz[7] * scale, scalarValue[6], scalarValue[7]);
	edgeVert[7] = lerp(xyz[7] * scale, xyz[4] * scale, scalarValue[7], scalarValue[4]);

	edgeVert[8] = lerp(xyz[0] * scale, xyz[4] * scale, scalarValue[0], scalarValue[4]);
	edgeVert[9] = lerp(xyz[1] * scale, xyz[5] * scale, scalarValue[1], scalarValue[5]);
	edgeVert[10] = lerp(xyz[2] * scale, xyz[6] * scale, scalarValue[2], scalarValue[6]);
	edgeVert[11] = lerp(xyz[3] * scale, xyz[7] * scale, scalarValue[3], scalarValue[7]);
	
	// Issue triangles
	int i = 0;
	while (true) {
		int ti = texelFetch(triTable, ivec2(i, cubeIndex), 0).a;
		if (ti == -1) break;
		vertexPosition = edgeVert[ti];
		gl_Position = vertexPosition;
		EmitVertex();

		ti = texelFetch(triTable, ivec2(i + 1, cubeIndex), 0).a;
		vertexPosition = edgeVert[ti];
		gl_Position = vertexPosition;
		EmitVertex();

		ti = texelFetch(triTable, ivec2(i + 2, cubeIndex), 0).a;
		vertexPosition = edgeVert[ti];
		gl_Position = vertexPosition;
		EmitVertex();

		EndPrimitive();
		i += 3;
	}
	*/
}