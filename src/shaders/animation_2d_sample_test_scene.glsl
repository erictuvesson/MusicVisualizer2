//! #include "func/color.glsl"

void main(void) {
	float fft_size = 256;

	float delta = iResolution.x / fft_size;
	float index = floor(
		(gl_FragCoord.x / delta)
	);

	
	if (textureSize(iSample, 0).x > 128) {
		float color1 = texture2D(iSample, vec2(mod(gl_FragCoord.x / 10, 256), 0)).r;
		float color2 = texture(iSample, vec2(index, 0)).r;

		if (color1 > 0.001) {
			gl_FragColor = vec4(color1, color2, 0, 1);
		} else {
			gl_FragColor = vec4(0, 0, 1, 1);
		}
	} else {
		gl_FragColor = vec4(1, 1, 1, 1);
	}
}
