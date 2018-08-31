//! #include "func/math_helper.glsl"

void main(void) {
	vec2 center = iResolution.xy / 2;

	float y = 0;
	if (gl_FragCoord.y > center.y - 10 && 
		gl_FragCoord.y < center.y + 10) {
		y = 1 - sin(gl_FragCoord.x);
	}

	gl_FragColor = vec4(y, 0, 0, 1);
}
