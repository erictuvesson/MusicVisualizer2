#include "curve.hpp"

Curve::Curve()
{

}

float Curve::At(float value)
{
	if (value < 0) {
		return 0;
	}
	else if (value > 0.025f) {
		return 0.025f;
	}

	return (value * value) / 2;
}
