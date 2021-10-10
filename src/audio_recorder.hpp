#pragma once

#define WASAPI true

#include <vector>
#include <memory>
#include <thread>
#include <mutex>
#include <bass.h>
#include <basswasapi.h>

enum AudioDeviceType
{
	NETWORKDEVICE	= 0,
	SPEAKERS		= 1,
	LINELEVEL		= 2,
	HEADPHONES		= 3,
	MICROPHONE		= 4,
	HEADSET			= 5,
	HANDSET			= 6,
	DIGITAL			= 7,
	SPDIF			= 8,
	HDMI			= 9,
	UNKNOWN			= 10
};

/**
 * AudioDevice, a detected audio device that can be captured.
 */
struct AudioDevice
{
	int32_t index;

	const char* name;	// description
	const char* driver;	// driver

	AudioDeviceType type;
	bool flag_default;
	int32_t freq;
	int32_t channels;
};

/**
 * AudioSample, analyzed audio data by AudioRecorder.
 */
struct AudioSample
{
	float fft[256];
	float sum;
	float average;
};

/**
 * AudioRecorder, analyze and capture audio input.
 * Captures audio from recording devices.
 */
class AudioRecorder
{
public:
	AudioRecorder();
	virtual ~AudioRecorder();

	/**
	 * Get all the detected audio devices on the current device.
	 */
	std::vector<AudioDevice> GetDevices() const;

	/**
	 * Get the current sample.
	 */
	const AudioSample* GetSample() const;

	/**
	 * Set the current device we are going to listen to.
	 */
	bool Listen(const AudioDevice& device);

	void Destroy();

	/**
	 * Check if we can use the audio system.
	 */
	static bool IsValid(std::string& message);

private:
	void destroy_thread();
	void process_audio();

private:
	bool running;
	bool initialized;
	AudioDevice device;
	std::thread thread;
	std::unique_ptr<AudioSample> sample;
	std::mutex m;
};
