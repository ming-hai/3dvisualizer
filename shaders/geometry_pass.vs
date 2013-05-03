#version 410 compatibility

in layout(location = 0) vec3 Position;
in layout(location = 1) vec2 in_TexCoord;
in layout(location = 2) vec3 in_Normal;  

out  vec3 WorldPos;
out  vec2 TexCoord;
out  vec3 Normal;  

uniform mat4 gWVP;
uniform mat4 gWorld;

void main()         
{                   
  gl_Position = gWVP * vec4(Position, 1.0);
  TexCoord   = in_TexCoord;                  
  Normal     = (gWorld * vec4(in_Normal, 0.0)).xyz;   
  WorldPos   = (gWorld * vec4(InPosition, 1.0)).xyz; 
}
