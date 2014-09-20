#pragma once
#include "pch.h"
#include "SequencerConfiguration.h"
using namespace SequencerLib;

#define SAMPLERATE 44100
#define DUALBAND 2
#define BYTESPERSECOND (SAMPLERATE * DUALBAND)
SequencerConfiguration::SequencerConfiguration(int RecurrenceIntervalSeconds, int ResolutionPerSecond, int ErrorCorrectiveCoeffitient, UINT64 BeatLengthInBytes)
{
	_RecurrenceIntervalSeconds = RecurrenceIntervalSeconds;
	_ResolutionPerSecond = ResolutionPerSecond;
	_ErrorCorrectiveCoeffitient = ErrorCorrectiveCoeffitient;
	_BeatLengthInBytes = BeatLengthInBytes;

}
int SequencerConfiguration::GetRecurrenceIntervalPerSecond(void)
{
	return _RecurrenceIntervalSeconds;
}

int SequencerConfiguration::GetErrorCorrectiveCoeffitient(void)
{
	return _ErrorCorrectiveCoeffitient;
}

int SequencerConfiguration::GetTotalBeats(void)
{
	return this->GetRecurrenceIntervalPerSecond() * this->GetResolutionPerSecond();
}
int SequencerConfiguration::GetResolutionPerSecond(void)
{
	return _ResolutionPerSecond;
}

UINT64 SequencerConfiguration::GetBufferSize(void)
{
	return BYTESPERSECOND * _RecurrenceIntervalSeconds;
}
UINT64 SequencerConfiguration::GetBeatLength(void)
{
	return _BeatLengthInBytes;
}
UINT64 SequencerConfiguration::GetSmallestPointBetweenBeats(void)
{
	return (GetBufferSize() / DUALBAND) / (GetTotalBeats());
}