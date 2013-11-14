#include "pch.h"
#include "Utility.h"


using namespace Xaudio2Test;
Utility::Utility(void)
{
	return;
}
void Utility::createSoundWave(int sec, int freq, int dualaudio, byte *soundData, int size)
{
	if(size != sec * freq *dualaudio){
		ApplicationError(__FUNCTIONW__ ,L"Size of byte array did not equal dimentions of soundwave");
	}
	for (int index = 0, second = 0; second < SECONDS; second++)
	{
		for (int cycle = 0; cycle < CYCLE; cycle++)
		{
			for (int sample = 0; sample < SAMPLE; sample++)
			{
				short value = sample < 50 ? SHORT_MAX : SHORT_MIN;
				//stored with low bytes first
				soundData[index++] = value & BITMASK_SHORT;
				soundData[index++] = (value >> 8) & BITMASK_SHORT;
			}
		}
	}
}

void Utility:: ApplyVolume(byte *soundData, double samples)
{
	for (int index = 0, sample = 0; sample < samples; sample++)
	{
		double t = 1;
		if (sample < 2 * samples / 5)
			t = sample / (2.0 * samples / 5);
		else if (sample > 3 * samples / 5)
			t = (samples - sample) / (2.0 * samples / 5);
		double amplitude = pow(2, 15 * t) - 1;
		short waveform = sample % 100 < 50 ? 1 : -1;
		short value = short(amplitude * waveform);
		soundData[index++] = value;
		soundData[index++] = value;

	}
}
void Utility::CreatePitch(byte *soundData, double samples,double samplesPerSec)
{
	double angle = 0;
	for (int index = 0, sample = 0; sample < samples; sample++)
	{
		double t = 1;
		if (sample < 2 * samples / 5)
			t = sample / (2.0 * samples / 5);
		else if (sample > 3 * samples / 5)
			t = (samples - sample) / (2.0 * samples / 5);
		double frequency = 220 * pow(2, 2 * t);
		double angleIncrement = 360 * frequency / samplesPerSec;
		angle += angleIncrement;
		while (angle > 360)
			angle -= 360;
		short value = angle < 180 ? 32767 : -32767;
		soundData[index++] = value;
		soundData[index++] = value;
	}
}
bool Utility::IsApplicationError(HRESULT hr,wchar_t * method, wchar_t * message)
{

	if (FAILED(hr))
	{
		ApplicationError(method,message);
		return true;
	}
	return false;
}
void  Utility::ApplicationError(wchar_t* method, wchar_t * message)
{
	//This might seriously fuck shit up!

	OutputDebugString(method);
	OutputDebugString(message);
	return;
}

