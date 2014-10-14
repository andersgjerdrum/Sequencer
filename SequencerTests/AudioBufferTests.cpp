#include "pch.h"
#include "CppUnitTest.h"
#include "AudioBuffer.h"
#include "TestUtility.cpp"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace SequencerLib;
namespace SequencerTests
{
	struct DummySequencer :ISequencer
	{
		std::function<bool(UINT64)> ValidbeatFunc;
		DummySequencer(std::function<bool(UINT64)> func)
		{
			ValidbeatFunc = func;
		}
		bool IsValidBeatPoint(UINT64 SamplePoint){ return ValidbeatFunc(SamplePoint); }
		int AddBeat(UINT64 SamplePoint){ return -1; }

	};
	TEST_CLASS(AudioBufferTests)
	{
	public:

		TEST_METHOD(AudioBuffer_CallsAudioForFillWithTheExpectedpParameters)
		{
			auto dummysequencer = new DummySequencer([](UINT64 a)->bool {
				return false;
			});
			std::vector<byte>  dummy;
			auto buffer = new AudioBuffer(100, dummysequencer, dummy);
			buffer->PrepareBuffer(100, [this](int startindex, int count, int sampleSize, std::vector<byte> *bufferpointer)->void
			{
				Assert::AreEqual(0, startindex, L"Starts at beginning of buffer");
				Assert::AreEqual(50, count, L"Count samples should be half the size of the buffer");
				Assert::AreEqual(50, sampleSize, L"Buffersize is as expected");
			});
		}
		TEST_METHOD(AudioBuffer_WrapsArround)
		{
			auto dummysequencer = new DummySequencer([](UINT64 a)->bool {
				return false;
			});
			std::vector<byte>  dummy;
			auto buffer = new AudioBuffer(100, dummysequencer, dummy);
			buffer->PrepareBuffer(50, [this](int startindex, int count, int buffersize, std::vector<byte> *bufferpointer)->void
			{
				Assert::AreEqual(0, startindex, L"Starts at beginning of buffer");
				Assert::AreEqual(25, count, L"Count samples should be half the size of the buffer");
				Assert::AreEqual(50, buffersize, L"Samplesize is as expected");
			});
			buffer->PrepareBuffer(50, [this](int startindex, int count, int buffersize, std::vector<byte> *bufferpointer)->void
			{
				Assert::AreEqual(25, startindex, L"Starts at middle of buffer");
				Assert::AreEqual(25, count, L"Count samples should be half the size of the buffer");
				Assert::AreEqual(50, buffersize, L"Samplesize is as expected");
			});
			buffer->PrepareBuffer(50, [this](int startindex, int count, int buffersize, std::vector<byte> *bufferpointer)->void
			{
				Assert::AreEqual(0, startindex, L"Starts at beginning of buffer");
				Assert::AreEqual(25, count, L"Count samples should be half the size of the buffer");
				Assert::AreEqual(50, buffersize, L"Samplesize is as expected");
			});
		}
		TEST_METHOD(AudioBuffer_WrapsCorrectlyAtEndOfBuffer)
		{
			auto dummysequencer = new DummySequencer([](UINT64 a)->bool {
				return false;
			});
			std::vector<byte>  dummy;
			auto buffer = new AudioBuffer(100, dummysequencer, dummy);
			buffer->PrepareBuffer(98, [this](int startindex, int count, int buffersize, std::vector<byte> *bufferpointer)->void
			{
				Assert::AreEqual(0, startindex, L"Starts at beginning of buffer");
				Assert::AreEqual(49, count, L"Count samples should be half the size of the buffer");
				Assert::AreEqual(50, buffersize, L"Samplesize is as expected");
			});
			int increment = 0;
			buffer->PrepareBuffer(10, [this, &increment](int startindex, int count, int buffersize, std::vector<byte> *bufferpointer)->void
			{
				if (increment == 0){
					Assert::AreEqual(49, startindex, L"Starts at end of buffer minus 2");
					Assert::AreEqual(1, count, L"Count samples should be half the size of the buffer");
					Assert::AreEqual(50, buffersize, L"Samplesize is as expected");
					increment++;
				}
				else{
					Assert::AreEqual(0, startindex, L"Starts at end of buffer minus 2");
					Assert::AreEqual(4, count, L"Count samples should be half the size of the buffer");
					Assert::AreEqual(50, buffersize, L"Samplesize is as expected");
				}
			});

		}
		//Add  acctual data comparison when supporting sample audio
		TEST_METHOD(AudioBuffer_PreparesBufferCorrectly)
		{
			auto dummysequencer = new DummySequencer([](UINT64 a)->bool {
				return true;
			});
			std::vector<byte> dummy(10);
			TestUtility::FillWith(0xf, &dummy, 10);

			auto buffer = new AudioBuffer(10, dummysequencer, dummy, false);
			buffer->PrepareBuffer(10, [this](int startindex, int count, int buffersize, std::vector<byte> *bufferpointer)->void
			{
				Assert::AreEqual(0, startindex, L"Starts at beginning of buffer");
				Assert::AreEqual(5, count, L"Count samples should be half the size of the buffer");
				Assert::AreEqual(5, buffersize, L"Samplesize is as expected");
				Assert::IsTrue(TestUtility::CheckBuffer(0xf, bufferpointer, 0, buffersize * 2), L"Identical Buffers");
			});
		}

		
		

	};
}