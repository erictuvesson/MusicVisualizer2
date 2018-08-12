//! #variable ANIMATIONS, ["AnimOverShoot", "AnimSpring", "AnimBounce", "AnimQuart", "AnimLinear", "AnimQuartSine", "AnimHalfSine"]

//--- Animation Functions ---

float anim_overShoot(float s, float e, float t) {
  return smoothstep(s, e, t) + sin(smoothstep(s, e, t) * pi) * 0.5;
}

float anim_spring(float s, float e, float t) {
  t = clamp((t - s) / (e - s), 0.0, 1.0);
  return 1.0 - cos(t * pi * 6.0) * exp(-t * 6.5);
}

float anim_bounce(float s, float e, float t) {
  t = clamp((t - s) / (e - s), 0.0, 1.0);
  return 1.0 - abs(cos(t * pi * 4.0)) * exp(-t * 6.0);
}

float anim_quart(float s, float e, float t) {
  t = clamp((t - s) / (e - s), 0.0, 1.0);
  return 1.0 - pow(1.0 - t, 4.0);
}

float anim_linear(float s, float e, float t) {
  return clamp((t - s) / (e - s), 0.0, 1.0);
}

float anim_quartSine(float s, float e, float t) {
  t = clamp((t - s) / (e - s), 0.0, 1.0);
  return sin(t * pi / 2.0);
}

float anim_halfSine(float s, float e, float t) {
  t = clamp((t - s) / (e - s), 0.0, 1.0);
  return 1.0 - cos(t * pi) * 0.5 + 0.5;
}

//---------------------------
