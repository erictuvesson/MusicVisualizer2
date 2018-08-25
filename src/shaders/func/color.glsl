// https://github.com/jamieowen/glsl-blend

//--- Color Functions ---

float color_blendAdd(float base, float blend) {
	return min(base + blend, 1.0);
}

vec3 color_blendAdd(vec3 base, vec3 blend) {
	return min(base + blend, vec3(1.0));
}

vec3 color_blendAdd(vec3 base, vec3 blend, float opacity) {
	return (color_blendAdd(base, blend) * opacity + base * (1.0 - opacity));
}

vec3 color_blendAverage(vec3 base, vec3 blend) {
	return (base + blend) / 2.0;
}

vec3 color_blendAverage(vec3 base, vec3 blend, float opacity) {
	return (color_blendAverage(base, blend) * opacity + base * (1.0 - opacity));
}

float color_blendColorBurn(float base, float blend) {
	return (blend == 0.0) ? blend : max((1.0 - ((1.0 - base) / blend)), 0.0);
}

vec3 color_blendColorBurn(vec3 base, vec3 blend) {
	return vec3(
		color_blendColorBurn(base.r, blend.r),
		color_blendColorBurn(base.g, blend.g),
		color_blendColorBurn(base.b, blend.b)
	);
}

vec3 color_blendColorBurn(vec3 base, vec3 blend, float opacity) {
	return (color_blendColorBurn(base, blend) * opacity + base * (1.0 - opacity));
}

float color_blendColorDodge(float base, float blend) {
	return (blend == 1.0) ? blend : min(base / (1.0 - blend), 1.0);
}

vec3 color_blendColorDodge(vec3 base, vec3 blend) {
	return vec3(
		color_blendColorDodge(base.r, blend.r),
		color_blendColorDodge(base.g, blend.g),
		color_blendColorDodge(base.b, blend.b)
	);
}

vec3 color_blendColorDodge(vec3 base, vec3 blend, float opacity) {
	return (color_blendColorDodge(base, blend) * opacity + base * (1.0 - opacity));
}

float color_blendLighten(float base, float blend) {
	return max(blend, base);
}

vec3 color_blendLighten(vec3 base, vec3 blend) {
	return vec3(
		color_blendLighten(base.r, blend.r),
		color_blendLighten(base.g, blend.g),
		color_blendLighten(base.b, blend.b)
	);
}

vec3 color_blendLighten(vec3 base, vec3 blend, float opacity) {
	return (color_blendLighten(base, blend) * opacity + base * (1.0 - opacity));
}

//---------------------------
