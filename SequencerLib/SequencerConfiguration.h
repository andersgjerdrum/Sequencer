#pragma once
#include "pch.h"
#include "ISequencerConfiguration.h"

namespace SequencerLib{

	class SequencerConfiguration : public ISequencerConfiguration{
	public:
		SequencerConfiguration(int RecurrenceIntervalSeconds, int ResolutionPerSecond, int ErrorCorrectiveCoeffitient,UINT64 BeatLenghtInBytes);
		int GetRecurrenceIntervalPerSecond(void);
		int GetErrorCorrectiveCoeffitient(void);
		int GetResolutionPerSecond(void);
		UINT64 GetBufferSize(void);
		UINT64 GetSmallestPointBetweenBeats(void);
		int GetTotalBeats(void);
		UINT64 GetBeatLength(void);
	private:
		int _RecurrenceIntervalSeconds;
		int _ResolutionPerSecond;
		int _ErrorCorrectiveCoeffitient;
		UINT64 _BeatLengthInBytes;
	};
}