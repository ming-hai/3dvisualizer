#version 130
vec3 InPosition;
vec2 InTexCoord;
vec3 InNormal;  

out  vec3 WorldPos;
out  vec2 TexCoord;
out  vec3 Normal;  

uniform mat4 gWVP;
uniform mat4 gWorld;

void main()         
{                   
  gl_Position = gWVP * vec4(InPosition, 1.0);
  TexCoord   = InTexCoord;                  
  Normal     = (gWorld * vec4(InNormal, 0.0)).xyz;   
  WorldPos   = (gWorld * vec4(InPosition, 1.0)).xyz; 
}
