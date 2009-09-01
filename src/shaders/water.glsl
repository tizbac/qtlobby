[VertexShader]
uniform float time;
varying vec3 v_texCoord3D;
varying vec4 diffuse,ambientGlobal,ambient;
varying vec3 normal,lightDir,halfVector;
varying float dist;
uniform int lightSource;
	
void main()	{	
	vec4 ecPos;
	vec3 aux;
		
	normal = normalize(gl_NormalMatrix * gl_Normal);
		
	ecPos = gl_ModelViewMatrix * gl_Vertex;
	gl_ClipVertex = ecPos;
		
	v_texCoord3D = gl_Vertex.xyz;
		
	aux = vec3(gl_LightSource[lightSource].position-ecPos);
	lightDir = normalize(aux);
	dist = length(aux);
	
	halfVector = normalize(gl_LightSource[lightSource].halfVector.xyz);
		
	diffuse = gl_FrontMaterial.diffuse * gl_LightSource[lightSource].diffuse;
		
	ambient = gl_FrontMaterial.ambient * gl_LightSource[lightSource].ambient;
	ambientGlobal = gl_LightModel.ambient * gl_FrontMaterial.ambient;
		
			
	gl_Position = ftransform();
}
[FragmentShader]
uniform sampler2D permTexture;
//uniform sampler2D reflectionTexture;
uniform float time; // Used for texture animation
uniform int lightSource;
varying vec4 diffuse,ambientGlobal, ambient;
varying vec3 normal,lightDir,halfVector;
varying float dist;
varying vec3 v_texCoord3D;
varying vec3 v_texCoord2D;

#define ONE 0.00390625
#define ONEHALF 0.001953125


float fade(float t) {
  // return t*t*(3.0-2.0*t); // Old fade, yields discontinuous second derivative
  return t*t*t*(t*(t*6.0-15.0)+10.0); // Improved fade, yields C2-continuous noise
}


