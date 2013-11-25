#pragma once
#include "pch.h"
#include <list>

using namespace Windows::Foundation;
using namespace Platform;
using namespace Windows::UI::Xaml;
namespace Xaudio2Test 
{
	public ref class Sequencer sealed
	{
	private:
		int timeSignatureSeconds;
		void (*function)(Platform::Object^ sender, Platform::Object^ e);
		Concurrency::task<void> *asynctask;
		int CurrentTime;
		int TimeResolution;
		DispatcherTimer^ SequenceTimer;
		std::list<double> list;
		
	internal:
		Sequencer(int TimeSeconds,int BPM, void (*function)(Platform::Object^ sender, Platform::Object^ e));
		void Sequencer::Reset();
		void AddBeat();
	private:
		void WrapperFunc(Platform::Object^ sender, Platform::Object^ e);
	};
}