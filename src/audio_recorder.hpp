#pragma once

#include <vector>
#include <memory>
#include <thread>
#include <bass.h>

/**
 * AudioDevice, a detected audio device that can be captured.
 */
struct AudioDevice
{
	int32_t index;
	BASS_DEVICEINFO info;
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
	const std::shared_ptr<AudioSample> GetSample() const;

	/**
	 * Set the current device we are going to listen to.
	 */
	bool Listen(const AudioDevice& device);

	/**
	 * Check if we can use the audio system.
	 */
	static bool IsValid(std::string& message);

private:
	void destroy_thread();
	void process_audio();

private:
	AudioDevice device;
	std::thread thread;
	std::shared_ptr<bool> running;
	std::shared_ptr<AudioSample> sample;
};
