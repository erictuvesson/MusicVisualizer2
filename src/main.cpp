#include "app.hpp"

#include <iostream>

int main(int argc, char *argv[])
{
	App app;
	
	const int result = app.Run();
	std::cin.get();

	return result;
}
