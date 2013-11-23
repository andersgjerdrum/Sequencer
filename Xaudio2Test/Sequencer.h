#pragma once
#include "pch.h"

using namespace Windows::Foundation;
using namespace Platform;
using namespace Windows::UI::Xaml;
namespace Xaudio2Test 
{
	public ref class Sequencer sealed
	{
	private:
		void (*func)(Platform::Object^ sender, Platform::Object^ e);
		int timeSignatureSeconds;
		DispatcherTimer^ SequenceTimer;

		
	internal:
		Sequencer(int TimeSeconds,int BPM, void (*function)(Platform::Object^ sender, Platform::Object^ e));
		void Sequencer::Reset();
		void AddBeat();
	private:
		void WrapperFunc(Platform::Object^ sender, Platform::Object^ e);
	};
}