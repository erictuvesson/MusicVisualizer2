

//--- 2D Helpers ---

#define PI 3.14159265359
#define TAU 6.28318530718

vec2 screen_center() {
	vec2 res = iResolution.xy / iResolution.y;
	return (gl_FragCoord.xy / iResolution.y) - res / 2.0;
}

//-----------------
