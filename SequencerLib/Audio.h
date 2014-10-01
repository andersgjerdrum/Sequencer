#pragma once
#include "pch.h"
#include "IAudioBuffer.h"

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

namespace SequencerLib
{
	class Audio : public IXAudio2VoiceCallback 
	{
	private:
		IAudioBuffer * BufferObject;
		//static const int BUFFER_LENGTH = BYTESPERSECOND * 2;
		Microsoft::WRL::ComPtr<IXAudio2> pXAudio2;
		IXAudio2MasteringVoice * pMasteringVoice;
		IXAudio2SourceVoice * pSourceVoice;
	public:
        void SetAmplitude(float amp);
		UINT64 GetBufferReadFromSourceVoice(void);
		Audio(IXAudio2* pXAudio2, IAudioBuffer* buffer);
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
		void FillAndSubmit(int startIndex, int count, int bytes, byte *bufferAddr);
	};

}
