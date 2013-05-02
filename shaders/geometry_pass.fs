#version 130
in  vec3 WorldPos;
in  vec2 TexCoord;
in  vec3 Normal;  

uniform sampler2D gColorMap;          

out vec4 out_frag_color;      

void main()                                 
{                                           
    out_frag_color = vec4(WorldPos,1);                 
    out_frag_color = texture(gColorMap, TexCoord); 
    out_frag_color = vec4(normalize(Normal),1);                    
    out_frag_color = vec4(TexCoord, 0.0, 1.0);              
};