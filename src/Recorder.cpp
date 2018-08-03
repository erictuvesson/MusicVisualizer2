#include "Recorder.hpp"

#include <iostream>

static BOOL MyRecordProc(HRECORD handle, const void *buffer, DWORD length, void *user)
{
	return true;
}

Recorder::Recorder()
	: device({ -1 })
	, running(std::make_shared<bool>(true))
{
	if (HIWORD(BASS_GetVersion()) != BASSVERSION) {
		std::cout << "An incorrect version of BASS.DLL was loaded";
	}
}

Recorder::~Recorder()
{
	destroy_thread();
	BASS_RecordFree();
}

std::vector<RecordDevice> Recorder::GetDevices() const
{
	std::vector<RecordDevice> devices;

	int n = 0;
	BASS_DEVICEINFO info;
	while (BASS_RecordGetDeviceInfo(n, &info)) {
		devices.push_back({ n, info });
		n++;
	}

	return devices;
}

bool Recorder::Listen(const RecordDevice& device)
{
	if (this->device.index != -1) {
		destroy_thread();
		BASS_RecordFree();
	}

	if (!BASS_RecordInit(device.index)) {
		return false;
	}

	this->device = device;
	this->thread = std::thread(&Recorder::process, this);
	return true;
}

void Recorder::destroy_thread()
{
	if (thread.joinable()) {
		running = false;
		thread.join();
	}
}

void Recorder::process()
{
	Sample sample;
	float temp = 0;
	int32_t i = 0;

	DWORD _recHandle = BASS_RecordStart(44100, 2, 0, &MyRecordProc, 0);
	while (running && BASS_ChannelGetData(_recHandle, sample.fft, BASS_DATA_FFT512) > 0)
	{
		temp = 0;
		sample.total = 0;

		for (i = 0; i < 256; i++)
			sample.total += sample.fft[i];

		sample.sum = sample.total != 0 ? sample.total / 256 : 0;

		this->sample = std::make_shared<Sample>(sample);
	}
}
