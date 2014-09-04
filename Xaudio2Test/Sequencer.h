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
		int _RecurenceInterval;
		bool ContinueLoop;
		int Leniancy_In_BeatStrokes;
		SequencerExecuteDelegate^ sequencercorefunc;
		int CurrentTime;
		int LastTime;
		LONG64 StopWatch;
		int _ResolutionOverInterval;
		HANDLE lock;
		DispatcherTimer^ SequenceTimer;
		std::list<int> list;
		int Speed;
		void WaitFunc(int miliseconds);
		LONG64 GetNanoSec();
		void CreateTask(void);
	internal:
		Sequencer(int RecurrenceInterval, int ResolutionOverInterval, float InaccuracyCoefficient, SequencerExecuteDelegate^ CoreFunctionToCall);
		void Reset();
		int AddBeat();
	public:
		virtual ~Sequencer();
	private:
		void WrapperFunc();
	};
	
}