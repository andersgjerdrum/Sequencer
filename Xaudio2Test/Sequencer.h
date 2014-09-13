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
		int _RecurenceInterval;
		int Leniancy_In_BeatStrokes;
		int _ResolutionOverInterval;
		HANDLE lock;
		LONG64 GetNanoSec();
	internal:
		Sequencer(int RecurrenceInterval, int ResolutionOverInterval, float InaccuracyCoefficient);
		void Reset();
		std::list<int> list;
		int AddBeat();
	public:
		virtual ~Sequencer();
	private:
		int GetIndexFromTime(LONG64 time);
	};
	
}