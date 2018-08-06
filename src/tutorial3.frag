
#define PI 3.14159265359
#define TAU 6.28318530718

float polygon( vec2 st, float sides, float radius ) {
	float a = atan(st.x,st.y) + PI;
	float r = TAU / sides;
	float d = cos( floor(0.5 + a / r) * r - a) * length(st);
	return smoothstep( radius - 0.003, radius + 0.003, d );
}

float stepUpDown(float begin, float end, float t) {
  return step(begin, t) - step(end, t);
}


void main(void) {
	float shortSide = min( iResolution.x, iResolution.y );
	vec2 uv = gl_FragCoord.xy / shortSide;
	vec2 center = iResolution.xy / (shortSide * 2.);
	uv -= center;
	
	float period = mod( iTime, 5.0);
	
	float sides = stepUpDown( 0.0, 1.0, period ) * 3.0
				+ stepUpDown( 1.0, 2.0, period ) * 4.0
				+ stepUpDown( 2.0, 3.0, period ) * 5.0
				+ stepUpDown( 3.0, 4.0, period ) * 6.0
				+ stepUpDown( 4.0, 5.0, period ) * 8.0;
	
	period = mod( iTime, 12.0);
	vec3 color = stepUpDown( 0.0, 1.0, period ) * vec3(0.389,0.266,1.000)
				+ stepUpDown( 1.0, 3.0, period ) * vec3(1.000,0.935,0.334)
				+ stepUpDown( 3.0, 5.0, period ) * vec3(0.443,0.665,0.297)
				+ stepUpDown( 5.0, 7.0, period ) * vec3(0.900,0.200,0.410)
				+ stepUpDown( 7.0, 9.0, period ) * vec3(0.100,0.095,0.350)
				+ stepUpDown( 9.0, 11.0, period ) * vec3(0.900,0.423,0.168)
				+ stepUpDown( 11.0, 12.0, period ) * vec3(0.389,0.266,1.000);
	
	gl_FragColor = vec4(color, 0.0);
		
	float d = polygon( uv, sides, fract(iTime) * fract(iTime) );
	
	if( mod(floor(iTime), 2.0) == 0.0 ) {
		d = 1.0 - d;
	}
	
	color = d * ( vec3(1.0) );
	
	gl_FragColor += vec4(color, 1.0);
}
