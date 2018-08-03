#pragma once

#include <vector>
#include <memory>
#include <thread>
#include <bass.h>

struct RecordDevice
{
	int32_t index;
	BASS_DEVICEINFO info;
};

struct Sample
{
	float fft[256];
	float total;
	float sum;
};

class Recorder
{
public:
	std::shared_ptr<Sample> sample;

public:
	Recorder();
	virtual ~Recorder();

	std::vector<RecordDevice> GetDevices() const;

	bool Listen(const RecordDevice& device);

private:
	void destroy_thread();
	void process();

private:
	RecordDevice device;
	std::thread thread;
	std::shared_ptr<bool> running;
};
