//! #variable ANIMATIONS, ["AnimOverShoot", "AnimSpring", "AnimBounce", "AnimQuart", "AnimLinear", "AnimQuartSine", "AnimHalfSine"]

//--- Animation Functions ---

float AnimOverShoot(float s, float e, float t) {
  return smoothstep(s, e, t) + sin(smoothstep(s, e, t) * pi) * 0.5;
}

float AnimSpring(float s, float e, float t) {
  t = clamp((t - s) / (e - s), 0.0, 1.0);
  return 1.0 - cos(t * pi * 6.0) * exp(-t * 6.5);
}

float AnimBounce(float s, float e, float t) {
  t = clamp((t - s) / (e - s), 0.0, 1.0);
  return 1.0 - abs(cos(t * pi * 4.0)) * exp(-t * 6.0);
}

float AnimQuart(float s, float e, float t) {
  t = clamp((t - s) / (e - s), 0.0, 1.0);
  return 1.0 - pow(1.0 - t, 4.0);
}

float AnimLinear(float s, float e, float t) {
  return clamp((t - s) / (e - s), 0.0, 1.0);
}

float AnimQuartSine(float s, float e, float t) {
  t = clamp((t - s) / (e - s), 0.0, 1.0);
  return sin(t * pi / 2.0);
}

float AnimHalfSine(float s, float e, float t) {
  t = clamp((t - s) / (e - s), 0.0, 1.0);
  return 1.0 - cos(t * pi) * 0.5 + 0.5;
}

//---------------------------
