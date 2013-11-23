#include "pch.h"
#include "Sequencer.h"



using namespace Xaudio2Test;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml;

Sequencer::Sequencer(int TimeSeconds,int BPM, void (*function)(Platform::Object^ sender, Platform::Object^ e))
{
	func = function;
	timeSignatureSeconds = TimeSeconds;
	SequenceTimer = ref new DispatcherTimer();
		SequenceTimer->Tick += ref new EventHandler<Object^>(this, &Sequencer::WrapperFunc);
		TimeSpan t;
		t.Duration=10000000/2;
		SequenceTimer->Interval = t;
		SequenceTimer->Start();
}
void Sequencer::WrapperFunc(Platform::Object^ sender, Platform::Object^ e)
{
	//dotimestuff
	
	func(sender,e);

}
void Sequencer::AddBeat()
{

}
void Sequencer::Reset()
{

}