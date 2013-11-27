#pragma once
#include "pch.h"
#include <list>

using namespace Windows::Foundation;
using namespace Platform;
using namespace Windows::UI::Xaml;
namespace Xaudio2Test 
{
	public delegate void SequencerExecuteDelegate (Platform::Object^ sender, Platform::Object^ e);
	public ref class Sequencer sealed
	{
	private:
		int timeSignatureSeconds;
		SequencerExecuteDelegate^ sequencercorefunc;
		int CurrentTime;
		int TimeResolution;
		HANDLE lock;
		DispatcherTimer^ SequenceTimer;
		std::list<int> list;
		int Speed;
	internal:
	Sequencer::Sequencer(int TimeSeconds, int resolution, SequencerExecuteDelegate^ func);
		void Sequencer::Reset();
		void AddBeat();
		void Sequencer::AddBeatPosition(int Position);
	public:
		virtual ~Sequencer();
	private:
		void WrapperFunc(Platform::Object^ sender, Platform::Object^ e);
	};
}