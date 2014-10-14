#pragma once
#include "pch.h"
#include "ISequencer.h"
#include "IAudioBuffer.h"



namespace SequencerLib
{
	class AudioBuffer : public IAudioBuffer
	{
	private:
		std::vector<byte> *pWaveformBuffer;
		UINT64 SampleSize;
		std::vector<byte> SampleBufferObject;
		ISequencer * SequencerObject;
		UINT64 _index;
		int _angle;
		int _angleIncrement;
		static const int WAVEFORM_LENGTH = 8192;
		bool SimpleWaveform;
		UINT64 SampleIndex;
		void FillBuffer(int startIndex, int count, std::function<void(int, int, int, std::vector<byte>*)> process);
	public:
		AudioBuffer(UINT64 buffsize, ISequencer * Sequencer, std::vector<byte> sample, bool SimpleWave = true);
		void PrepareBuffer(UINT32 bytesRequired, std::function<void(int, int, int, std::vector<byte>*)> process);
		void SetFrequency(float freq);
	};
}