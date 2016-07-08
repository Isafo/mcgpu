#version 450
layout(location = 0) in vec3 VertexPosition;

uniform vec3 octantPos;

uniform sampler3D scalarField;
uniform sampler3D edgeTex;

const float isoValue = 0.5;

out vec4 vert1;
out vec4 vert2;
out vec4 vert3;

out vec3 norm;

const float stepLength = 0.5f;
const uint dim = 32; 

vec3 lerp(vec3 first, vec3 second, float firstI, float secondI) {
	float dVal = (isoValue - firstI) / (secondI - firstI);
	return first + dVal*(second - first) ;
}

void main () {

	vert1 = vec4(0.0, 0.0, 0.0, 0.0);
	vert2 = vec4(0.0, 0.0, 0.0, 0.0);
	vert3 = vec4(0.0, 0.0, 0.0, 0.0);

	// get the position of the scalarvalues in the 3Dtexture

	//TODO: Scale vertexPosition directly and then calculate scaled xyz vectors instead of always scaling xyz vectors
	vec3 xyz[8];
	xyz[0] = VertexPosition + vec3(-stepLength, -stepLength, -stepLength );
	xyz[1] = VertexPosition + vec3(stepLength, -stepLength, -stepLength );
	xyz[2] = VertexPosition + vec3(stepLength, -stepLength, stepLength );
	xyz[3] = VertexPosition + vec3(-stepLength, -stepLength, stepLength );

	xyz[4] = VertexPosition + vec3(-stepLength, stepLength, -stepLength );
	xyz[5] = VertexPosition + vec3(stepLength, stepLength, -stepLength );
	xyz[6] = VertexPosition + vec3(stepLength, stepLength, stepLength );
	xyz[7] = VertexPosition + vec3(-stepLength, stepLength, stepLength );
	
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

	// get the edge data from the texture
	float edge1 = texture(edgeTex, VertexPosition * scale).r;
	float edge2 = texture(edgeTex, VertexPosition * scale).g;
	float edge3 = texture(edgeTex, VertexPosition * scale).b;

	// Calculate the position if the edge contains a vertex
	if (edge1 != 0.0){
		vert1 = vec4(lerp(xyz[5], xyz[6], scalarValue[5], scalarValue[6]), 1.0);
	}
	if (edge2 != 0.0){
		vert2 = vec4(lerp(xyz[6], xyz[7], scalarValue[6], scalarValue[7]), 1.0);
	}
	if (edge3 != 0.0){
		vert3 = vec4(lerp(xyz[2], xyz[6], scalarValue[2], scalarValue[6]), 1.0);
	}

	// calculate the vertex normal using the gradient
	float scalarX = texture(scalarField, (VertexPosition + vec3(3 * stepLength, stepLength, stepLength )) * scale).r;
	float scalarY = texture(scalarField, (VertexPosition + vec3(stepLength, 3 * stepLength, stepLength )) * scale).r;
	float scalarZ = texture(scalarField, (VertexPosition + vec3(stepLength, stepLength, 3 * -stepLength )) * scale).r;

	norm.x = scalarX - scalarValue[7];
	norm.y = scalarY - scalarValue[2];
	norm.z = scalarValue[5] - scalarZ;

	normalize(norm);

	//TODO: look at using gl_Position for one of the generated verts

	//vec3 vertexPosition = VertexPosition;
	gl_Position = vec4(VertexPosition , 1.0f);
}