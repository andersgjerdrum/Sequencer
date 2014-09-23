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
			Assert::AreEqual((int)sequencer->GetBeatLength(), 200, L"Beat length retrieved from sequencer configuration matches exected");
		}

		TEST_METHOD(SequencerConfiguration_ReturnsCorrectTotalBeats)
		{
			auto sequencer = new SequencerConfiguration(2, 3, 1, 2100);
			Assert::AreEqual(sequencer->GetTotalBeats(), 6, L"Total Beats retrieved from sequencer configuration matches expected");
		}

		TEST_METHOD(SequencerConfiguration_ReturnsCorrectBufferSize)
		{
			auto sequencer = new SequencerConfiguration(2, 3, 1, 2100);
			Assert::AreEqual((int)sequencer->GetBufferSize(), BYTESPERSECOND * 2, L"Buffer size returned from sequencer configuration matches expected");
		}

		TEST_METHOD(SequencerConfiguration_ReturnsCorrectErrorCorrectiveCoefficient)
		{
			auto sequencer = new SequencerConfiguration(2, 3, 1, 2100);
			Assert::AreEqual(sequencer->GetErrorCorrectiveCoeffitient(), 1, L"Error Corrective Coefficient returned from sequencer configuration matches expected");
		}

		TEST_METHOD(SequencerConfiguration_ReturnsCorrectSmallestPointBetweenBeats)
		{
			auto sequencer = new SequencerConfiguration(2, 3, 1, 2100);
			Assert::AreEqual((int)sequencer->GetSmallestPointBetweenBeats(), (BYTESPERSECOND  / 6), L"Smallest point between beats returned from sequencer configuration matches expected");
		}

		TEST_METHOD(SequencerConfiguration_ReturnsCorrectRecurrenceInterval)
		{
			auto sequencer = new SequencerConfiguration(2, 3, 1, 2100);
			Assert::AreEqual(sequencer->GetRecurrenceIntervalPerSecond(), 2, L"Smallest point between beats returned from sequencer configuration matches expected");

		}

		TEST_METHOD(SequencerConfiguration_ReturnsCorrectResolutionPerSeccond)
		{
			auto sequencer = new SequencerConfiguration(2, 3, 1, 2100);
			Assert::AreEqual(sequencer->GetResolutionPerSecond(), 3, L"Smallest point between beats returned from sequencer configuration matches expected");
		}

		TEST_METHOD(SequencerConfiguration_ThrowsWhenRecurrenceIntervalIsZero)
		{
			auto func = [this] {auto sequencer = new SequencerConfiguration(0, 3, 1, 2100); };
			Assert::ExpectException<std::invalid_argument>(func, L"Exception thrown when parameter recurrence interval is zero");
		}

		TEST_METHOD(SequencerConfiguration_ThrowsWhenResolutionPerSeccondIsZero)
		{
			auto func = [this] {auto sequencer = new SequencerConfiguration(2, 0, 1, 2100); };
			Assert::ExpectException<std::invalid_argument>(func, L"Exception thrown when parameter resolution per seccond is zero");
		}

		TEST_METHOD(SequencerConfiguration_DoesNotThrowWhenErrorCorrectingCoefficientIsZero)
		{
			auto sequencer = new SequencerConfiguration(2, 1, 0, 2100);
			Assert::IsTrue(true, L"Exception not thrown when error correcting coefficient is zero");
		}

		TEST_METHOD(SequencerConfiguration_ThrowsWhenBeatLengthIsGreaterThanSmallestPointBetweenBeats)
		{
			auto func = [this] {auto sequencer = new SequencerConfiguration(2, 3, 1, (BYTESPERSECOND / 6) + 1); };
			Assert::ExpectException<std::invalid_argument>(func, L"Exception thrown when parameter beat length is greater than smallest point between beats");
		}
	};
}