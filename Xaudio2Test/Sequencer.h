#pragma once
#include "pch.h"
#include <list>
using namespace Windows::Foundation;
using namespace Platform;
using namespace Windows::UI::Xaml;
namespace Xaudio2Test 
{
	class Sequencer
	{
	private:
		int _RecurenceInterval;
		int Leniancy_In_BeatStrokes;
		int _ResolutionPerSecond;
		HANDLE lock;
		LONG64 GetNanoSec();
	public:
		Sequencer(int RecurrenceInterval, int ResolutionOverInterval, float InaccuracyCoefficient);
		void Reset();
		std::list<int> list;
		int AddBeat();
		virtual ~Sequencer();
	private:
		int GetIndexFromTime(LONG64 time);
	};
	
}