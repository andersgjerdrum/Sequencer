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
	OutputDebugString(L"Start Of beat");
	if(CurrentTime >= timeSignatureSeconds * TimeResolution)
	{
		CurrentTime = 0;
	}
	if(list.size() < 0)
	{
		return;
	}
	//dotimestuff
	WaitForSingleObjectEx(lock,INFINITE,false);
	std::list<double>::iterator findIter = std::find(list.begin(), list.end(), CurrentTime);
	ReleaseMutex(lock);
	if(*findIter == CurrentTime)
	{
		sequencercorefunc(sender, e);
	}
	CurrentTime++;
}
void Sequencer::AddBeat()
{
	//do we need duplication
	WaitForSingleObjectEx(lock,INFINITE,false);
	list.push_front(CurrentTime);
	ReleaseMutex(lock);
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
	CloseHandle(lock);
	SequenceTimer->Stop();
}