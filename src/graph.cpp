#include "graph.hpp"

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
}

const float* Graph::Data() const
{
	return queue.data(); 
}

int Graph::Size() const
{ 
	return (int)queue.size(); 
}
