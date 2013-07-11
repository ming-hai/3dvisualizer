#version 130
 
in  vec2 vTexCoord;
//out vec4 gl_FragColor;

uniform sampler2D Diffuse;
uniform sampler2D Normal;
in vec3 vNormal;
in vec3 vTangent;
in vec3 vBiNormal;
 
void main(void) {
        gl_FragColor.rgb = normalize(vNormal) * 0.5 + 0.5;
	gl_FragColor.a = 1;
}