float noise(vec3 P) {
  vec3 Pi = ONE*floor(P)+ONEHALF; // Integer part, scaled so +1 moves one texel
                                  // and offset 1/2 texel to sample texel centers
  vec3 Pf = fract(P);     // Fractional part for interpolation

  // Noise contributions from (x=0, y=0), z=0 and z=1
  float perm00 = texture2D(permTexture, Pi.xy).a ;
  vec3  grad000 = texture2D(permTexture, vec2(perm00, Pi.z)).rgb * 4.0 - 1.0;
  float n000 = dot(grad000, Pf);
  vec3  grad001 = texture2D(permTexture, vec2(perm00, Pi.z + ONE)).rgb * 4.0 - 1.0;
  float n001 = dot(grad001, Pf - vec3(0.0, 0.0, 1.0));

  // Noise contributions from (x=0, y=1), z=0 and z=1
  float perm01 = texture2D(permTexture, Pi.xy + vec2(0.0, ONE)).a ;
  vec3  grad010 = texture2D(permTexture, vec2(perm01, Pi.z)).rgb * 4.0 - 1.0;
  float n010 = dot(grad010, Pf - vec3(0.0, 1.0, 0.0));
  vec3  grad011 = texture2D(permTexture, vec2(perm01, Pi.z + ONE)).rgb * 4.0 - 1.0;
  float n011 = dot(grad011, Pf - vec3(0.0, 1.0, 1.0));

  // Noise contributions from (x=1, y=0), z=0 and z=1
  float perm10 = texture2D(permTexture, Pi.xy + vec2(ONE, 0.0)).a ;
  vec3  grad100 = texture2D(permTexture, vec2(perm10, Pi.z)).rgb * 4.0 - 1.0;
  float n100 = dot(grad100, Pf - vec3(1.0, 0.0, 0.0));
  vec3  grad101 = texture2D(permTexture, vec2(perm10, Pi.z + ONE)).rgb * 4.0 - 1.0;
  float n101 = dot(grad101, Pf - vec3(1.0, 0.0, 1.0));

  // Noise contributions from (x=1, y=1), z=0 and z=1
  float perm11 = texture2D(permTexture, Pi.xy + vec2(ONE, ONE)).a ;
  vec3  grad110 = texture2D(permTexture, vec2(perm11, Pi.z)).rgb * 4.0 - 1.0;
  float n110 = dot(grad110, Pf - vec3(1.0, 1.0, 0.0));
  vec3  grad111 = texture2D(permTexture, vec2(perm11, Pi.z + ONE)).rgb * 4.0 - 1.0;
  float n111 = dot(grad111, Pf - vec3(1.0, 1.0, 1.0));

  // Blend contributions along x
  vec4 n_x = mix(vec4(n000, n001, n010, n011),
                 vec4(n100, n101, n110, n111), fade(Pf.x));

  // Blend contributions along y
  vec2 n_xy = mix(n_x.xy, n_x.zw, fade(Pf.y));

  // Blend contributions along z
  float n_xyz = mix(n_xy.x, n_xy.y, fade(Pf.z));

  // We're done, return the final noise value.
  return n_xyz;
}
	
	
float F(vec3 v) {
	float t = time;
	//float theta = 100 + 20*sin(t);//sin(t)+t/sin(t);
	/*vec3 u = vec3(1,0,0);
	float c = cos(theta);
	float s = sin(theta);
	mat3 rot = mat3(u.x*u.x+(1.0-u.x*u.x)*c, u.x*u.y*(1.0-c)-u.z*s, u.x*u.z*(1.0-c)+u.y*s,
                    u.x*u.y*(1.0-c)+u.z*s, u.y*u.y+(1.0-u.y*u.y)*c, u.y*u.z*(1.0-c)-u.x*s,
                    u.x*u.z*(1.0-c)-u.y*s, u.y*u.z*(1.0-c)+u.x*s, u.z*u.z+(1.0-u.z*u.z)*c);*/
	float d = 0.3*t;
	v += vec3(d,-d,d);
	return (noise(v))/1.5;
}


void main() {
	vec3 n,halfV,viewV,ldir;
	float NdotL,NdotHV;
	//vec4 reflectionColor = texture2D(reflectionTexture,gl_TexCoord[0].st).rgba;
	vec4 color = ambientGlobal;
	float att;
		
	n = normalize(normal);
	float F0, Fx, Fy, Fz;
	float e = 5.0;
	F0 = F(v_texCoord3D.xyz);
	Fx = F(vec3(v_texCoord3D.x+e,v_texCoord3D.y,v_texCoord3D.z));
	Fy = F(vec3(v_texCoord3D.x,v_texCoord3D.y+e,v_texCoord3D.z));
	Fz = F(vec3(v_texCoord3D.x,v_texCoord3D.y,v_texCoord3D.z+e));
	vec3 dF = vec3((Fx-F0)/e,(Fy-F0)/e,(Fz-F0)/e);
	n = normalize(n - dF);
	//n.y = abs(n.y);
		
	NdotL = max(dot(n,normalize(lightDir)),0.0);
	
	if (NdotL > 0.0) {
		
		att = 1.0 / (gl_LightSource[lightSource].constantAttenuation +
				gl_LightSource[lightSource].linearAttenuation * dist +
				gl_LightSource[lightSource].quadraticAttenuation * dist * dist);
		color += att * (diffuse * NdotL + ambient);
		
			
		halfV = normalize(halfVector);
		NdotHV = max(dot(n,halfV),0.0);
		color += att * gl_FrontMaterial.specular * gl_LightSource[lightSource].specular * 
						pow(NdotHV,gl_FrontMaterial.shininess);
	}
	//color*=reflectionColor;
	gl_FragColor = vec4(color.rgb, 0.3);
}


[Parameters]
int lightSource = 0;
sampler2D permTexture = load("../../../../../../tmp/permutations.png");
sampler2D reflectionTexture = load("");
