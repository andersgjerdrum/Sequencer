#pragma once
#include "pch.h"
#include "AudioBuffer.h"
using namespace SequencerLib;


AudioBuffer::AudioBuffer(UINT64 buffsize, ISequencer * Sequencer, std::vector<byte> sample, bool SimpleWave)
{
	SequencerObject = Sequencer;
	SampleSize = buffsize / 2;
	_index = 0;
	_angle = 0;
	_angleIncrement = 0;
	pWaveformBuffer = new std::vector<byte>(buffsize);
	SampleBufferObject = sample;
	SimpleWaveform = SimpleWave;
	SampleIndex = 0;
}
void AudioBuffer::SetFrequency(float freq)
{
	_angleIncrement = (int)(65536.0 * WAVEFORM_LENGTH * freq / 44100.0);
}

void AudioBuffer::PrepareBuffer(UINT32 bytesRequired, std::function<void(int, int, int, std::vector<byte>*)> process)
{
	if (bytesRequired == 0)
		return;

	int startIndex = _index;
	int endIndex = startIndex + bytesRequired / 2;

	if (endIndex <= SampleSize)
	{
		FillBuffer(startIndex, endIndex - startIndex, process);
	}
	else
	{
		FillBuffer(startIndex, SampleSize - startIndex, process);
		FillBuffer(0, endIndex % SampleSize, process);
	}
	_index = (_index + bytesRequired / 2) % SampleSize;
}

void AudioBuffer::FillBuffer(int startIndex, int count, std::function<void(int, int, int, std::vector<byte>*)> process)
{
	for (int i = startIndex * 2; i < (startIndex * 2) + (count * 2); i++)
	{
		if (SequencerObject->IsValidBeatPoint(i))
		{
			if (SimpleWaveform){
				pWaveformBuffer->at(i) = (short)(_angle / WAVEFORM_LENGTH - 32768);
				_angle = (_angle + _angleIncrement) % (WAVEFORM_LENGTH * 65536);
			}
			else{
				pWaveformBuffer->at(i) = SampleBufferObject[SampleIndex];
				SampleIndex++;
			}
		}
		else
		{
			SampleIndex = 0;
			pWaveformBuffer->at(i) = 0;
		}
	}
	process(startIndex, count, SampleSize, pWaveformBuffer);
}