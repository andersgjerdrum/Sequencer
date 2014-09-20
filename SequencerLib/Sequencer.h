#pragma once
#include "pch.h"
#include "ISequencer.h"
#include "ISequencerConfiguration.h"
namespace SequencerLib
{
	class Sequencer : public ISequencer
	{
	public:
		int AddBeat(UINT64 SamplePoint);
		bool IsValidBeatPoint(UINT64 SamplePoint);
		std::list<int> list;
		Sequencer(ISequencerConfiguration * config);
	private:
		ISequencerConfiguration * SequencerConfiguration;
		int GetIndexFromBufferSample(UINT64 BufferPoint);
		bool NextBeat(UINT64 bufferplace);
	};

}