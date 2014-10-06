#pragma once
#include "pch.h"
#include "ISequencer.h"
#include "IAudioBuffer.h"



namespace SequencerLib
{
	class AudioBuffer : public IAudioBuffer
	{
	private:
		short * pWaveformBuffer;
		UINT64 SampleSize;
		ISequencer * SequencerObject;
		UINT64 _index;
		int _angle;
		int _angleIncrement;
		static const int WAVEFORM_LENGTH = 8192;
		void FillBuffer(int startIndex, int count, std::function<void(int, int, int,byte*)> process);
	public:
		AudioBuffer(UINT64 buffsize, ISequencer * Sequencer);
		void PrepareBuffer(UINT32 bytesRequired, std::function<void(int, int, int,byte*)> process);
		void SetFrequency(float freq);
		short * GetBufferAddress(void);
	};
}