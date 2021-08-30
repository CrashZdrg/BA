#include "BA.hpp"

#include <iostream>
#include <chrono>
#include <thread>

#include "zlib.h"

#ifndef NOAUDIO
#include "miniaudio.h"
#endif

constexpr unsigned int DEFLATEDDATASIZE = sizeof(ba);
constexpr unsigned int DATASIZE = 1635300;
constexpr unsigned int WIDTH = 80;
constexpr unsigned int HEIGHT = 30;
constexpr unsigned char PIXEL = '#';
constexpr unsigned int BACKBUFFERSIZE = ((WIDTH * HEIGHT) + HEIGHT) + 1; // ((W * H) + NewLines) + NullTermination

using chrono_clock = std::chrono::high_resolution_clock;
using chrono_duration = std::chrono::milliseconds;

constexpr auto FRAMETIME = chrono_duration(1000 / 25); // 1s / FPS

#ifdef USE_SPINWAIT
constexpr auto ONEMS = std::chrono::duration<unsigned int, std::milli>(1);
constexpr unsigned int SPINFACTOR = 50;
#endif

unsigned char dat[DATASIZE];

voidpf zcalloc(voidpf, unsigned items, unsigned size) { return malloc(items * size); }
void zcfree(voidpf, voidpf ptr) { free(ptr); }

void InitData()
{
	z_stream infStream
	{
		ba,
		DEFLATEDDATASIZE,
		0,
		dat,
		DATASIZE,
		0,
		Z_NULL,
		Z_NULL,
		zcalloc,
		zcfree,
		Z_NULL,
		0,
		0,
	};

	inflateInit(&infStream);
	inflate(&infStream, Z_FINISH);
	inflateEnd(&infStream);
}

#ifndef NOAUDIO
ma_device audioDevice;
ma_decoder audioDecoder;

void AudioCallback(ma_device*, void* pOutput, const void*, ma_uint32 frameCount)
{
	ma_decoder_read_pcm_frames(&audioDecoder, pOutput, frameCount);
}

void InitAudio()
{
	ma_decoder_init_memory_mp3(ba_audio, sizeof(ba_audio), nullptr, &audioDecoder);

	ma_device_config deviceConfig = ma_device_config_init(ma_device_type_playback);
	deviceConfig.playback.format = audioDecoder.outputFormat;
	deviceConfig.playback.channels = audioDecoder.outputChannels;
	deviceConfig.sampleRate = audioDecoder.outputSampleRate;
	deviceConfig.dataCallback = AudioCallback;

	ma_device_init(nullptr, &deviceConfig, &audioDevice);

	ma_device_start(&audioDevice);
}
#endif

#ifndef NOINTRO
void Intro()
{
	std::this_thread::sleep_for(chrono_duration(500));
	std::cout << "\x42\x61\x64" << std::flush;
	std::this_thread::sleep_for(chrono_duration(500));
	std::cout << "\x20\x41\x70\x70\x6C\x65" << std::flush;
	std::this_thread::sleep_for(chrono_duration(500));
	std::cout << "\x20\x62\x79" << std::flush;
	std::this_thread::sleep_for(chrono_duration(500));
	std::cout << "\x20\x43\x72\x61\x73\x68\x5A" << std::flush;
	std::this_thread::sleep_for(chrono_duration(800));
	std::cout << "\x64" << std::flush;
	std::this_thread::sleep_for(chrono_duration(200));
	std::cout << "\x72" << std::flush;
	std::this_thread::sleep_for(chrono_duration(200));
	std::cout << "\x67" << std::flush;
	std::this_thread::sleep_for(chrono_duration(800));
}
#endif

int main()
{
#ifndef NOINTRO
	Intro();
#endif

	InitData();

	unsigned int datIdx = 0;
	unsigned int bitIdx = 0;

	unsigned int backBufferIdx = 0;
	unsigned char backBuffer[BACKBUFFERSIZE];
	backBuffer[backBufferIdx++] = '\r';
	backBuffer[BACKBUFFERSIZE - 1] = 0;

#ifndef NOAUDIO
	InitAudio();
#endif

	chrono_clock::time_point nextFrame = chrono_clock::now();

	while (datIdx < DATASIZE)
	{
		nextFrame += FRAMETIME;

		for (unsigned int y = 0; y < HEIGHT; y++)
		{
			for (unsigned int x = 0; x < WIDTH; x++)
			{
				if (bitIdx == 8)
				{
					datIdx++;
					bitIdx = 0;
				}

				backBuffer[backBufferIdx++] = ((dat[datIdx] & (1 << bitIdx++)) == 0) ? ' ' : PIXEL;
			}

			if (y < (HEIGHT - 1))
				backBuffer[backBufferIdx++] = '\n';
		}

		std::cout << backBuffer;

		backBufferIdx = 0;
		backBuffer[backBufferIdx++] = '\n';
		
#ifdef USE_SPINWAIT
		for (unsigned int spin = 1; chrono_clock::now() < nextFrame; spin++)
		{
			if ((spin % SPINFACTOR) == 0)
				std::this_thread::sleep_for(ONEMS);
			else
				std::this_thread::yield();
		}
#else
		std::this_thread::sleep_until(nextFrame);
#endif
	}

	return 0;
}