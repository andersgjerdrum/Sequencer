//// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//// PARTICULAR PURPOSE.
////
//// Source: https://code.msdn.microsoft.com/windowsapps/XAudio2-Stream-Effect-3f95c8f2/view/SourceCode
//// Copyright (c) Microsoft Corporation. All rights reserved

#pragma once
#include "pch.h"
#include <roapi.h>
#include <mfidl.h>
#include <mfapi.h>
#include <mfreadwrite.h>
using namespace Windows::Storage::Streams;

namespace SequencerLib
{
	class AudioStream
	{
	public:
		AudioStream(IRandomAccessStream^ streamHandle);

		void Restart();
		std::vector<byte> ReadAll();
		std::vector<byte> GetNextBuffer();

		const WAVEFORMATEX& GetOutputWaveFormatEx() const
		{
			return m_waveFormat;
		}

		uint32 GetMaxStreamLengthInBytes() const
		{
			return m_maxStreamLengthInBytes;
		}

	private:
		void ThrowIfFailed(HRESULT hr);
		Microsoft::WRL::ComPtr<IMFSourceReader> m_reader;
		WAVEFORMATEX                            m_waveFormat;
		uint32                                  m_maxStreamLengthInBytes;
	};
}