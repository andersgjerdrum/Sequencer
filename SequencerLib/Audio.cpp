#include "pch.h"
#include "Audio.h"

using Platform::COMException;
using namespace SequencerLib;

Audio::Audio(IXAudio2* pXAudio2, ISequencer* Sequencer) 
{
	SequencerObject = Sequencer;
	// Create a source voice
	WAVEFORMATEX waveFormat;
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nChannels = 1;
	waveFormat.nSamplesPerSec = SAMPLERATE;
	waveFormat.nAvgBytesPerSec = BYTESPERSECOND;
	waveFormat.nBlockAlign = DUALBAND;
	waveFormat.wBitsPerSample = 16;
	waveFormat.cbSize = 0;

	HRESULT hr = pXAudio2->CreateSourceVoice(&pSourceVoice, &waveFormat,
		XAUDIO2_VOICE_NOPITCH, 1.0f, this);

	if (FAILED(hr))
		throw ref new COMException(hr, "CreateSourceVoice failure");

	// Allocate buffer for submitting waveform data
	pWaveformBuffer = new short[BUFFER_LENGTH];
	_index = 0;
	_angle = 0;
	_angleIncrement = 0;
	// Start the voice playing
	pSourceVoice->Start();
	
}

Audio::~Audio()
{
	pSourceVoice->Stop();
	delete pWaveformBuffer;
}

void Audio::SetFrequency(float freq)
{
	_angleIncrement = (int)(65536.0 * WAVEFORM_LENGTH * freq / 44100.0);
}

void Audio::SetAmplitude(float amp)
{
	pSourceVoice->SetVolume(amp);
}

void _stdcall Audio::OnVoiceProcessingPassStart(UINT32 bytesRequired)
{
	if (bytesRequired == 0)
		return;

	int startIndex = _index;
	int endIndex = startIndex + bytesRequired / 2;

	if (endIndex <= BUFFER_LENGTH)
	{
		FillAndSubmit(startIndex, endIndex - startIndex);
	}
	else
	{
		FillAndSubmit(startIndex, BUFFER_LENGTH - startIndex);
		FillAndSubmit(0, endIndex % BUFFER_LENGTH);
	}
	_index = (_index + bytesRequired / 2) % BUFFER_LENGTH;
}

void Audio::FillAndSubmit(int startIndex, int count)
{
	for (int i = startIndex; i < startIndex + count; i++)
	{
		if (SequencerObject->IsValidBeatPoint(i))
		{
			pWaveformBuffer[i] = (short)(_angle / WAVEFORM_LENGTH - 32768);
			_angle = (_angle + _angleIncrement) % (WAVEFORM_LENGTH * 65536);
		}
		else
		{
			pWaveformBuffer[i] = 0;
		}
	}

	XAUDIO2_BUFFER buffer = { 0 };
	buffer.AudioBytes = DUALBAND * BUFFER_LENGTH;
	buffer.pAudioData = (byte *)pWaveformBuffer;
	buffer.Flags = 0;
	buffer.PlayBegin = startIndex;
	buffer.PlayLength = count;

	HRESULT hr = pSourceVoice->SubmitSourceBuffer(&buffer);

	if (FAILED(hr))
		throw ref new COMException(hr, "SubmitSourceBuffer");
}


UINT64 Audio::GetBufferReadFromSourceVoice(void)
{
	XAUDIO2_VOICE_STATE state;
	pSourceVoice->GetState(&state, 0);
	return state.SamplesPlayed;
}
