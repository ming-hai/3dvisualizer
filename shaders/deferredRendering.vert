#version 130

in vec2 in_TexCoord;

out vec2 vTexCoord;

void main( void )
{
  gl_Position = ftransform(); // gl_ModelViewProjectionMatrix * gl_Vertex

  vTexCoord = in_TexCoord;
}
