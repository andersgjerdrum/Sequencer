#pragma once
#include "pch.h"

namespace SequencerLib
{
	struct ISequencerConfiguration
	{
		virtual int GetRecurrenceIntervalPerSecond(void) = 0;
		virtual int GetErrorCorrectiveCoeffitient(void) = 0;
		virtual int GetResolutionPerSecond(void) = 0;
		virtual UINT64 GetBufferSize(void) = 0;
		virtual UINT64 GetSmallestPointBetweenBeats(void) = 0;
		virtual int GetTotalBeats(void) = 0;
		virtual UINT64 GetBeatLength(void) = 0;
		
	};

}