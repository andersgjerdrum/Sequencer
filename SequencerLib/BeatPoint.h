#pragma once
#include "pch.h"
#include <map>

using namespace Windows::Foundation;
using namespace Platform;
using namespace Windows::UI::Xaml;
using namespace std;
namespace SequencerLib
{
	class BeatPoint{
	public:
		BeatPoint();
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
		HANDLE lock;

	private:
		std::map<int,BeatPoint> pointStorage;

	};
}
