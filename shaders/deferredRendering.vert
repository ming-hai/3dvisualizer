#version 130

in vec3 in_Position;
in vec3 in_Normal;
in vec2 in_TexCoord;

out vec2 vTexCoord;

void main( void )
{
  //gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
  gl_Position = vec4(in_Position, 1.0);

  //gl_TexCoord[0] = gl_MultiTexCoord0;
  vTexCoord = in_TexCoord;

  gl_FrontColor = vec4(1.0, 1.0, 1.0, 1.0);
}
