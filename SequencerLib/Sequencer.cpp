#pragma once
#include "pch.h"
#include "Sequencer.h"
#include "ISequencerConfiguration.h"
using namespace SequencerLib;


Sequencer::Sequencer(ISequencerConfiguration * config) 
{
	SequencerConfiguration = config;

}
int Sequencer::AddBeat(UINT64 SamplePoint)
{
	int totalBeats = SequencerConfiguration->GetTotalBeats();

	if (list.size() >= SequencerConfiguration->GetTotalBeats()){
		return -1;
	}
	int BeatIndex = GetIndexFromBufferSample(SamplePoint);

	int Pushable = BeatIndex;
	if (list.end() != std::find(list.begin(), list.end(), BeatIndex))
	{
		Pushable = -1;
		for (int i = 1; i <= SequencerConfiguration->GetErrorCorrectiveCoeffitient(); i++){
			if (list.end() == std::find(list.begin(), list.end(), (BeatIndex + i) % totalBeats))
			{
				Pushable = (BeatIndex + i) % totalBeats;
			}
			else if (list.end() == std::find(list.begin(), list.end(), (BeatIndex - i) % totalBeats))
			{
				Pushable = (BeatIndex - i) % totalBeats;
			}
		}
	}

	if (Pushable == -1){
		return -1;
	}

	list.push_front(Pushable);
	return Pushable;
}
bool Sequencer::IsValidBeatPoint(UINT64 index)
{
	//WaitForSingleObjectEx(_lock, INFINITE, false);
	bool IsValid = NextBeat(index) && list.end() != std::find(list.begin(), list.end(), GetIndexFromBufferSample(index));
	//ReleaseMutex(_lock);
	return IsValid;
}
bool Sequencer::NextBeat(UINT64 bufferplace)
{
	//TODO: itsafuckingconstantinthemiddleofnowhere!!
	return bufferplace % SequencerConfiguration->GetSmallestPointBetweenBeats() < SequencerConfiguration->GetBeatLength();
}

int Sequencer::GetIndexFromBufferSample(UINT64 BufferPoint)
{
	UINT64 CyclePoint = BufferPoint % (SequencerConfiguration->GetBufferSize() / 2);
	return CyclePoint / SequencerConfiguration->GetSmallestPointBetweenBeats();
}

