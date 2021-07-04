//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef _VOICE_CALLBACK_H_
#define _VOICE_CALLBACK_H_

#include "XAudio2Wrapper.h"
#include "CircularData.h"
#include "SndCallRegistry.h"
#include "VoiceManager.h"
#include <thread>

//  Sample voice callback
class VoiceCallback : public IXAudio2VoiceCallback
{
public:
	bool   finished = false;
	CircularData* pOut;
	CircularData* pin;

	char* nextName;
	int nextUniqueId;
	char* thisName;

	VoiceCallback(CircularData* _pOut)
	{
		assert(_pOut);
		this->pOut = _pOut;
	}

	VoiceCallback(CircularData* _pOut, char *pNextName, int nextUniqueID)
	{
		assert(_pOut);
		this->pOut = _pOut;
		this->nextName = pNextName;
		this->nextUniqueId = nextUniqueID;
	}

	VoiceCallback(CircularData* _pOut, CircularData* pin, char* name) {
		assert(_pOut);
		this->pOut = _pOut;
		this->pin = pin;
		this->nextUniqueId = -1;
		this->thisName = name;
	}

	~VoiceCallback()
	{
	
	}

	void handleOutput() {
		int myId;
		pin->PopFront(myId);

		// sends the id then the input queue
		pOut->PushBack(myId);
		pOut->PushBack((int)pin);

		int duration;
		int pan;
		int handleId = myId;
		char* name = this->thisName;

		while (!pin->PopFront(duration)) std::this_thread::sleep_for(std::chrono::milliseconds(25));
		while (!pin->PopFront(pan)) std::this_thread::sleep_for(std::chrono::milliseconds(25));
		Trace::out("(duration %d)\t(handleID %d)\t(pan value %d)\t(name: %s)\n", duration, handleId, pan, name);
		return;
	}

	void STDMETHODCALLTYPE OnStreamEnd()
	{
		if (this->nextUniqueId == -1) {
			//std::thread::thread(&VoiceCallback::handleOutput, this);
			handleOutput();
			return;
		}
		Snd* nextSnd = VoiceManager::GetSndByUniqueId(nextUniqueId);
		Trace::out("Playing next wave: %s\n", nextName);
		nextSnd->Play();
	}

	void STDMETHODCALLTYPE OnVoiceProcessingPassEnd() { }
	void STDMETHODCALLTYPE OnVoiceProcessingPassStart(UINT32) {  }
	void STDMETHODCALLTYPE OnBufferEnd(void*) {  }
	void STDMETHODCALLTYPE OnBufferStart(void*) {  }
	void STDMETHODCALLTYPE OnLoopEnd(void*) {  }
	void STDMETHODCALLTYPE OnVoiceError(void*, HRESULT) {  }
};

#endif

// End of File 
