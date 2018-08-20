//! #include "func/helpers_2d.glsl"
//! #include "func/shape_2d.glsl"
//! #include "func/animation.glsl"

#define HEART_SPEED 5

void main(void) {
	vec2 center = screen_center() * 10;
	
	vec3 color = vec3(0.8);

	for (int i = 0; i < 15; i++) {
		vec2 newCenter = center;

		newCenter *= rotate(anim_linear(0.0, HEART_SPEED, mod(iAudioTime + i * 0.333, HEART_SPEED)) * PI * 2.0);
		newCenter += vec2(3, 0);
		newCenter *= rotate(PIOver2);

		vec3 heart = shape2d_heart1(newCenter);
		vec3 heartColor = vec3(1.0, 0.5 * heart.y, 0.3);

		color = mix(color, heartColor, heart.x);
	}
	
	for (int i = 0; i < 5; i++) {
		vec2 newCenter = center;

		newCenter *= rotate(anim_linear(0.0, HEART_SPEED, mod(iTime + i, HEART_SPEED)) * PI * 2.0);
		newCenter += vec2(1, 0);
		newCenter *= rotate(PIOver2);

		vec3 heart = shape2d_heart1(newCenter);
		vec3 heartColor = vec3(1.0, 0.5 * heart.y, 0.3);

		color = mix(color, heartColor, heart.x);
	}

	gl_FragColor = vec4(color, 1.0);
}
