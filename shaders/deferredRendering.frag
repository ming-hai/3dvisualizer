#version 130

in  vec2 vTexCoord;

uniform sampler2D tDiffuse; 
uniform sampler2D tPosition;
uniform sampler2D tNormals;
uniform sampler2D tShadowMap;
uniform vec3 cameraPosition;

uniform mat4 worldToLightViewMatrix;
uniform mat4 lightViewToProjectionMatrix;
uniform mat4 worldToCameraViewMatrix;

void main( void )
{
  //vec4 image = texture2D( tDiffuse, gl_TexCoord[0].xy );
  //vec4 position = texture2D( tPosition, gl_TexCoord[0].xy );
  //vec4 normal = texture2D( tNormals, gl_TexCoord[0].xy );
  vec4 image = texture2D( tDiffuse, vTexCoord );
  vec4 position = texture2D( tPosition, vTexCoord );
  vec4 normal = texture2D( tNormals, vTexCoord );

  vec3 light = vec3(50, 100, 100);
  vec3 lightDir = light - position.xyz;

  normal = normalize(normal);
  lightDir = normalize(lightDir);

  vec3 eyeDir = normalize(cameraPosition - position.xyz);
  vec3 vHalfVector = normalize(lightDir.xyz+eyeDir);

  gl_FragColor = max(dot(normal, vec4(lightDir, 1.0)), 0.0) * image + pow(max(dot(normal, vec4(vHalfVector, 1.0)), 0.0), 100) * 1.5;
}
