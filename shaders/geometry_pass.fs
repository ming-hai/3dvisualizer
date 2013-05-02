#version 330 compatibility

out FSOutput
{
    vec3 WorldPos;
    vec3 Diffuse;
    vec3 Normal;
    vec3 TexCoord;
};

uniform sampler2D gColorMap;                

void main()
{
    FSout.WorldPos = FSin.WorldPos;
    FSout.Diffuse  = texture(gColorMap, FSin.TexCoord).xyz;
    FSout.Normal   = normalize(FSin.Normal);
    FSout.TexCoord = vec3(FSin.TexCoord, 0.0);
};