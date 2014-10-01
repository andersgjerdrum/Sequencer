#include "pch.h"
#include "Audio.h"

using Platform::COMException;
using namespace SequencerLib;

Audio::Audio(IXAudio2* pXAudio2, IAudioBuffer* buffer) 
{
	BufferObject = buffer;
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
	// Start the voice playing
	pSourceVoice->Start();
	
}

Audio::~Audio()
{
	pSourceVoice->Stop();
}

void Audio::SetAmplitude(float amp)
{
	pSourceVoice->SetVolume(amp);
}

void _stdcall Audio::OnVoiceProcessingPassStart(UINT32 bytesRequired)
{
	BufferObject->PrepareBuffer(bytesRequired, [this](int a, int b, int c, byte *bufferAddr)->void {
		FillAndSubmit(a, b, c,bufferAddr);
	});
}

void Audio::FillAndSubmit(int startIndex, int count, int bytes, byte * bufferAddr)
{
	
	XAUDIO2_BUFFER buffer = { 0 };
	buffer.AudioBytes = bytes * 2;
	buffer.pAudioData = bufferAddr;
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
