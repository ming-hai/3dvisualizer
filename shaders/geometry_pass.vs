#version 330 compatibility

in VSInput
{
    vec3 Position;
    vec2 TexCoord;
    vec3 Normal;
} VSin;

out VSOutput
{
    vec3 WorldPos;
    vec2 TexCoord;
    vec3 Normal;
} VSout;

uniform mat4 gWVP;
uniform mat4 gWorld;

void main()
{
    gl_Position = gWVP * vec4(VSin.Position, 1.0);
    VSout.TexCoord   = VSin.TexCoord;
    VSout.Normal     = (gWorld * vec4(VSin.Normal, 0.0)).xyz;
    VSout.WorldPos   = (gWorld * vec4(VSin.Position, 1.0)).xyz;
};
