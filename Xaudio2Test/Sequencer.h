#pragma once
#include "pch.h"
#include <list>
using namespace Windows::Foundation;
using namespace Platform;
using namespace Windows::UI::Xaml;
namespace Xaudio2Test 
{
	public delegate void SequencerExecuteDelegate (int SequenceId);
	public ref class Sequencer sealed
	{
	private:
		int timeSignatureSeconds;
		bool ContinueLoop;
		SequencerExecuteDelegate^ sequencercorefunc;
		int CurrentTime;
		int LastTime;
		LONG64 StopWatch;
		int TimeResolution;
		LONG64 CumulativeOfBy;
		HANDLE lock;
		DispatcherTimer^ SequenceTimer;
		std::list<int> list;
		int Speed;
	internal:
		Sequencer(int TimeSeconds, int resolution, SequencerExecuteDelegate^ func);
		void Reset();
		int AddBeat();
		void AddBeatPosition(int Position);
		void WaitFunc(int miliseconds);
		LONG64 GetNanoSec();
	public:
		virtual ~Sequencer();
	private:
		int GetDiff(int CurrentTime, int LastTime);
		int OffBy();
		void WrapperFunc();
	};
	
}