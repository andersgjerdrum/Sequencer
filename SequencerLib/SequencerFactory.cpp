#pragma once
#include "pch.h"
#include "SequencerFactory.h"
#include "Sequencer.h"
#include "AudioBuffer.h"
#include "AudioStream.h"
using namespace SequencerLib;



SequencerInstance * SequencerFactory::Create(ISequencerConfiguration * config, IXAudio2 *pXAudio, Platform::String^ url)
{
	auto sequencerInst = new SequencerInstance();
	sequencerInst->Control = new Sequencer(config);
	auto stream = new AudioStream(url);
	sequencerInst->Sound = new Audio(pXAudio, new AudioBuffer(config->GetBufferSize(), sequencerInst->Control, stream->ReadAll()));
	return sequencerInst;
}