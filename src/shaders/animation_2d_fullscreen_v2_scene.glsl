//! #include "func/helpers_2d.glsl"
//! #include "func/shape_2d.glsl"
//! #include "func/animation.glsl"

void main(void) {
	vec2 center = screen_center() * 2;
	
	float time = iTime; 
	time = mod(time, 3.0);

	center *= rotate(anim_linear(0.0, 3.0, time) * pi * 2.0)

	vec3 backgroundColor = vec3(0, 0, 0);
	vec3 heart = shape2d_heart1(center);
	vec3 heartColor = vec3(1.0, 0.5 * heart.y, 0.3);
	
	vec3 color = mix(backgroundColor, heartColor, heart.x);

	gl_FragColor = vec4(color, 1.0);
}
