#pragma once
#include "pch.h"

#define SECONDS 5
#define SAMPLERATE 44100
#define DUALBAND 2
#define BITSPERSAMPLE 16
#define DATASIZE (DUALBAND * SECONDS * SAMPLERATE)
#define BYTESPERSECOND (SAMPLERATE * DUALBAND)
#define CYCLE 441 // A above middle C 
#define SAMPLE 100
#define SHORT_MIN -32768
#define SHORT_MAX 32767
#define BITMASK_SHORT 0XFF
#define ONESECONDIN100NANO 10000000L
#define ADJUST 25

namespace Xaudio2Test 
{
	class Audio : public IXAudio2VoiceCallback 
	{
	private:
		int _RecurenceInterval;
		int Leniancy_In_BeatStrokes;
		int _ResolutionPerSecond;
		HANDLE lock;
		LONG64 GetNanoSec();
		UINT64 _SmallestPointBetweenBeats;
		UINT64 index;
        int angle;
        int angleIncrement;
		static const int BUFFER_LENGTH = BYTESPERSECOND;
		static const int WAVEFORM_LENGTH = 8192;
		short * pWaveformBuffer;
		Microsoft::WRL::ComPtr<IXAudio2> pXAudio2;
		IXAudio2MasteringVoice * pMasteringVoice;
		IXAudio2SourceVoice * pSourceVoice;
		bool ContinuousPlay;
	public:
		void SetFrequency(float freq);
        void SetAmplitude(float amp);
		Audio(IXAudio2* pXAudio2, int RecurrenceInterval, int InaccuracyCoefficient, int ResolutionPerSecond, bool Continuous);
		~Audio();
		void Reset();
		std::list<int> list;
		int AddBeat();
		//Callbacks requred for IXAudio2VoiceCallback
		void _stdcall OnVoiceProcessingPassStart(UINT32 bytesRequired);
        void _stdcall OnVoiceProcessingPassEnd(){}
        void _stdcall OnStreamEnd(){}
        void _stdcall OnBufferStart(void* pContext){};
        void _stdcall OnBufferEnd(void* pContext){};
        void _stdcall OnLoopEnd(void*){}
        void _stdcall OnVoiceError(void*, HRESULT){}
	private:
		void FillAndSubmit(int startIndex, int count);
		void CreateBlank(int startIndex, int count);
		int GetIndexFromTime(LONG64 time);
		UINT64 GetBufferReadFromSourceVoice(void);
		int GetIndexFromBufferSample(UINT64 BufferPoint);
		bool NextBeat(UINT64 bufferplace);
		bool IsValidBeatPoint(UINT64 index);
		void CreateBeat(int startIndex, int count);
	};

}
