#include "pch.h"
#include "Audio.h"

using Platform::COMException;
using namespace Xaudio2Test;

Audio::Audio(IXAudio2* pXAudio2, int RecurrenceInterval, int InaccuracyCoefficient, int ResolutionPerSecond, bool Continuous)
{
	//Sequencer
	_RecurenceInterval = RecurrenceInterval;
	Leniancy_In_BeatStrokes = InaccuracyCoefficient;
	_SmallestPointBetweenBeats = BUFFER_LENGTH / (_RecurenceInterval * ResolutionPerSecond);

	lock = CreateMutexEx(nullptr, nullptr, 0, SYNCHRONIZE);
	//nano second resolution
	_ResolutionPerSecond = ResolutionPerSecond;


	// Create a source voice
	WAVEFORMATEX waveFormat;
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nChannels = 1;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.nAvgBytesPerSec = 44100 * 2;
	waveFormat.nBlockAlign = 2;
	waveFormat.wBitsPerSample = 16;
	waveFormat.cbSize = 0;

	HRESULT hr = pXAudio2->CreateSourceVoice(&pSourceVoice, &waveFormat,
		XAUDIO2_VOICE_NOPITCH, 1.0f, this);

	if (FAILED(hr))
		throw ref new COMException(hr, "CreateSourceVoice failure");

	// Allocate buffer for submitting waveform data
	pWaveformBuffer = new short[BUFFER_LENGTH];
	index = 0;
	angle = 0;
	angleIncrement = 0;
	ContinuousPlay = Continuous;
	// Start the voice playing
	if (Continuous)
	{
		pSourceVoice->Start();
	}
}

Audio::~Audio()
{
	pSourceVoice->Stop();
	delete pWaveformBuffer;
}

void Audio::SetFrequency(float freq)
{
	angleIncrement = (int)(65536.0 * WAVEFORM_LENGTH * freq / 44100.0);
}

void Audio::SetAmplitude(float amp)
{
	pSourceVoice->SetVolume(amp);
}

bool Audio::NextBeat(UINT64 bufferplace)
{
	return bufferplace % _SmallestPointBetweenBeats == 0;
}

void _stdcall Audio::OnVoiceProcessingPassStart(UINT32 bytesRequired)
{
	UINT64 BeatLenghtInBytes = 200;
	if (bytesRequired == 0 || !ContinuousPlay)
		return;

	//handle offsett start
	//if start is without beat or if start is within beat

	int startIndex = index;
	int endIndex = startIndex + bytesRequired / 2;

	if (endIndex <= BUFFER_LENGTH)
	{
		for (int i = startIndex; i < endIndex - startIndex; i += BeatLenghtInBytes)
		{
			FillAndSubmit(i, BeatLenghtInBytes / DUALBAND);
		}
	}
	else
	{
		for (int i = startIndex; i < BUFFER_LENGTH - startIndex; i += BeatLenghtInBytes)
		{
			FillAndSubmit(i, BeatLenghtInBytes / DUALBAND);
		}
		for (int i = 0; i < endIndex % BUFFER_LENGTH; i += BeatLenghtInBytes)
			FillAndSubmit(0, endIndex % BUFFER_LENGTH);
	}
}


UINT64 Audio::GetBufferReadFromSourceVoice(void)
{
	XAUDIO2_VOICE_STATE state;
	pSourceVoice->GetState(&state, 0);
	return state.SamplesPlayed * DUALBAND;
}

void Audio::CreateBlank(int startIndex, int count)
{
	for (int i = startIndex; i < startIndex + count; i++)
	{
		pWaveformBuffer[i] = 0;
	}
	XAUDIO2_BUFFER buffer = { 0 };
	buffer.AudioBytes = 2 * BUFFER_LENGTH;
	buffer.pAudioData = (byte *)pWaveformBuffer;
	buffer.Flags = 0;
	buffer.PlayBegin = startIndex;
	buffer.PlayLength = count;

	HRESULT hr = pSourceVoice->SubmitSourceBuffer(&buffer);

	if (FAILED(hr))
		throw ref new COMException(hr, "SubmitSourceBuffer");
}


void Audio::FillAndSubmit(int startIndex, int count)
{
	if (IsValidBeatPoint(index))
	{
		CreateBeat(startIndex, count);
	}
	else
	{
		CreateBlank(startIndex, count);
	}
	index += count * DUALBAND;
}

void Audio::CreateBeat(int startIndex, int count)
{
	for (int i = startIndex; i < startIndex + count; i++)
	{
		pWaveformBuffer[i] = (short)(angle / WAVEFORM_LENGTH - 32768);
		angle = (angle + angleIncrement) % (WAVEFORM_LENGTH * 65536);
	}

	XAUDIO2_BUFFER buffer = { 0 };
	buffer.AudioBytes = 2 * BUFFER_LENGTH;
	buffer.pAudioData = (byte *)pWaveformBuffer;
	buffer.Flags = 0;
	buffer.PlayBegin = startIndex;
	buffer.PlayLength = count;

	HRESULT hr = pSourceVoice->SubmitSourceBuffer(&buffer);

	if (FAILED(hr))
		throw ref new COMException(hr, "SubmitSourceBuffer");
}

bool Audio::IsValidBeatPoint(UINT64 index)
{
	WaitForSingleObjectEx(lock, INFINITE, false);
	bool IsValid = NextBeat(index) && (list.end() != std::find(list.begin(), list.end(), GetIndexFromBufferSample(index)));
	ReleaseMutex(lock);
	return IsValid;
}

int Audio::GetIndexFromBufferSample(UINT64 BufferPoint)
{
	UINT64 CyclePoint = BufferPoint % BUFFER_LENGTH;
	return CyclePoint / _SmallestPointBetweenBeats;
}
int Audio::AddBeat()
{
	WaitForSingleObjectEx(lock, INFINITE, false);

	if (list.size() >= _RecurenceInterval * _ResolutionPerSecond){
		return -1;
	}
	int BeatIndex = GetIndexFromBufferSample(GetBufferReadFromSourceVoice());

	int Pushable = BeatIndex;
	if (list.end() != std::find(list.begin(), list.end(), BeatIndex))
	{
		Pushable = -1;
		for (int i = 1; i <= Leniancy_In_BeatStrokes; i++){
			if (list.end() == std::find(list.begin(), list.end(), (BeatIndex + i) % (_RecurenceInterval * _ResolutionPerSecond)))
			{
				Pushable = (BeatIndex + i) % (_RecurenceInterval * _ResolutionPerSecond);
			}
			else if (list.end() == std::find(list.begin(), list.end(), (BeatIndex - i) % (_RecurenceInterval * _ResolutionPerSecond)))
			{
				Pushable = (BeatIndex - i) % (_RecurenceInterval * _ResolutionPerSecond);
			}
		}
	}

	if (Pushable == -1){
		return -1;
	}

	list.push_front(Pushable);
	ReleaseMutex(lock);
	return Pushable;
}

void Audio::Reset()
{
	WaitForSingleObjectEx(lock, INFINITE, false);
	list.clear();
	ReleaseMutex(lock);

}

