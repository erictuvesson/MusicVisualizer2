#pragma once

#include <vector>

/**
 * Graph, basically just a queue.
 */
class Graph
{
public:
	Graph(int maxQueue = 300);

	void Push(float value);

	const float* Data() const;
	int Size() const;

	float Min() const { return statMin; }
	float Max() const { return statMax; }
	float Sum() const { return statSum; }
	float Average() const { return statAverage; }

private:
	std::vector<float> queue;
	int maxQueue;
	float statMin, statMax, statSum, statAverage;
};
