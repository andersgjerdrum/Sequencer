#include "pch.h"
#include "Sequencer.h"
#include <algorithm>
#include <list>
#include <ppltasks.h>
#define LENIANCY 0.25
#define ONESECONDIN100NANO 10000000L
#define ADJUST 25
using namespace Xaudio2Test;
using namespace Concurrency;
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml;


Sequencer::Sequencer(int TimeSeconds, int resolution, SequencerExecuteDelegate^ func)
{

	sequencercorefunc = func;
	timeSignatureSeconds = TimeSeconds;
	
	StopWatch = 0;
	lock = CreateMutexEx(nullptr,nullptr,0,SYNCHRONIZE);
	//nano second resolution
	Speed = ONESECONDIN100NANO/resolution;
	CurrentTime = 0;
	LastTime = 0;
	TimeResolution = resolution;
	ContinueLoop = true;
	CumulativeOfBy = 0;
    create_task([this] () {    
    }).then([this](){
		while(ContinueLoop){
			LONG64 time =  GetNanoSec();
			WrapperFunc();
			CumulativeOfBy += OffBy();
			WaitFunc((1000/this->TimeResolution) - ADJUST);
			while((GetNanoSec() - time) <  (ONESECONDIN100NANO/this->TimeResolution)){
				continue;
			}
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
	CurrentTime = (CurrentTime + 1 ) % (timeSignatureSeconds * TimeResolution);
	
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
int Sequencer::GetDiff(int CurrentTime, int LastTime)
{
	if(CurrentTime < LastTime){
	  	return (timeSignatureSeconds * TimeResolution - LastTime) + CurrentTime;
	}else{
		return CurrentTime - LastTime;
	}
}
int Sequencer::OffBy()
{
	LONG64 pre = StopWatch;
	StopWatch = GetNanoSec();

	LONG64 OffBy = (LONG64)(StopWatch - pre) - (((ONESECONDIN100NANO/(LONG64)TimeResolution)* (LONG64)GetDiff(CurrentTime,LastTime)));
	float OffsetInMillI = (float)OffBy / (float)10000L;
	float cuminmili = (float)CumulativeOfBy / (float)10000L;
	OutputDebugString(OffsetInMillI.ToString()->Data());
	OutputDebugString(L"\n");
	OutputDebugString(cuminmili.ToString()->Data());
	OutputDebugString(L"\n");
	return OffBy;
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
	if(list.size() >= timeSignatureSeconds * TimeResolution){
		return -1;
	}

	int Pushable = CurrentTime;
	if(list.end() != std::find(list.begin(), list.end(), CurrentTime))
	{
		Pushable = -1;
		int Leniancy_In_BeatStrokes = (int) ((LENIANCY*ONESECONDIN100NANO) / Speed); 
		for(int i = 1; i <= Leniancy_In_BeatStrokes; i++){
			if(list.end() == std::find(list.begin(), list.end(), (CurrentTime + i) % (timeSignatureSeconds * TimeResolution)))
			{
				Pushable = (CurrentTime + i) % (timeSignatureSeconds * TimeResolution);
			}
			else if(list.end() == std::find(list.begin(), list.end(), (CurrentTime - i) % (timeSignatureSeconds * TimeResolution)))
			{
				Pushable = (CurrentTime - i) % (timeSignatureSeconds * TimeResolution );
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
void Sequencer::AddBeatPosition(int Position)
{

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


