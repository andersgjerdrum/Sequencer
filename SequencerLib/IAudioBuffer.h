#pragma once
#include "pch.h"

namespace SequencerLib
{
	struct IAudioBuffer
	{
		virtual void PrepareBuffer(UINT32 bytesRequired, std::function<void(int, int, int,byte*)> process) = 0;
		virtual void SetFrequency(float freq) = 0; 
		virtual short * GetBufferAddress(void) = 0;
	};
}
