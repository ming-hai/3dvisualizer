#version 130
in vec3 in_Position;
in vec3 in_Normal;
in vec2 in_TexCoord;
in vec3 in_Tangent;
in vec3 in_BiNormal;

uniform mat4 ModelView;
uniform mat4 ViewProjection;
 
out vec3 vNormal;
out vec3 vTangent;
out vec3 vBiNormal;

out vec2 vTexCoord;
void main(void) {
	vTexCoord = in_TexCoord;
 
    //gl_Position = ViewProjection * ModelView * vec4(in_Position.x, in_Position.y, in_Position.z, 1.0);
	gl_Position = ViewProjection * vec4(in_Position.x, in_Position.y, in_Position.z, 1.0);
 
    //vNormal = (ModelView * vec4(in_Normal,0.0)).xyz;
	vNormal = in_Normal;
	////When using a Normalmap
	//vTangent = (ModelView * vec4(in_Tangent,0.0)).xyz;
	//vBiNormal = (ModelView * vec4(in_BiNormal,0.0)).xyz;
}