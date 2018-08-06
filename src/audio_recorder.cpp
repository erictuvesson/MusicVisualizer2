#include "audio_recorder.hpp"

static BOOL bassRecordProc(HRECORD handle, const void *buffer, DWORD length, void *user)
{
	// I am not planning on using this as there is no benefit.
	return true;
}

AudioRecorder::AudioRecorder()
	: device({ -1 })
	, running(std::make_unique<bool>(true))
{

}

AudioRecorder::~AudioRecorder()
{
	Destroy();
}

std::vector<AudioDevice> AudioRecorder::GetDevices() const
{
	std::vector<AudioDevice> devices;

	int n = 0;
	BASS_DEVICEINFO info;
	while (BASS_RecordGetDeviceInfo(n, &info)) {
		devices.push_back({ n, info });
		n++;
	}

	return devices;
}

const AudioSample* AudioRecorder::GetSample() const
{ 
	return sample.get();
}

bool AudioRecorder::Listen(const AudioDevice& device)
{
	if (this->device.index != -1) {
		destroy_thread();
		BASS_RecordFree();
	}

	if (!BASS_RecordInit(device.index)) {
		return false;
	}

	this->device = device;
	this->thread = std::thread(&AudioRecorder::process_audio, this);
	return true;
}

void AudioRecorder::Destroy()
{
	destroy_thread();
	BASS_RecordFree();
}

void AudioRecorder::destroy_thread()
{
	if (thread.joinable()) {
		running = false;
		thread.join();
	}
}

void AudioRecorder::process_audio()
{
	AudioSample sample;
	int32_t i = 0;

	DWORD recHandle = BASS_RecordStart(44100, 2, 0, &bassRecordProc, 0);
	while (running && BASS_ChannelGetData(recHandle, sample.fft, BASS_DATA_FFT512) > 0)
	{
		sample.sum = 0;

		for (i = 0; i < 256; i++) {
			sample.sum += sample.fft[i];
		}

		sample.average = sample.sum != 0 ? sample.sum / 256 : 0;

		m.lock();
		this->sample = std::make_unique<AudioSample>(sample);
		m.unlock();
	}
}

bool AudioRecorder::IsValid(std::string& message)
{
	if (HIWORD(BASS_GetVersion()) != BASSVERSION) {
		message = "An incorrect version of BASS.DLL was loaded";
		return false;
	}
	return true;
}
