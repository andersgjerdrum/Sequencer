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


namespace Xaudio2Test 
{
	class Audio : public IXAudio2VoiceCallback 
	{
	private:
		int index;
        int angle;
        int angleIncrement;
		static const int BUFFER_LENGTH = 1024;
		static const int WAVEFORM_LENGTH = 8192;
		short waveformBuffer[BUFFER_LENGTH];
		short * pWaveformBuffer;
		Microsoft::WRL::ComPtr<IXAudio2> pXAudio2;
		IXAudio2MasteringVoice * pMasteringVoice;
		IXAudio2SourceVoice * pSourceVoice;
		bool ContinuousPlay;
	public:
		void SetFrequency(float freq);
        void SetAmplitude(float amp);
		void Audio::StartFillSubmitStop();
		Audio(IXAudio2* pXAudio2, bool Continuous);
		~Audio();

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


	};

}
