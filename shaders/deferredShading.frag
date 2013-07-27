#version 130

in  vec2 vTexCoord;

varying vec4       position;
varying vec3       normals;
uniform sampler2D  tDiffuse;

void main( void )
{
  gl_FragData[0] = vec4(texture2D(tDiffuse, vTexCoord).rgb, 0);
  gl_FragData[1] = vec4(position.xyz, 0);
  gl_FragData[2] = vec4(normals.xyz, 0);
}
