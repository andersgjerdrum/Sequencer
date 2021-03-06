#pragma once
#include "pch.h"
#include "ISequencerConfiguration.h"

#define SAMPLERATE 44100
#define DUALBAND 2
#define BYTESPERSECOND (SAMPLERATE * DUALBAND)

namespace SequencerLib{

	class SequencerConfiguration : public ISequencerConfiguration{
	public:
		SequencerConfiguration(int RecurrenceIntervalSeconds, float ResolutionPerSecond, int ErrorCorrectiveCoeffitient,UINT64 BeatLenghtInBytes);
		int GetRecurrenceIntervalPerSecond(void);
		int GetErrorCorrectiveCoeffitient(void);
		float GetResolutionPerSecond(void);
		UINT64 GetBufferSize(void);
		UINT64 GetSmallestPointBetweenBeats(void);
		int GetTotalBeats(void);
		UINT64 GetBeatLength(void);
	private:
		int _RecurrenceIntervalSeconds;
		float _ResolutionPerSecond;
		int _ErrorCorrectiveCoeffitient;
		UINT64 _BeatLengthInBytes;
		bool ValidateParameters();
	};
}