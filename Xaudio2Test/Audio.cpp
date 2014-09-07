#include "pch.h"
#include "Audio.h"

using Platform::COMException;
using namespace Xaudio2Test;

Audio::Audio(IXAudio2* pXAudio2, bool Continuous, Sequencer ^Seq)
{
	
    // Create a source voice
	Sequencerobject = Seq;
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
	if(Continuous)
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

void _stdcall Audio::OnVoiceProcessingPassStart(UINT32 bytesRequired)
{
	if (Sequencerobject->IsTime()){
		OutputDebugString(L"ITs TIme!!\n");
	}
	
    if (bytesRequired == 0 || !ContinuousPlay)
        return;

    int startIndex = index;
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
    index = (index + bytesRequired / 2) % BUFFER_LENGTH;
}

void Audio::FillAndSubmit(int startIndex, int count)
{
    for (int i = startIndex; i < startIndex + count; i++)
    {
        pWaveformBuffer[i] = (short)(angle / WAVEFORM_LENGTH - 32768);
        angle = (angle + angleIncrement) % (WAVEFORM_LENGTH * 65536);
    }

    XAUDIO2_BUFFER buffer = {0};
    buffer.AudioBytes = 2 * BUFFER_LENGTH;
    buffer.pAudioData = (byte *)pWaveformBuffer;
    buffer.Flags = 0;
    buffer.PlayBegin = startIndex;
    buffer.PlayLength = count;
    
    HRESULT hr = pSourceVoice->SubmitSourceBuffer(&buffer);

    if (FAILED(hr))
        throw ref new COMException(hr, "SubmitSourceBuffer");
}
void Audio::StartFillSubmitStop()
{
    pSourceVoice->Start();
	FillAndSubmit(0,BUFFER_LENGTH);
}


