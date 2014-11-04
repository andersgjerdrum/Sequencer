#pragma once
#include "pch.h"
#include "SequencerFactory.h"
#include "SequencerConfiguration.h"
#include "Sequencer.h"
#include "AudioBuffer.h"
#include "AudioStream.h"
using namespace Windows::Storage::Streams;

using namespace SequencerLib;


SequencerInstance * SequencerFactory::Create(int RecurrenceIntervalSecconds,  IXAudio2 *pXAudio, IRandomAccessStream^ streamHandle)
{
	auto sequencerInst = new SequencerInstance();
	auto stream = new AudioStream(streamHandle);
	auto configuration = new SequencerConfiguration(RecurrenceIntervalSecconds, 0.10f, 2, stream->GetMaxStreamLengthInBytes());
	sequencerInst->Control = new Sequencer(configuration);
	sequencerInst->Sound = new Audio(pXAudio, new AudioBuffer(stream->GetMaxStreamLengthInBytes(), sequencerInst->Control, stream->ReadAll()));
	return sequencerInst;
}