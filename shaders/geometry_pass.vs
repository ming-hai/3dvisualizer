#version 330 compatibility

in layout(location = 0) vec3 inPosition;
in layout(location = 1) vec2 inTexCoord;
in layout(location = 2) vec3 inNormal;  

out  vec3 WorldPos;
out  vec2 TexCoord;
out  vec3 Normal;  

uniform mat4 gWVP;
uniform mat4 gWorld;

void main()         
{                   
  gl_Position = gWVP * vec4(inPosition, 1.0);
  TexCoord   = inTexCoord;                  
  Normal     = (gWorld * vec4(inNormal, 0.0)).xyz;   
  WorldPos   = (gWorld * vec4(inPosition, 1.0)).xyz; 
}
