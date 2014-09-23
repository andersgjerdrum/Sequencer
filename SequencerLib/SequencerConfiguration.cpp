#pragma once
#include "pch.h"
#include "SequencerConfiguration.h"
using namespace SequencerLib;


SequencerConfiguration::SequencerConfiguration(int RecurrenceIntervalSeconds, int ResolutionPerSecond, int ErrorCorrectiveCoeffitient, UINT64 BeatLengthInBytes)
{
	_RecurrenceIntervalSeconds = RecurrenceIntervalSeconds;
	_ResolutionPerSecond = ResolutionPerSecond;
	_ErrorCorrectiveCoeffitient = ErrorCorrectiveCoeffitient;
	_BeatLengthInBytes = BeatLengthInBytes;
	if (!ValidateParameters())
	{
		throw std::invalid_argument("Invalid arguments input to SequencerConfiguration Ctor");
	}
}

bool SequencerConfiguration::ValidateParameters(void)
{
	return _RecurrenceIntervalSeconds != 0 
		&& _ResolutionPerSecond != 0 
		&& _BeatLengthInBytes != 0
		&& _BeatLengthInBytes <= GetSmallestPointBetweenBeats();
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