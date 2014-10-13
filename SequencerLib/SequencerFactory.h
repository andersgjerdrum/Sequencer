#pragma once
#pragma once
#include "pch.h"
#include "ISequencer.h"
#include "Audio.h"
#include "ISequencerConfiguration.h"
namespace SequencerLib
{
	struct SequencerInstance
	{
		ISequencer * Control;
		Audio * Sound;
	};

	class SequencerFactory{
	public:
		SequencerInstance * Create(ISequencerConfiguration * config, IXAudio2 *pXAudio, Platform::String^ url);
	};
}