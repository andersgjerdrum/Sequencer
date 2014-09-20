#pragma once
#include "pch.h"

namespace SequencerLib
{
	struct ISequencer
	{
		virtual int AddBeat(UINT64 SamplePoint) = 0;
		virtual bool IsValidBeatPoint(UINT64 SamplePoint) = 0;
	};
}
