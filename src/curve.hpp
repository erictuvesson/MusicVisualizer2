#pragma once

/**
 * 
 */
class Curve
{
public:
	/**
	 * 
	 */
	Curve(float min = 0.0f, float max = 1.0f);

	/**
	 * 
	 */
	float At(float value, float speed = 1.0f);

	const float Current() const { return current;  }

private:
	float min, max, current = 0.0f;
};

