#include "audio_recorder.hpp"

#include <iostream>

static BOOL bassRecordProc(HRECORD handle, const void *buffer, DWORD length, void *user)
{
	// I am not planning on using this as there is no benefit.
	return true;
}

#if WASAPI
static DWORD DUMMY_BASS_WASAPI_Proc(void* buffer, DWORD length, void* user)
{
	return length;
}
#endif

AudioRecorder::AudioRecorder()
	: device({ -1 })
	, running(true)
	, initialized(false)
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
	
#if WASAPI
	BASS_WASAPI_DEVICEINFO info;
	while (BASS_WASAPI_GetDeviceInfo(n, &info))
	{
		if ((info.flags & BASS_DEVICE_ENABLED) > 0
		 && (info.flags & BASS_DEVICE_LOOPBACK) > 0)
		{
			AudioDevice device {};
			device.index = n;
			device.name = info.name;
			device.driver = info.id;
			device.type = (AudioDeviceType)info.type;
			device.flag_default = (info.flags & BASS_DEVICE_DEFAULT) > 0;
			device.freq = info.mixfreq;
			device.channels = info.mixchans;
		
			devices.push_back(device);
		}
		n++;
	}
#else
	BASS_DEVICEINFO info;
	while (BASS_GetDeviceInfo(n, &info))
	{
		AudioDevice device {};
		device.index = n;
		device.name = info.name;
		device.driver = info.driver;

		devices.push_back(device);
		n++;
	}
#endif

	return devices;
}

const AudioSample* AudioRecorder::GetSample() const
{ 
	return sample.get();
}

bool AudioRecorder::Listen(const AudioDevice& device)
{
	// It is probably the same audio device.
	if (this->device.index == device.index)
	{
		return true;
	}

	if (this->device.index != -1)
	{
		destroy_thread();
	}

#if WASAPI
	if (!this->initialized)
	{
		// Initialize the "No sound"
		BASS_SetConfig(BASS_CONFIG_UPDATETHREADS, false);
		if (!BASS_Init(0, 44100, BASS_DEVICE_DEFAULT, NULL, NULL))
		{
			std::cout << "BASS ERROR: " << BASS_ErrorGetCode() << std::endl;
			return false;
		}
		this->initialized = true;
	}

	if (!BASS_WASAPI_Init(device.index, 44100, 2, BASS_WASAPI_BUFFER, 1.0f, 0.05f, &DUMMY_BASS_WASAPI_Proc, NULL))
	{
		std::cout << "BASS ERROR: " << BASS_ErrorGetCode() << std::endl;
		return false;
	}

	if (!BASS_WASAPI_Start())
	{
		std::cout << "BASS ERROR: " << BASS_ErrorGetCode() << std::endl;
		return false;
	}
#else
	if (!BASS_RecordInit(device.index))
	{
		// TODO: Handle initialize failure
		return false;
	}
#endif

	this->running = true;
	this->device = device;
	this->thread = std::thread(&AudioRecorder::process_audio, this);
	return true;
}

void AudioRecorder::Destroy()
{
	destroy_thread();

#if WASAPI
	BASS_WASAPI_Stop(false);
	BASS_WASAPI_Free();
#endif

	BASS_RecordFree();
	BASS_Free();
}

void AudioRecorder::destroy_thread()
{
	if (thread.joinable())
	{
		running = false;
		thread.join();
	}
}

void AudioRecorder::process_audio()
{
	AudioSample sample;
	int32_t i = 0;

#if WASAPI
	while (running && BASS_WASAPI_GetData(sample.fft, BASS_DATA_FFT512) > 0)
	{
		int error = BASS_ErrorGetCode();
		if (error != BASS_OK)
		{
			std::cout << "BASS ERROR: " << error << std::endl;
		}

		sample.sum = 0;

		for (i = 0; i < 256; i++)
		{
			sample.sum += sample.fft[i];
		}

		sample.average = sample.sum != 0 ? sample.sum / 256 : 0;

		m.lock();
		this->sample = std::make_unique<AudioSample>(sample);
		m.unlock();

		DWORD level = BASS_WASAPI_GetLevel();
		std::cout << "BASS LEVEL: " << level << std::endl;
	}

	if (!BASS_WASAPI_Stop(true))
	{
		std::cout << "BASS ERROR: " << BASS_ErrorGetCode() << std::endl;
	}
#else
	DWORD recHandle = BASS_RecordStart(44100, 2, 0, &bassRecordProc, 0);
	while (running && BASS_ChannelGetData(recHandle, sample.fft, BASS_DATA_FFT512) > 0)
	{
		sample.sum = 0;
	
		for (i = 0; i < 256; i++)
		{
			sample.sum += sample.fft[i];
		}
	
		sample.average = sample.sum != 0 ? sample.sum / 256 : 0;
	
		m.lock();
		this->sample = std::make_unique<AudioSample>(sample);
		m.unlock();
	}
#endif
}

bool AudioRecorder::IsValid(std::string& message)
{
	if (HIWORD(BASS_GetVersion()) != BASSVERSION)
	{
		message = "An incorrect version of BASS.DLL was loaded";
		return false;
	}
	return true;
}
