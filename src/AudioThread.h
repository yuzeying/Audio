//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef AUDIO_THREAD_H
#define AUDIO_THREAD_H

#include "SndCallRegistry.h"
#include "VoiceCallback.h"
#include "XAudio2Wrapper.h"
#include "CircularData.h"
#include "VoiceManager.h"

class AudioThreadData {
public:
	WAVEFORMATEXTENSIBLE playlistwfx[8];
	XAUDIO2_BUFFER playlistBuffers[8];
};

IXAudio2* pXAudio2 = nullptr;
IXAudio2MasteringVoice* pMasterVoice = nullptr;

IXAudio2* GetAudio() {
	return pXAudio2;
}

void InitializeXAudio2()
{
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	HRESULT hr;

	if (FAILED(hr = XAudio2Create(&pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR)))
	{
		printf("error %x", (unsigned int)hr);
		assert(false);
	}

	if (FAILED(hr = pXAudio2->CreateMasteringVoice(&pMasterVoice)))
	{
		printf("error %x", hr);
		assert(false);
	}

	VoiceManager::pX2 = pXAudio2;
}



IXAudio2SourceVoice* CallSound(CircularData* pOut, WAVEFORMATEXTENSIBLE& wfx, XAUDIO2_BUFFER& buffer, VoiceCallback* pCallback)
{
	HRESULT hr;
	pOut = pOut + 0;



	// -------------------------------------------------------
	// Create a source voice
	//		Format in wfx
	// -------------------------------------------------------

	IXAudio2SourceVoice* pSourceVoice;


	hr = pXAudio2->CreateSourceVoice(&pSourceVoice, (WAVEFORMATEX*)&wfx, 0, XAUDIO2_MAX_FREQ_RATIO, pCallback);
	assert(hr == S_OK);

	//if (FAILED(hr = pXAudio2->CreateSourceVoice(&pSourceVoice, (WAVEFORMATEX*)&wfx)))
	//{
	//	assert(false);
	//}

	// -------------------------------------------------------
	// Create a move the voice onto the voice queue
	//		Only files on the queue can be played
	//		There is no deep copy, we still hold the buffer while its active
	// -------------------------------------------------------

	if (FAILED(hr = pSourceVoice->SubmitSourceBuffer(&buffer)))
	{
		assert(false);
	}

	// -------------------------------------------------------
	// Start the source voice
	// -------------------------------------------------------
	pSourceVoice->SetVolume(0.8f);
	if (FAILED(hr = pSourceVoice->Start(0)))
	{
		assert(false);
	}

	return pSourceVoice;
}


void AudioMain(CircularData* pIn, CircularData* pOut)
{
	pOut = pOut;
	ThreadHelper::DebugBegin(1);
	InitializeXAudio2();

	float pan = -1.0f;

	//// Getting the channel mask of master for panning
	DWORD dwChannelMask;
	pMasterVoice->GetChannelMask(&dwChannelMask);
	float outputMatrix[8];
	for (int i = 0; i < 8; i++) outputMatrix[i] = 0;
	XAUDIO2_VOICE_DETAILS VoiceDetails;
	XAUDIO2_VOICE_DETAILS MasterVoiceDetails;
	pMasterVoice->GetVoiceDetails(&MasterVoiceDetails);

	//// updating the current panning settings of the outputMatrix
	outputMatrix[0] = 0.5f - pan / 2;;
	outputMatrix[1] = 0.5f + pan / 2;

	//IXAudio2SourceVoice* voices[8] = { 0 };
	//int lastUsedId = -1;

	while (1)
	{
		int val;

		if (pIn->PopFront(val) == true)
		{
			// Quit - hack
			if (val == 0xABABABAB)
			{
				break;
			}

			Trace::out("\n");
			Trace::out("A:%x \n", val);

			if (val == (int)SndId::AdjustID) {
				int id, action, value;
				if (!pIn->PopFront(id)) continue;
				if (!pIn->PopFront(action)) continue;
				if (!pIn->PopFront(value)) continue;

				IXAudio2SourceVoice* voice = VoiceManager::GetVoiceByUniqueId(id);

				// stop
				if (action == -1) voice->Stop();

				// play
				if (action == (int)SndId::Play) voice->Start();

				// volume
				if (action == (int)SndId::AdjustVolume) voice->SetVolume((float)value / 100.0f);

				// pan
				if (action == (int)SndId::AdjustPan) {
					pan = ((float)value - 50.0f) / 50.0f; // value of 0 is hard left, 50 middle, 100 hard right
					outputMatrix[0] = 0.5f - pan / 2;;
					outputMatrix[1] = 0.5f + pan / 2;
					voice->GetVoiceDetails(&VoiceDetails);
					voice->SetOutputMatrix(NULL, VoiceDetails.InputChannels, MasterVoiceDetails.InputChannels, outputMatrix);
				}
				
				// pitch
				if (action == (int)SndId::AdjustPitch) {
					voice->SetFrequencyRatio((float)value / 100.0f);
				}
			}

		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	Trace::out("Ending sound loop");
	// Fall through... then end thread
	ThreadHelper::DebugEnd(1);
}
#endif

// End of File 
