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

private:
	std::vector<float> queue;
	int maxQueue;
};
