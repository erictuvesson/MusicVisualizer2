//! #include "func/helpers_2d.glsl"
//! #include "func/shape_2d.glsl"

float stepUpDown(float begin, float end, float t) {
  return step(begin, t) - step(end, t);
}

void main(void) {
	float shortSide = min( iResolution.x, iResolution.y );
	vec2 uv = gl_FragCoord.xy / shortSide;
	vec2 center = iResolution.xy / (shortSide * 2.);
	uv -= center;
	
	float period = mod(iTime, 8.0);
	
	float sides = stepUpDown( 0.0, 1.0, period ) * 3.0
				+ stepUpDown( 1.0, 2.0, period ) * 4.0
				+ stepUpDown( 2.0, 3.0, period ) * 5.0
				+ stepUpDown( 3.0, 4.0, period ) * 6.0
				+ stepUpDown( 4.0, 5.0, period ) * 8.0
				+ stepUpDown( 5.0, 6.0, period ) * 10.0
				+ stepUpDown( 6.0, 7.0, period ) * 12.0
				+ stepUpDown( 7.0, 8.0, period ) * 14.0;
	
	period = mod(iTime, 12.0);
	vec3 color = stepUpDown( 0.0, 1.0, period ) * vec3(0.389,0.266,1.000)
				+ stepUpDown( 1.0, 3.0, period ) * vec3(1.000,0.935,0.334)
				+ stepUpDown( 3.0, 5.0, period ) * vec3(0.443,0.665,0.297)
				+ stepUpDown( 5.0, 7.0, period ) * vec3(0.900,0.200,0.410)
				+ stepUpDown( 7.0, 9.0, period ) * vec3(0.100,0.095,0.350)
				+ stepUpDown( 9.0, 11.0, period ) * vec3(0.900,0.423,0.168)
				+ stepUpDown( 11.0, 12.0, period ) * vec3(0.389,0.266,1.000);
	
	gl_FragColor = vec4(color, 0.0);
		
	float d = shape2d_polygon(uv, sides, fract(iTime) * fract(iTime));
	
	if (mod(floor(iTime), 2.0) == 0.0) {
		d = 1.0 - d;
	}
	
	color = d * vec3(1.0);
	
	gl_FragColor += vec4(color, 1.0);
}
