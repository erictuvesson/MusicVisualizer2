#include "curve.hpp"

Curve::Curve(float min, float max)
	: min(min)
	, max(max)
{

}

float Curve::At(float value, float speed)
{
	if (value < min) {
		return min;
	}
	else if (value > max) {
		return max;
	}

	const float target = (value * value) / 8;
	return current = (current + (target - current) * speed);
}
