#include "pch.h"
#include "Sequencer.h"
#include <algorithm>
#include <list>
#include <ppltasks.h>
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
	lock = CreateMutexEx(nullptr,nullptr,0,SYNCHRONIZE);
	//nano second resolution
	t.Duration = (10000000L/resolution);
	SequenceTimer->Interval = t;
	CurrentTime = 0;
	TimeResolution = resolution;
	SequenceTimer->Start();
	OutputDebugString(L"Start Of Sequencer");


}
void Sequencer::WrapperFunc(Platform::Object^ sender, Platform::Object^ e)
{
	if(CurrentTime >= timeSignatureSeconds * TimeResolution)
	{
		CurrentTime = 0;
	}
	
	CurrentTime++;
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
		sequencercorefunc(sender, e);
	}
}
void Sequencer::AddBeat()
{
	//do we need duplication
	if(list.size() >= timeSignatureSeconds * TimeResolution){
		return;
	}
	int Pushable = CurrentTime;
	if(list.end() != std::find(list.begin(), list.end(), CurrentTime))
	{
		if(list.end() == std::find(list.begin(), list.end(), (CurrentTime + 1) % timeSignatureSeconds * TimeResolution)){
			Pushable = (CurrentTime + 1) % timeSignatureSeconds * TimeResolution;
		}
		else if(list.end() == std::find(list.begin(), list.end(), (CurrentTime - 1) % timeSignatureSeconds * TimeResolution)){
			Pushable = (CurrentTime - 1) % timeSignatureSeconds * TimeResolution;
		}
		else
		{
			return;
		}
	}
	WaitForSingleObjectEx(lock,INFINITE,false);
	list.push_front(Pushable);
	ReleaseMutex(lock);
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