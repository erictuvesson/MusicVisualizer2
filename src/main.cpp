
#include <iostream>

#include "Recorder.hpp"

int main() 
{
	Recorder recorder;

	auto devices = recorder.GetDevices();
	recorder.Listen(devices[0]);

	while (true) {
		if (recorder.sample) {
			printf("%f, %f\n", recorder.sample->total, recorder.sample->sum);
		}

		Sleep(100);
	}

	std::cin.get();
	return 0;
}
