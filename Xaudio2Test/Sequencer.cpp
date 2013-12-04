#include "pch.h"
#include "Sequencer.h"
#include <algorithm>
#include <list>
#include <ppltasks.h>
#define LENIANCY 0.25
#define ONESECOND 10000000L
using namespace Xaudio2Test;
using namespace Concurrency;
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml;

Sequencer::Sequencer(int TimeSeconds, int resolution, SequencerExecuteDelegate^ func)
{

	sequencercorefunc = func;
	timeSignatureSeconds = TimeSeconds;
	
	TimeSpan t;
	StopWatch = 0;
	lock = CreateMutexEx(nullptr,nullptr,0,SYNCHRONIZE);
	//nano second resolution
	Speed = ONESECOND/resolution;
	CurrentTime = 0;
	LastTime = 0;
	TimeResolution = resolution;
	ContinueLoop = true;

    create_task([this] () {    
    }).then([this](){
		while(ContinueLoop){
			OffBy();
			WrapperFunc();
			int hey = OffBy();
			concurrency::wait((1000/4));
		}
	}, task_continuation_context::use_arbitrary());

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
	int pre = StopWatch;
	SYSTEMTIME st;
	GetLocalTime(&st);
	StopWatch = st.wMilliseconds + (st.wSecond * 1000);

	int OffBy = (StopWatch - pre) - (((1000/TimeResolution)* GetDiff(CurrentTime,LastTime)));

	OutputDebugString(OffBy.ToString()->Data());
	OutputDebugString(L"\n");
	return OffBy;
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
		int Leniancy_In_BeatStrokes = (int) ((LENIANCY*ONESECOND) / Speed); 
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


