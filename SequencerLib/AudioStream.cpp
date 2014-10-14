//// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//// PARTICULAR PURPOSE.
////
//// Source: https://code.msdn.microsoft.com/windowsapps/XAudio2-Stream-Effect-3f95c8f2/view/SourceCode
//// Copyright (c) Microsoft Corporation. All rights reserved

#include "pch.h"

#include "AudioStream.h"

#include <initguid.h>
using namespace SequencerLib;
using namespace Windows::Storage::Streams;
using namespace Microsoft::WRL;


AudioStream::AudioStream(IRandomAccessStream^ streamHandle)
{
	Microsoft::WRL::ComPtr<IMFMediaType> outputMediaType;
	Microsoft::WRL::ComPtr<IMFMediaType> mediaType;

	ThrowIfFailed(
		MFStartup(MF_VERSION)
		);

	//
	// Build an attribute object to set MF_LOW_LATENCY
	// Each title should evaluate whether the latency v. power trade-off is appropriate
	// Longer running music tracks that are not latency sensitive may need this flag less than shorter sounds.
	// This sample is using the flag on a music track for demonstrative purposes.
	//
	Microsoft::WRL::ComPtr<IMFAttributes> lowLatencyAttribute;
	ThrowIfFailed(
		MFCreateAttributes(&lowLatencyAttribute, 1)
		);

	ThrowIfFailed(
		lowLatencyAttribute->SetUINT32(MF_LOW_LATENCY, TRUE)
		);

	//
	// Create the source reader on the streamHandle (file) with the low latency attributes
	//
	ComPtr<IUnknown> pStreamUnk = reinterpret_cast<IUnknown*>(streamHandle);
	ComPtr<IMFByteStream> pMFStream;
	ThrowIfFailed(MFCreateMFByteStreamOnStreamEx(pStreamUnk.Get(), &pMFStream));

	ThrowIfFailed(MFCreateSourceReaderFromByteStream(pMFStream.Get(), NULL, &m_reader));

	//ThrowIfFailed(
	//	MFCreateSourceReaderFromByteStream(pByteStream, lowLatencyAttribute.Get(), &m_reader)
	//	);

	// Set the decoded output format as PCM
	// XAudio2 on Windows can process PCM and ADPCM-encoded buffers.
	// When using MF, this sample always decodes into PCM.

	ThrowIfFailed(
		MFCreateMediaType(&mediaType)
		);

	ThrowIfFailed(
		mediaType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio)
		);

	ThrowIfFailed(
		mediaType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM)
		);

	ThrowIfFailed(
		m_reader->SetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, 0, mediaType.Get())
		);

	// Get the complete WAVEFORMAT from the Media Type
	ThrowIfFailed(
		m_reader->GetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, &outputMediaType)
		);

	uint32 formatByteCount = 0;
	WAVEFORMATEX* waveFormat;
	ThrowIfFailed(
		MFCreateWaveFormatExFromMFMediaType(outputMediaType.Get(), &waveFormat, &formatByteCount)
		);
	CopyMemory(&m_waveFormat, waveFormat, sizeof(m_waveFormat));
	CoTaskMemFree(waveFormat);

	// Get the total length of the stream in bytes
	PROPVARIANT var;
	ThrowIfFailed(
		m_reader->GetPresentationAttribute(MF_SOURCE_READER_MEDIASOURCE, MF_PD_DURATION, &var)
		);
	LONGLONG duration = var.uhVal.QuadPart;
	float64 durationInSeconds = (duration / (float64)(10000 * 1000));
	m_maxStreamLengthInBytes = (uint32)(durationInSeconds * m_waveFormat.nAvgBytesPerSec);
}

std::vector<byte> AudioStream::GetNextBuffer()
{
	std::vector<byte> resultData;

	Microsoft::WRL::ComPtr<IMFSample> sample;
	Microsoft::WRL::ComPtr<IMFMediaBuffer> mediaBuffer;

	uint8* audioData = nullptr;
	uint32 sampleBufferLength = 0;
	uint32 flags = 0;

	ThrowIfFailed(
		m_reader->ReadSample(MF_SOURCE_READER_FIRST_AUDIO_STREAM, 0, nullptr, reinterpret_cast<DWORD*>(&flags), nullptr, &sample)
		);

	// End of stream
	if (flags & MF_SOURCE_READERF_ENDOFSTREAM)
	{
		return resultData;
	}
	if (sample == nullptr)
	{
		throw ref new Platform::FailureException();
	}

	ThrowIfFailed(
		sample->ConvertToContiguousBuffer(&mediaBuffer)
		);

	ThrowIfFailed(
		mediaBuffer->Lock(&audioData, nullptr, reinterpret_cast<DWORD*>(&sampleBufferLength))
		);

	//
	// Prepare and return the resultant array of data
	//
	resultData.resize(sampleBufferLength);
	CopyMemory(&resultData[0], audioData, sampleBufferLength);

	// Release the lock
	ThrowIfFailed(
		mediaBuffer->Unlock()
		);

	return resultData;
}

std::vector<byte> AudioStream::ReadAll()
{
	std::vector<byte> resultData;

	// Make sure stream is set to start
	// Restart();

	for (;;)
	{
		std::vector<byte> nextBuffer = GetNextBuffer();
		if (nextBuffer.size() == 0)
		{
			break;
		}

		// Append the new buffer to the running total
		size_t lastBufferSize = resultData.size();
		resultData.resize(lastBufferSize + nextBuffer.size());
		CopyMemory(&resultData[lastBufferSize], &nextBuffer[0], nextBuffer.size());
	}

	return resultData;
}

void AudioStream::Restart()
{
	PROPVARIANT var;
	ZeroMemory(&var, sizeof(var));
	var.vt = VT_I8;

	ThrowIfFailed(
		m_reader->SetCurrentPosition(GUID_NULL, var)
		);

}
void AudioStream::ThrowIfFailed(HRESULT hr){
	if (FAILED(hr))
	{
		throw new std::exception("AudioStream failure");
	}
}