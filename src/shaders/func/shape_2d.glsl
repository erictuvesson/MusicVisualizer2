
//--- 2D Shapes ---

#define polar(a) vec2(cos(a),sin(a))
#define rotate(a) mat2(cos(a),sin(a),-sin(a),cos(a));

const float pi = atan(1.0) * 4.0;

const vec2 hex0 = polar((1.0 * pi) / 6.0);
const vec2 hex1 = polar((3.0 * pi) / 6.0);
const vec2 hex2 = polar((5.0 * pi) / 6.0);

float hexagon(vec2 uv, float r) {
  return max(max(abs(dot(uv, hex0)), abs(dot(uv, hex1))), abs(dot(uv, hex2))) - r;
}

float circle(vec2 uv, float r) {
  return length(uv) - r;
}

//-----------------
