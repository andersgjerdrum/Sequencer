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


Sequencer::Sequencer(int RecurrenceInterval, int ResolutionOverInterval, float InaccuracyCoefficient, SequencerExecuteDelegate^ CoreFunctionToCall)
{

	sequencercorefunc = CoreFunctionToCall;
	_RecurenceInterval = RecurrenceInterval;
	Leniancy_In_BeatStrokes = (int)((InaccuracyCoefficient*ONESECONDIN100NANO) / Speed);

	StopWatch = 0;
	lock = CreateMutexEx(nullptr,nullptr,0,SYNCHRONIZE);
	//nano second resolution
	Speed = ONESECONDIN100NANO/ResolutionOverInterval;
	CurrentTime = 0;
	LastTime = 0;
	_ResolutionOverInterval = ResolutionOverInterval;
	ContinueLoop = true;
	CreateTask();
}
void Sequencer::CreateTask(void)
{
	create_task([this]() {
	}).then([this](){
		while (ContinueLoop){
			WrapperFunc();
			WaitFunc((1000 / this->_ResolutionOverInterval));
		}
	}, task_continuation_context::use_arbitrary());
}
void Sequencer::WaitFunc(int miliseconds)
{
	HANDLE m_event;
	m_event= CreateEventEx(NULL, NULL, CREATE_EVENT_MANUAL_RESET, EVENT_ALL_ACCESS);
	WaitForSingleObjectEx(m_event, miliseconds, false);
}
void Sequencer::WrapperFunc()
{
	LastTime = CurrentTime;
	CurrentTime = (CurrentTime + 1) % (_RecurenceInterval * _ResolutionOverInterval);
	
	if(list.size() <= 0)
	{
		return;
	}
	WaitForSingleObjectEx(lock,INFINITE,false);
	std::list<int>::iterator findIter = std::find(list.begin(), list.end(), CurrentTime);
	ReleaseMutex(lock);

	if(findIter == list.end()){
		return;
	}
	if(*findIter <= CurrentTime)
	{
		sequencercorefunc(*findIter);
	}
}

bool Sequencer::IsTime(void)
{
	LONG64 time = GetNanoSec();
	LONG64 IntervalLengthInNano = _RecurenceInterval * ONESECONDIN100NANO * 100;
	LONG64 RelativePointInTime = time % (IntervalLengthInNano);
	LONG64 SmallestIntervalBetweenBeats = IntervalLengthInNano / (_RecurenceInterval * _ResolutionOverInterval);
	int BeatIndex = RelativePointInTime / SmallestIntervalBetweenBeats;
	OutputDebugString(BeatIndex.ToString()->Data());
	OutputDebugString(L"\n");

	return std::find(list.begin(), list.end(), BeatIndex) != list.end();
}


LONG64 Sequencer::GetNanoSec()
{
	Windows::Globalization::Calendar^ c = ref new Windows::Globalization::Calendar;
	c->SetToNow();
	return  c->GetDateTime().UniversalTime;
}
int Sequencer::AddBeat()
{
	//do we need duplication
	if (list.size() >= _RecurenceInterval * _ResolutionOverInterval){
		return -1;
	}

	int Pushable = CurrentTime;
	if(list.end() != std::find(list.begin(), list.end(), CurrentTime))
	{
		Pushable = -1;
		for(int i = 1; i <= Leniancy_In_BeatStrokes; i++){
			if (list.end() == std::find(list.begin(), list.end(), (CurrentTime + i) % (_RecurenceInterval * _ResolutionOverInterval)))
			{
				Pushable = (CurrentTime + i) % (_RecurenceInterval * _ResolutionOverInterval);
			}
			else if (list.end() == std::find(list.begin(), list.end(), (CurrentTime - i) % (_RecurenceInterval * _ResolutionOverInterval)))
			{
				Pushable = (CurrentTime - i) % (_RecurenceInterval * _ResolutionOverInterval);
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
	CurrentTime = 0;
	ReleaseMutex(lock);

}
Sequencer::~Sequencer()
{
	SequenceTimer->Stop();
	CloseHandle(lock);
}


