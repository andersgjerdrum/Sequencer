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
		double StopWatch;
		int TimeResolution;
		double CumulativeOfBy;
		HANDLE lock;
		DispatcherTimer^ SequenceTimer;
		std::list<int> list;
		int Speed;
	internal:
		Sequencer::Sequencer(int TimeSeconds, int resolution, SequencerExecuteDelegate^ func);
		void Sequencer::Reset();
		int AddBeat();
		void Sequencer::AddBeatPosition(int Position);
	public:
		virtual ~Sequencer();
	private:
		int GetDiff(int CurrentTime, int LastTime);
		int OffBy();
		void WrapperFunc();
	};
	
}