#pragma once
#include "pch.h"
#include "SequencerFactory.h"
#include "Sequencer.h"
#include "AudioBuffer.h"
using namespace SequencerLib;



SequencerInstance * SequencerFactory::Create(ISequencerConfiguration * config, IXAudio2 *pXAudio)
{
	auto sequencerInst = new SequencerInstance();
	sequencerInst->Control = new Sequencer(config);
	sequencerInst->Sound = new Audio(pXAudio, new AudioBuffer(config->GetBufferSize(), sequencerInst->Control));
	return sequencerInst;
}