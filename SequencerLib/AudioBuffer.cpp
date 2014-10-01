#pragma once
#include "pch.h"
#include "AudioBuffer.h"
using namespace SequencerLib;


AudioBuffer::AudioBuffer(UINT64 buffsize, ISequencer * Sequencer)
{
	SequencerObject = Sequencer;
	BufferSize = buffsize;
	_index = 0;
	_angle = 0;
	_angleIncrement = 0;
	pWaveformBuffer = new short[buffsize];
}
void AudioBuffer::SetFrequency(float freq)
{
	_angleIncrement = (int)(65536.0 * WAVEFORM_LENGTH * freq / 44100.0);
}

void AudioBuffer::PrepareBuffer(UINT32 bytesRequired, std::function<void(int,int,int,byte*)> process)
{
	if (bytesRequired == 0)
		return;

	int startIndex = _index;
	int endIndex = startIndex + bytesRequired / 2;

	if (endIndex <= BufferSize)
	{
		FillBuffer(startIndex, endIndex - startIndex,process);
	}
	else
	{
		FillBuffer(startIndex, BufferSize - startIndex,process);
		FillBuffer(0, endIndex % BufferSize,process);
	}
	_index = (_index + bytesRequired / 2) % BufferSize;
}
short* AudioBuffer::GetBufferAddress(void)
{
	return pWaveformBuffer;
}

void AudioBuffer::FillBuffer(int startIndex, int count, std::function<void(int,int,int,byte*)> process)
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
	process(startIndex, count, BufferSize,(byte*)pWaveformBuffer);
}