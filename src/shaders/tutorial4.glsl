
//! #define "ANIM_FUNC", "AnimBounce", ANIMATIONS

void main(void) {
	vec3 audioSumColor = vec3(iAudioSum);

	float y = gl_FragCoord.y / iResolution.y;
	
	audioSumColor *= y / 2.;

	gl_FragColor = vec4(audioSumColor, 1);
}


