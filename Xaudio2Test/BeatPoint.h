#pragma once
#include "pch.h"
#include <list>

using namespace Windows::Foundation;
using namespace Platform;
using namespace Windows::UI::Xaml;

namespace Xaudio2Test 
{
	class BeatPoint{
	public:
		BeatPoint(int x, int y, int sequenceId);
		int Xaxis;
		int Yaxis;
		int SequenceId;
	
	};
	class BeatPoints{
	public:
		BeatPoints();
		void AddPoint(BeatPoint bp);
		BeatPoint Find(int sequenceId);
	private:
		std::map<int,BeatPoint> pointStorage;

	};
}
