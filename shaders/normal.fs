#version 130
 
in  vec2 vTexCoord;
//out vec4 gl_FragColor;

uniform sampler2D Diffuse;
uniform sampler2D Normal;
in vec3 vNormal;
in vec3 vTangent;
in vec3 vBiNormal;
 
void main(void) {
	////When using a Normalmap
	//vec4 nTex = texture2D(Normal, vTexCoord) * 2 - 1;
	//vec3 N = nTex.a * vTangent + -nTex.g * vBiNormal + nTex.b * vNormal;
	
	//Using only the geometry normal
	//vec3 N = vNormal;
	
	//gl_FragColor.rgb = normalize(vNormal) * 0.5 + 0.5;
	gl_FragColor = texture2D(Diffuse, vTexCoord);
	//gl_FragColor.rg = vTexCoord;
	gl_FragColor.a = 1;
}