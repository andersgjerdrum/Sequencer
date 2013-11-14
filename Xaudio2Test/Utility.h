#pragma once
#include "pch.h"

#define SECONDS 5
#define SAMPLERATE 44100
#define DUALBAND 2
#define BITSPERSAMPLE 16
#define DATASIZE (DUALBAND * SECONDS * SAMPLERATE)
#define BYTESPERSECOND (SAMPLERATE * DUALBAND)
#define CYCLE 441 // A above middle C 
#define SAMPLE 100
#define SHORT_MIN -32768
#define SHORT_MAX 32767
#define BITMASK_SHORT 0XFF

namespace Xaudio2Test
{
	class Utility
	{
	public:
		Utility();
		static void createSoundWave(int sec, int freq, int dualaudio, byte *ptr, int size);
		static void Utility:: ApplyVolume(byte *soundData, double samples);
		static void Utility::CreatePitch(byte *soundData, double samples,double samplesPerSec);
		static void ApplicationError(wchar_t* method, wchar_t * message);
		static bool IsApplicationError(HRESULT hr,wchar_t * method, wchar_t * message);
	};
}