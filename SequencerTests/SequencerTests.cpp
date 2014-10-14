#include "pch.h"
#include "CppUnitTest.h"
#include "SequencerConfiguration.h"
#include "Sequencer.h"
#include "TestUtility.cpp"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace SequencerLib;
namespace SequencerTests
{
	TEST_CLASS(SequencerTests)
	{
	public:
		TEST_METHOD(Sequnencer_SingleBeatAllowsSingleInsert)
		{
			auto conf = new SequencerConfiguration(1, 1, 0, 1);
			auto seq = new Sequencer(conf);
			Assert::AreEqual(seq->AddBeat(0), 0, L"Adding beat at beginning, gives first index in sequence");
			Assert::AreEqual(conf->GetBufferSize() / 2, conf->GetSmallestPointBetweenBeats(), L"Only one beat allowed");
			Assert::AreEqual(seq->AddBeat(conf->GetBufferSize()), -1, L"Adding another at end will give -1 because single beat allready inserted");
		}
		TEST_METHOD(Sequencer_FullBeatListDeniesEntries)
		{
			auto conf = new SequencerConfiguration(10, 10, 0, 1);
			auto seq = new Sequencer(conf);
			TestUtility::FillBeats(seq, conf, 10 * 10);
			Assert::IsFalse(TestUtility::AnyAvailible(seq, conf, 10 * 10), L"Cannot Add more beats");
		}
		TEST_METHOD(Sequencer_ErrorCorrective_if_after_is_taken)
		{
			auto conf = new SequencerConfiguration(2, 2, 1, 1);
			auto seq = new Sequencer(conf);
			Assert::AreEqual(0, seq->AddBeat(0), L"Adding first in beat is OK");
			Assert::AreEqual(2, seq->AddBeat(conf->GetSmallestPointBetweenBeats() * 2), L"Adding seccond is OK");
			Assert::AreEqual(3, seq->AddBeat(conf->GetSmallestPointBetweenBeats() * 3), L"Adding third is OK");
			Assert::AreEqual(1, seq->AddBeat(conf->GetSmallestPointBetweenBeats() * 2), L"Adding seccond should trigger 1");
		}
		TEST_METHOD(Sequencer_ErrorCorrection_LongerCorrective)
		{
			auto conf = new SequencerConfiguration(2, 2, 2, 1);
			auto seq = new Sequencer(conf);
			Assert::AreEqual(0, seq->AddBeat(0), L"Adding first in beat is OK");
			Assert::AreEqual(1, seq->AddBeat(conf->GetSmallestPointBetweenBeats()), L"Adding seccond is OK");
			Assert::AreEqual(2, seq->AddBeat(conf->GetSmallestPointBetweenBeats() * 2), L"Adding third is OK");
			Assert::AreEqual(3, seq->AddBeat(conf->GetSmallestPointBetweenBeats()), L"Adding seccond should trigger fourth");
		}
		TEST_METHOD(Sequencer_ValidBeatPoint_SinglePointBeat)
		{
			auto conf = new SequencerConfiguration(2, 2, 2, 1);
			auto seq = new Sequencer(conf);
			TestUtility::FillBeats(seq, conf, 2 * 2);
			for (int i = 0; i < (conf->GetBufferSize() / 2); i = i + conf->GetSmallestPointBetweenBeats())
			{
				Assert::IsFalse(seq->IsValidBeatPoint(i - 1), L"Is Not Valid Beat Point");
				Assert::IsTrue(seq->IsValidBeatPoint(i), L"Is Valid Beat Point");
				Assert::IsFalse(seq->IsValidBeatPoint(i + 1), L"Is Not Valid Beat Point");
			}
		}
		TEST_METHOD(Sequencer_ValidBeatPoint_LongerBeat)
		{
			auto conf = new SequencerConfiguration(2, 2, 2, 100);
			auto seq = new Sequencer(conf);
			TestUtility::FillBeats(seq, conf, 2 * 2);
			for (int i = 0; i < (conf->GetBufferSize() / 2); i = i + conf->GetSmallestPointBetweenBeats())
			{
				Assert::IsFalse(seq->IsValidBeatPoint(i - 100), L"Is Not Valid Beat Point");
				Assert::IsFalse(seq->IsValidBeatPoint(i - 50), L"Is Not Valid Beat Point");
				Assert::IsTrue(seq->IsValidBeatPoint(i), L"Is Valid Beat Point");
				Assert::IsTrue(seq->IsValidBeatPoint(i + 50), L"Is Valid Beat Point");
				Assert::IsFalse(seq->IsValidBeatPoint(i + 100), L"Is Not Valid Beat Point");
			}
		}
	};
}