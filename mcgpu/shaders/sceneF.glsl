#version 400
layout( location = 0 ) out vec4 FragColor;

in vec3 Position;
in vec3 Normal;
in vec2 UV;

uniform vec3 LP;
uniform sampler2D tex;

const float shininess = 16.0;

void main () {

	vec3 normal  = normalize( Normal );							
	vec3 lightDir = normalize(LP - Position);
	vec3 viewDir  = normalize(-Position);
		
	float lightIntensity = 0.6f/length(lightDir);
	lightDir = normalize(lightDir);

	vec2 invUV = vec2(UV.x, 1.0-UV.y);
	//vec3 texcolor = texture( tex, invUV ).rgb;
	vec3 white = vec3(1.0f, 1.0f, 1.0f);
	float texAlpha = texture( tex, invUV ).a;

	//Diffuse part-----------
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = diff * white * lightIntensity;

	//specular part-------------
	vec3 H = normalize(lightDir + viewDir);
	float NdH = max(dot(normal, H), 0.0);
	float spec = pow(NdH, shininess);
	vec3 specular = spec * white;

	// Ambient-------------
	vec3 ambient = 0.3*lightIntensity * white;// * texcolor * lightIntensity;
	
	vec3 resultLight = ambient + diffuse + specular;
	FragColor = vec4(resultLight, texAlpha);
}

