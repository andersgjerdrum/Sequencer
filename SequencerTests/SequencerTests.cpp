#include "pch.h"
#include "CppUnitTest.h"
#include "SequencerConfiguration.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace SequencerLib;
namespace SequencerTests
{
	TEST_CLASS(SequencerTests)
	{
	public:
		TEST_METHOD(SequencerConfiguration_ReturnsCorrectBeatLength)
		{
			// TODO: Your test code here
			auto sequencer = new SequencerConfiguration(2, 4, 1, 200);
			Assert::AreEqual(200, (int)sequencer->GetBeatLength(), L"Beat length retrieved from sequencer configuration matches exected");
		}
		TEST_METHOD(SequencerConfiguration_ReturnsCorrectTotalBeats)
		{
			auto sequencer = new SequencerConfiguration(2, 3, 1, 2100);
			Assert::AreEqual(6, sequencer->GetTotalBeats(), L"Total Beats retrieved from sequencer configuration matches expected");
		}
	};
}