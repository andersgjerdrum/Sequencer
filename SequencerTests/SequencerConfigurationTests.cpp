#include "pch.h"
#include "CppUnitTest.h"
#include "SequencerConfiguration.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace SequencerLib;
namespace SequencerTests
{
	TEST_CLASS(SequencerConfigurationTests)
	{
	public:
		TEST_METHOD(ReturnsCorrectBeatLength)
		{
			// TODO: Your test code here
			auto sequencer = new SequencerConfiguration(2, 4, 1, 200);
			Assert::AreEqual((int)sequencer->GetBeatLength(), 200, L"Beat length retrieved from sequencer configuration matches exected");
		}

		TEST_METHOD(ReturnsCorrectTotalBeatsIfBeatResolutionLowerThanZero)
		{
			auto sequencer = new SequencerConfiguration(4, 0.25, 1, 2100);
			Assert::AreEqual(sequencer->GetTotalBeats(), 1, L"Total Beats retrieved from sequencer configuration matches expected");
		}
		TEST_METHOD(ReturnsCorrectTotalBeats)
		{
			auto sequencer = new SequencerConfiguration(6, 2, 1, 2100);
			Assert::AreEqual(sequencer->GetTotalBeats(), 12, L"Total Beats retrieved from sequencer configuration matches expected");
		}
		TEST_METHOD(ReturnsCorrectBufferSize)
		{
			auto sequencer = new SequencerConfiguration(2, 3, 1, 2100);
			Assert::AreEqual((int)sequencer->GetBufferSize(), BYTESPERSECOND * 2, L"Buffer size returned from sequencer configuration matches expected");
		}

		TEST_METHOD(ReturnsCorrectErrorCorrectiveCoefficient)
		{
			auto sequencer = new SequencerConfiguration(2, 3, 1, 2100);
			Assert::AreEqual(sequencer->GetErrorCorrectiveCoeffitient(), 1, L"Error Corrective Coefficient returned from sequencer configuration matches expected");
		}

		TEST_METHOD(ReturnsCorrectSmallestPointBetweenBeats)
		{
			auto sequencer = new SequencerConfiguration(2, 3, 1, 2100);
			Assert::AreEqual((int)sequencer->GetSmallestPointBetweenBeats(), (BYTESPERSECOND  / 6), L"Smallest point between beats returned from sequencer configuration matches expected");
		}

		TEST_METHOD(ReturnsCorrectRecurrenceInterval)
		{
			auto sequencer = new SequencerConfiguration(2, 3, 1, 2100);
			Assert::AreEqual(sequencer->GetRecurrenceIntervalPerSecond(), 2, L"Smallest point between beats returned from sequencer configuration matches expected");

		}

		TEST_METHOD(ReturnsCorrectResolutionPerSeccond)
		{
			auto sequencer = new SequencerConfiguration(2, 3, 1, 2100);
			Assert::AreEqual((int)sequencer->GetResolutionPerSecond(), 3, L"Smallest point between beats returned from sequencer configuration matches expected");
		}

		TEST_METHOD(ThrowsWhenRecurrenceIntervalIsZero)
		{
			auto func = [this] {auto sequencer = new SequencerConfiguration(0, 3, 1, 2100); };
			Assert::ExpectException<std::invalid_argument>(func, L"Exception thrown when parameter recurrence interval is zero");
		}

		TEST_METHOD(ThrowsWhenResolutionPerSeccondIsZero)
		{
			auto func = [this] {auto sequencer = new SequencerConfiguration(2, 0, 1, 2100); };
			Assert::ExpectException<std::invalid_argument>(func, L"Exception thrown when parameter resolution per seccond is zero");
		}

		TEST_METHOD(ThrowsWhenResolutionPerSeccondDoesNotSumUpToOne)
		{
			auto func = [this] {auto sequencer = new SequencerConfiguration(10, 0.17, 1, 2100); };
			Assert::ExpectException<std::invalid_argument>(func, L"Exception thrown when parameter resolution per seccond does not sum up to one");
		}

		TEST_METHOD(ThrowsWhenResolutionIsLowerThanRecurrenceInterval)
		{
			auto func = [this] {auto sequencer = new SequencerConfiguration(1, 0.17, 1, 2100); };
			Assert::ExpectException<std::invalid_argument>(func, L"Exception thrown when parameter resolution is lower than recurrence interval");
		}


		TEST_METHOD(DoesNotThrowsWhenResolutionPerSeccondSumUpToOne)
		{
			auto sequencer = new SequencerConfiguration(10, 0.20, 1, 2100);
			Assert::IsTrue(true, L"Exception not thrown when parameter resolution per seccond sum up to one");
		}

		TEST_METHOD(DoesNotThrowWhenErrorCorrectingCoefficientIsZero)
		{
			auto sequencer = new SequencerConfiguration(2, 1, 0, 2100);
			Assert::IsTrue(true, L"Exception not thrown when error correcting coefficient is zero");
		}

		TEST_METHOD(ThrowsWhenBeatLengthIsGreaterThanSmallestPointBetweenBeats)
		{
			auto func = [this] {auto sequencer = new SequencerConfiguration(2, 3, 1, (BYTESPERSECOND / 6) + 1); };
			Assert::ExpectException<std::invalid_argument>(func, L"Exception thrown when parameter beat length is greater than smallest point between beats");
		}
	};
}