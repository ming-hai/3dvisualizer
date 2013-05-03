#version 330 compatibility

in vec3 inWorldPos;
in vec2 inTexCoord;
in vec3 inNormal;  

out vec3 WorldPos;    
out vec3 Diffuse;     
out vec3 Normal;      
out vec3 TexCoord; 
    
uniform sampler2D gColorMap;

void main()                                 
{                                           
    WorldPos = inWorldPos;                                 
    Diffuse  = texture(gColorMap, inTexCoord).xyz; 
    Normal   = normalize(inNormal);                                        
    TexCoord = vec3(inTexCoord, 0.0);           
};