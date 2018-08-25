
void main(void) {
	float fft_size = 256;

	float delta = iResolution.x / fft_size;
	float index = floor(
		(gl_FragCoord.x / delta)
	);

	float color = texture(iSample, vec2(10, 0)).r * 100;
	float color2 = texture(iSample, vec2(index, 0)).r;

	gl_FragColor = vec4(color, color2, textureSize(iSample, 0).x / 256, 1);
}
