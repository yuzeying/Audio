#ifndef PLAYLIST_H
#define PLAYLIST_H

#include "XAudio2Wrapper.h"
#include "CircularData.h"
#include "HandleMan.h"
#include "SndCallRegistry.h"

class Playlist
{
public:
	// defaults
	Playlist() {

	}
	~Playlist() {

	}


	Handle::Status Play() {


		return Handle::Status::SUCCESS;
	}
	Handle::Status Stop() {
		return Handle::Status::SUCCESS;
	}

private:
	SndId	sndID;

	// Here's the magic
	HANDLE_VARABLE


	class PlaylistVoiceCallback : public IXAudio2VoiceCallback
	{
	public:
		int current_id = 0;
		bool   finished = false;
		CircularData* pOut;

		PlaylistVoiceCallback(CircularData* _pOut)
		{
			assert(_pOut);
			this->pOut = _pOut;
		}

		PlaylistVoiceCallback(CircularData* _pOut, int start_id)
		{
			assert(_pOut);
			current_id = start_id;
			this->pOut = _pOut;
		}

		~PlaylistVoiceCallback()
		{

		}

		void STDMETHODCALLTYPE OnStreamEnd()
		{
			if (current_id == 0) return; // dont stich if we dont want to
			current_id++;

			if (current_id == (int)SndId::Exit + 1) {
				Trace::out("Quitting sound playlist\n");
				pOut->PushBack(0xABABABAB);
				finished = true;
			}
			else {
				Trace::out("Plaing new sound of id %d\n", current_id);
				pOut->PushBack(current_id);
			}
		}

		void STDMETHODCALLTYPE OnVoiceProcessingPassEnd() { }
		void STDMETHODCALLTYPE OnVoiceProcessingPassStart(UINT32) {  }
		void STDMETHODCALLTYPE OnBufferEnd(void*) {  }
		void STDMETHODCALLTYPE OnBufferStart(void*) {  }
		void STDMETHODCALLTYPE OnLoopEnd(void*) {  }
		void STDMETHODCALLTYPE OnVoiceError(void*, HRESULT) {  }
	};
};

#endif