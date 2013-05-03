#version 410 compatibility

in layout(location = 0) vec3 WorldPos;
in layout(location = 1) vec2 TexCoord;
in layout(location = 2) vec3 Normal;  

uniform sampler2D gColorMap;          

out vec4 out_frag_color;      

void main()                                 
{                                           
    out_frag_color = vec4(WorldPos,1);                 
    out_frag_color = texture(gColorMap, TexCoord); 
    out_frag_color = vec4(normalize(Normal),1);                    
    out_frag_color = vec4(TexCoord, 0.0, 1.0);              
};