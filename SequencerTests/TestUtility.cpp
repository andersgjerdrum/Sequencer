#include <windows.h>
#include "pch.h"
#include <vector>
#include "SequencerConfiguration.h"
#include "Sequencer.h"
using namespace SequencerLib;
namespace SequencerTests
{
	struct  TestUtility
	{
		static bool CheckBuffer(byte symbol, std::vector<byte> *bufferpointer, int start, int size)
		{
			for (int i = start; i < size; i++)
			{
				byte b = bufferpointer->at(i);
				if (b != symbol){
					return false;
				}
			}
			return true;
		}
		static void FillWith(byte Symbol, std::vector<byte>  *buffer, int size)
		{
			for (int i = 0; i < size; i++){
				buffer->at(i) = Symbol;
			}
		}
		static void FillBeats(ISequencer *seq, ISequencerConfiguration *conf, int count)
		{
			for (UINT64 i = 0; i < (conf->GetBufferSize() / 2); i = i + conf->GetSmallestPointBetweenBeats())
			{
				seq->AddBeat(i) == -1;
			}
		}

		static bool AnyAvailible(ISequencer *seq, ISequencerConfiguration *conf, int count)
		{
			for (UINT64 i = 0; i < (conf->GetBufferSize() / 2); i = i + conf->GetSmallestPointBetweenBeats())
			{
				if (seq->AddBeat(i) != -1){
					return true;
				}
			}
			return false;
		}

	};
}