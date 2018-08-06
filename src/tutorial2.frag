
void main(void) {
	gl_FragColor = vec4(gl_FragCoord.xy / iResolution.y * iTime, 0, 1);
}
