#include "graph.hpp"

#include <numeric>

Graph::Graph(int maxQueue)
	: maxQueue(maxQueue)
{

}

void Graph::Push(float value)
{
	queue.insert(queue.begin(), value);
	while (queue.size() > maxQueue) {
		queue.pop_back();
	}

	float min = 0, max = 0, sum = 0;
	for (auto& n : queue) {
		if (n < min) min = n;
		if (n > max) max = n;
		sum += n;
	}

	this->statMin = min;
	this->statMax = max;
	this->statSum = sum;
	this->statAverage = sum / queue.size();
}

const float* Graph::Data() const
{
	return queue.data(); 
}

int Graph::Size() const
{ 
	return (int)queue.size(); 
}
