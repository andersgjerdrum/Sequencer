#include "pch.h"
#include "Sequencer.h"
#include <algorithm>
#include <list>
#include <ppltasks.h>
#define LENIANCY (1/4)
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
	SequenceTimer = ref new DispatcherTimer();
	SequenceTimer->Tick += ref new EventHandler<Object^>(this, &Sequencer::WrapperFunc);
	TimeSpan t;
	StopWatch = 0;
	lock = CreateMutexEx(nullptr,nullptr,0,SYNCHRONIZE);
	//nano second resolution
	Speed = ONESECOND/resolution;
	t.Duration = Speed;
	SequenceTimer->Interval = t;
	CurrentTime = 0;
	LastTime = 0;
	TimeResolution = resolution;
	SequenceTimer->Start();
}
void Sequencer::WrapperFunc(Platform::Object^ sender, Platform::Object^ e)
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
		int pre = StopWatch;
		SYSTEMTIME st;
		GetLocalTime(&st);
		StopWatch = st.wMilliseconds + (st.wSecond * 1000);

		int OffBy = (StopWatch - pre) - (((1000/TimeResolution)*((CurrentTime - LastTime) % (timeSignatureSeconds * TimeResolution))));
		OutputDebugString(OffBy.ToString()->Data());
		OutputDebugString(L"\n");
		sequencercorefunc(*findIter);
	}
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
		int Leniancy_In_BeatStrokes = (int) LENIANCY*ONESECOND / Speed; 
		for(int i = 0; i < Leniancy_In_BeatStrokes; i++){
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