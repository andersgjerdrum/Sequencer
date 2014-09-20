#include "pch.h"
#include "BeatPoint.h"
#include <map>

using namespace SequencerLib;

BeatPoint::BeatPoint(int x, int y, int sequenceId)
{
	Xaxis = x;
	Yaxis = y;
	SequenceId = sequenceId;
}
BeatPoint::BeatPoint()
{
	Xaxis = 0;
	Yaxis = 0;
	SequenceId = -1;
}
BeatPoints::BeatPoints()
{
	lock = CreateMutexEx(nullptr,nullptr,0,SYNCHRONIZE);
}
void BeatPoints::AddPoint(BeatPoint bp)
{

	WaitForSingleObjectEx(lock,INFINITE,false);
	pointStorage[bp.SequenceId] = bp;
	ReleaseMutex(lock);

}

BeatPoint BeatPoints::Find(int sequenceid)
{
	WaitForSingleObjectEx(lock,INFINITE,false);
	return pointStorage[sequenceid];
	ReleaseMutex(lock);
}
