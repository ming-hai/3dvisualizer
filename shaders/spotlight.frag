#version 130

in  vec2 vTexCoord;

uniform sampler2D tDiffuse;
uniform sampler2D tPosition;
uniform sampler2D tNormals;

uniform vec4 lightPos, lightColor;
uniform float lightRadius, lightIntensity;
uniform vec3 spotDir;
uniform float spotCosCutoff;
uniform bool Debug;

void main(void)
{
	vec3 n, lightDir, l, r;
	float NdotL, RdotE;
	float dist, att;
	float spotDot, spotAtt;

        vec4 colorMap = texture2DProj(tDiffuse, vec3(vTexCoord, 0.0));
        vec3 normalMap = texture2DProj(tNormals, vec3(vTexCoord, 0.0)).xyz;
        vec3 ecPos = texture2DProj(tPosition, vec3(vTexCoord, 0.0)).xyz;
	n = normalize(normalMap * 2.0 - 1.0);

	lightDir = lightPos.xyz - ecPos;
	dist = length(lightDir);
	l = normalize(lightDir);
	spotDot = dot(normalize(spotDir), -l);
	if (Debug == false)
		if (dist > lightRadius || spotDot < spotCosCutoff)
			discard;

	gl_FragColor = vec4(0.0);
//	spotAtt = spotDot - spotCosCutoff;
	spotAtt = pow(spotDot, 12.0);
	att = spotAtt * clamp(1.0 - dist/lightRadius, 0.0, 1.0) * lightIntensity;

	NdotL = max(dot(n, l), 0.0);

	if (NdotL > 0.0) {
		gl_FragColor += att * (lightColor * NdotL);
		r = reflect(-l, n);
		RdotE = max(dot(r, normalize(-ecPos)), 0.0);
		gl_FragColor += vec4(att * lightColor * pow(RdotE, 16.0));
	}

	gl_FragColor *= colorMap;

	if (Debug) {	
		gl_FragColor = vec4(att);
		if (dist > lightRadius - 0.005 && dist < lightRadius + 0.005)
			gl_FragColor = vec4(1.0, 0.0, 0.0, 0.0);
		if ((spotDot > spotCosCutoff - 0.001) &&  (spotDot < spotCosCutoff + 0.001))
			gl_FragColor = vec4(0.0, 1.0, 0.0, 0.0);
	}		
}
