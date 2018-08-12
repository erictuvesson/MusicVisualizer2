

//--- 2D Helpers ---

vec2 screen_center() {
	vec2 res = iResolution.xy / iResolution.y;
	return (gl_FragCoord.xy / iResolution.y) - res / 2.0;
}

//-----------------
