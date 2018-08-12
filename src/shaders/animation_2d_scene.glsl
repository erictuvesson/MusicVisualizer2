//! #include "func/helpers_2d.glsl"
//! #include "func/shape_2d.glsl"
//! #include "func/animation.glsl"

#define ANIM_FUNC anim_bounce

void main(void)
{
	vec2 center = screen_center();

	float time = iAudioTime; // iTime;
	time = mod(time, 10.0);
	
	float hexrad = ANIM_FUNC(0.0, 1.0, time) - ANIM_FUNC(8.0, 9.0, time);
	hexrad = 0.1 * hexrad + 0.1;
	
	float df = shape2d_hexagon(center, hexrad);
	
	vec2 dirs[6];
	dirs[0] = hex0;
	dirs[1] = hex1;
	dirs[2] = hex2;
	dirs[3] = -hex0;
	dirs[4] = -hex1;
	dirs[5] = -hex2;
	
	center *= rotate(ANIM_FUNC(3.0, 6.0, time) * pi * 2.0)
	
	float coff = 0.0;
	for (int i = 0; i < 6; i++)
	{
		float open = 1.2 + 0.2 * float(i);
		float close = 6.0 + 0.2 * float(i);
		
		coff = ANIM_FUNC(open, open + 0.2, time) - ANIM_FUNC(close, close + 0.2, time);
		coff = coff * 0.35;
		
		df = min(df, shape2d_circle(center - dirs[i] * coff, 0.075));
	}
	
	vec3 color = vec3(0);
	color = vec3(smoothstep(0.005, 0.0, df) * 0.5 + 0.25);
	color /= iAudioSum;

	gl_FragColor = vec4(color, 1.0);
}
