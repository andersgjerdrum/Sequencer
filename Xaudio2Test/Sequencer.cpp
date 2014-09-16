#include "pch.h"
#include "Sequencer.h"
#include <algorithm>
#include <list>
#include <ppltasks.h>
#define ONESECONDIN100NANO 10000000L
#define ADJUST 25
using namespace Xaudio2Test;
using namespace Concurrency;
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml;


Sequencer::Sequencer(int RecurrenceInterval, int ResolutionPerSecond, float InaccuracyCoefficient)
{

	_RecurenceInterval = RecurrenceInterval;
	Leniancy_In_BeatStrokes = (int)(InaccuracyCoefficient / ResolutionPerSecond);

	lock = CreateMutexEx(nullptr,nullptr,0,SYNCHRONIZE);
	//nano second resolution
	_ResolutionPerSecond = ResolutionPerSecond;
}

LONG64 Sequencer::GetNanoSec()
{
	Windows::Globalization::Calendar^ c = ref new Windows::Globalization::Calendar;
	c->SetToNow();
	return  c->GetDateTime().UniversalTime;
}
int Sequencer::GetIndexFromTime(LONG64 time){

	LONG64 IntervalLengthInNano = _RecurenceInterval * ONESECONDIN100NANO * 100;
	LONG64 RelativePointInTime = time % (IntervalLengthInNano);
	LONG64 SmallestIntervalBetweenBeats = IntervalLengthInNano / (_RecurenceInterval * _ResolutionPerSecond);
	return RelativePointInTime / SmallestIntervalBetweenBeats;
}
int Sequencer::AddBeat()
{
	//
	//do we need duplication
	if (list.size() >= _RecurenceInterval * _ResolutionPerSecond){
		return -1;
	}
	int BeatIndex = GetIndexFromTime(GetNanoSec());

	int Pushable = BeatIndex;
	if (list.end() != std::find(list.begin(), list.end(), BeatIndex))
	{
		Pushable = -1;
		for(int i = 1; i <= Leniancy_In_BeatStrokes; i++){
			if (list.end() == std::find(list.begin(), list.end(), (BeatIndex + i) % (_RecurenceInterval * _ResolutionPerSecond)))
			{
				Pushable = (BeatIndex + i) % (_RecurenceInterval * _ResolutionPerSecond);
			}
			else if (list.end() == std::find(list.begin(), list.end(), (BeatIndex - i) % (_RecurenceInterval * _ResolutionPerSecond)))
			{
				Pushable = (BeatIndex - i) % (_RecurenceInterval * _ResolutionPerSecond);
			}
		}
	}

	if(Pushable == -1){
		return -1;
	}
	
	WaitForSingleObjectEx(lock,INFINITE,false);
	list.push_front(Pushable);
	ReleaseMutex(lock);
	return Pushable;
}

void Sequencer::Reset()
{
	WaitForSingleObjectEx(lock,INFINITE,false);
	list.clear();
	ReleaseMutex(lock);

}
Sequencer::~Sequencer()
{
	CloseHandle(lock);
}


