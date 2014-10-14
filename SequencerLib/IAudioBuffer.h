#pragma once
#include "pch.h"

namespace SequencerLib
{
	struct IAudioBuffer
	{
		virtual void PrepareBuffer(UINT32 bytesRequired, std::function<void(int, int, int, std::vector<byte>*)> process) = 0;
		virtual void SetFrequency(float freq) = 0; 
	};
}
