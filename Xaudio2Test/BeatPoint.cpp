#include "BeatPoint.h"
#include "pch.h"
#include <map>

using namespace Xaudio2Test;

BeatPoint::BeatPoint(int x, int y, int sequenceId)
{
	Xaxis = x;
	Yaxis = y;
	SequenceId = sequenceId;
}

void BeatPoints::AddPoint(BeatPoint bp)
{
	pointStorage[bp.SequenceId] = bp;
}

BeatPoint BeatPoints::Find(int sequenceid)
{
	return pointStorage[sequenceid];
}