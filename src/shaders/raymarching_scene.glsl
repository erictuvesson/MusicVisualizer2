//! #include "func/color.glsl"

#define PI 3.1415926535897932384626433832795
#define TAU 6.283185307179586476925286766559
#define t iTime

mat2 rz2 (float a) { float c=cos(a), s=sin(a); return mat2(c,s,-s,c); }
float sphere (vec3 p, float r) { return length(p)-r; }
float iso (vec3 p, float r) { return dot(p, normalize(sign(p)))-r; }
float cyl (vec2 p, float r) { return length(p)-r; }
float cube (vec3 p, vec3 r) { return length(max(abs(p)-r,0.)); }

vec2 modA (vec2 p, float count) {
	float an = TAU/count;
	float a = atan(p.y,p.x)+an*.5;
	a = mod(a, an)-an*.5;
	return vec2(cos(a),sin(a))*length(p);
}

float smin (float a, float b, float r)
{
	float h = clamp(.5+.5*(b-a)/r,0.,1.);
	return mix(b, a, h) - r*h*(1.-h);
}

float map (vec3 p)
{
	float sph3 = sphere(p, 3.);
	p.yz *= rz2(t*.2);
	p.xy *= rz2(t*.3);
	
	float d = length(p);
	
	float a = atan(p.y,p.x);
	float l = length(p.xy)-2.;
	p.xy = vec2(l,a);
	
	float as = PI*0.3;
	p.z += sin(a*2.+sin(l*4.))*.5;
	
	float wave1 = sin(p.y*6.)*.5+.5;
	float wave2 = .5+.5*sin(p.z*3.+t);
	
	p.x -= sin(p.z*1.+t)*.5;
	p.z = mod(p.z+t,as)-as*.5;
	
	float sphR = .2-.1*wave1;
	float sphC = .3;
	float sphN = 0.2;
	float sph1 = sphere(vec3(p.x,mod(sphN*p.y/TAU+t*.1,sphC)-sphC*.5,p.z), sphR);
	
	p.xz *= rz2(p.y*3.);
	p.xz = modA(p.xz, 3.);
	p.x -= 0.3*wave2;
	float cyl1 = cyl(p.xz, 0.02);
	float sph2 = sphere(vec3(p.x,mod(p.y*2.-t,1.)-.5,p.z), .1);
	
	return smin(sph1, smin(cyl1,sph2,.2), .2);
}

void main(void)
{
	vec2 uv = (gl_FragCoord.xy - .5 * iResolution.xy) / iResolution.y;
	vec3 ro = vec3(uv,-5), rp = vec3(uv, 1), mp = ro;
	int i = 0;

	const int count = 50;
	for(; i < count; ++i) {
		float md = map(mp);
		if (md < 0.001) {
			break;
		}
		mp += rp * md * .35;
	}

	float r = float(i) / float(count);
	
	gl_FragColor = vec4(1.0) - iThemeColor[0];
	gl_FragColor *= smoothstep(.0,10.,length(mp-ro)) * iThemeColor[1];
	gl_FragColor *= r;
	gl_FragColor = 1. - gl_FragColor;
}
