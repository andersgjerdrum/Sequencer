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

Sequencer::Sequencer(int TimeSeconds, int resolution, void (*function)(Platform::Object^ sender, Platform::Object^ e))
{
	asynctask = Concurrency::task<void>([](void (*func)(Platform::Object^ sender, Platform::Object^ e),Platform::Object^ sender, Platform::Object^ e)
		{
       			func(sender,e);
		});
	function = function;
	timeSignatureSeconds = TimeSeconds;
	SequenceTimer = ref new DispatcherTimer();
	SequenceTimer->Tick += ref new EventHandler<Object^>(this, &Sequencer::WrapperFunc);
	TimeSpan t;
	//nano second resolution
	t.Duration = (10000000L/resolution);
	SequenceTimer->Interval = t;
	CurrentTime = 0;
	TimeResolution = resolution;
	SequenceTimer->Start();

}
void Sequencer::WrapperFunc(Platform::Object^ sender, Platform::Object^ e)
{
	OutputDebugString(L"Start Of beat");
	if(CurrentTime >= timeSignatureSeconds * TimeResolution)
	{
		CurrentTime = 0;
	}
	if(list.size < 0)
	{
		return;
	}
	//dotimestuff
	std::list<double>::iterator findIter = std::find(list.begin(), list.end(), CurrentTime);
	if(*findIter == CurrentTime)
	{
		//todo: figure out how to make async
	}
	CurrentTime++;
}
void Sequencer::AddBeat()
{

}


void Sequencer::Reset()
{

}