
//--- 2D Shapes ---

#define PI			3.14159265359
#define PI2			(PI * 2)
#define PIOver2		(PI / 2)
#define PIOver4		(PI / 4)
#define TAU			6.28318530718

#define polar(a) vec2(cos(a),sin(a))
#define rotate(a) mat2(cos(a),sin(a),-sin(a),cos(a));

const float pi = atan(1.0) * 4.0; // TODO: REMOVE

const vec2 hex0 = polar((1.0 * PI) / 6.0);
const vec2 hex1 = polar((3.0 * PI) / 6.0);
const vec2 hex2 = polar((5.0 * PI) / 6.0);

float shape2d_hexagon(vec2 uv, float r) {
  return max(max(abs(dot(uv, hex0)), abs(dot(uv, hex1))), abs(dot(uv, hex2))) - r;
}

float shape2d_circle(vec2 uv, float r) {
  return length(uv) - r;
}

float shape2d_polygon(vec2 st, float sides, float radius) {
  float a = atan(st.x, st.y) + PI;
  float r = TAU / sides;
  float d = cos(floor(0.5 + a / r) * r - a) * length(st);
  return smoothstep(radius - 0.003, radius + 0.003, d);
}

vec3 shape2d_heart1(vec2 uv) {
  uv *= 0.8;
  uv.y = -0.1 - uv.y * 1.2 + abs(uv.x) * (1.0 - abs(uv.x));
  float r = length(uv);
  float d = 0.5;
  return vec3(smoothstep(-0.01, 0.01, d - r), r, d);
}

vec3 shape2d_heart2(vec2 uv) {
  uv.y -= 0.25;
  float a = atan(uv.x, uv.y) / PI;
  float r = length(uv);
  float h = abs(a);
  float d = (13.0 * h - 22.0 * h * h + 10.0 * h * h * h) / (6.0 - 5.0 * h);
  return vec3(smoothstep(-0.01, 0.01, d - r), r, d);
}

//-----------------
