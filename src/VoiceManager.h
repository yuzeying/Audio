#pragma once

#include <xaudio2.h>
//#include "AudioThread.h"
#include "BufferManager.h"
#include "VoiceCallback.h"
#include "Snd.h"

class VoiceManager {
public:
	static IXAudio2* pX2;

	static int CreateVoice(int sndId) {
		VoiceBlock* newBlock = new VoiceBlock(sndId);
		newBlock = newBlock;
		return uniqueId++;
	}

	static int GetNextID() {
		return uniqueId;
	}

	static int CreateVoice(int sndId, IXAudio2VoiceCallback* pCallback) {
		VoiceBlock* newBlock = new VoiceBlock(sndId, pCallback);
		newBlock = newBlock;
		return uniqueId++;
	}

	static void Destroy() {
		VoiceBlock* cur = head, * temp = nullptr;
		while (cur != nullptr) {
			temp = cur->next;
			delete cur->sndObj;
			delete cur;
			cur = temp;
		}
		head = nullptr;
	}

	static IXAudio2SourceVoice* GetVoiceByUniqueId(int id) {
		VoiceBlock* cur = head;
		while (cur != nullptr) {
			if (cur->uniqueId == id) return cur->voice;
			cur = cur->next;
		}
		return nullptr;
	}

	static Snd* GetSndByUniqueId(int id) {
		VoiceBlock* cur = head;
		while (cur != nullptr) {
			if (cur->uniqueId == id) return cur->sndObj;
			cur = cur->next;
		}
		return nullptr;
	}

	class VoiceBlock {
	public:
		VoiceBlock(int sndId) {
			this->next = head;
			head = this;
			this->uniqueId = VoiceManager::uniqueId;

			const XAUDIO2_BUFFER* buf = BufferManager::GetBufferFromId(sndId);
			const WAVEFORMATEXTENSIBLE* wfx = BufferManager::GetWFXFromId(sndId);

			HRESULT hr = pX2->CreateSourceVoice(&voice, (WAVEFORMATEX*)wfx, 0, XAUDIO2_MAX_FREQ_RATIO);
			assert(hr == S_OK);
			hr = voice->SubmitSourceBuffer(buf);
			assert(hr == S_OK);
			voice->SetVolume(0.7f);

			sndObj = new Snd((SndId)sndId, this->uniqueId);
		}

		VoiceBlock(int sndId, IXAudio2VoiceCallback* pCallback) {
			this->next = head;
			head = this;
			this->uniqueId = VoiceManager::uniqueId;

			const XAUDIO2_BUFFER* buf = BufferManager::GetBufferFromId(sndId);
			const WAVEFORMATEXTENSIBLE* wfx = BufferManager::GetWFXFromId(sndId);

			HRESULT hr = pX2->CreateSourceVoice(&voice, (WAVEFORMATEX*)wfx, 0, XAUDIO2_MAX_FREQ_RATIO, pCallback);
			assert(hr == S_OK);
			hr = voice->SubmitSourceBuffer(buf);
			assert(hr == S_OK);
			voice->SetVolume(0.7f);

			sndObj = new Snd((SndId)sndId, this->uniqueId);
		}

		Snd* sndObj;
		VoiceBlock* next;
		int uniqueId;
		IXAudio2SourceVoice* voice;
	};
private:
	static VoiceBlock* head;
	static int uniqueId;
};

