#pragma once
#include "pch.h"
#include "SequencerConfiguration.h"
using namespace SequencerLib;


SequencerConfiguration::SequencerConfiguration(int RecurrenceIntervalSeconds, float ResolutionPerSecond, int ErrorCorrectiveCoeffitient, UINT64 BeatLengthInBytes)
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
	//if resolution is negative,spanning multiple secconds, ensure that it is a direct multiplum of one second
	bool resolutionIsOk = true;
	if (_ResolutionPerSecond < 1.0f)
	{
		float f = 1.0f / _ResolutionPerSecond;
		resolutionIsOk = (fabs(f - round(f)) < 0.000001f) && _RecurrenceIntervalSeconds % (int) f  == 0;
	}

	return _RecurrenceIntervalSeconds != 0
		&& _ResolutionPerSecond != 0.0f
		&& _BeatLengthInBytes != 0
		&& resolutionIsOk
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
float SequencerConfiguration::GetResolutionPerSecond(void)
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