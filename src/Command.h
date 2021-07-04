#pragma once
#include "VoiceManager.h"
#include <chrono>
#include <thread>

class Command {
public:

	class PriorityTable {
		class PriorityBlock {
		public:
			PriorityBlock() {
				this->sndId = -1;
				this->priority = -1;
			}
			PriorityBlock(int id, int priority) {
				this->sndId = id;
				this->priority = priority;
			}

			int sndId;
			int priority;
		};
	public:
		PriorityTable(int size) {
			this->size = size;
			this->blocks = (PriorityBlock**)calloc(size, sizeof(PriorityBlock*));
			for (int i = 0; i < size; i++) {
				this->blocks[i] = new PriorityBlock();
			}
		}
		
		~PriorityTable() {
			for (int i = 0; i < this->size; i++) {
				delete this->blocks[i];
				free(this->blocks);
			}
		}

		void print() {
			Trace::out("-------  Active Table ------------------\n");
			for (int i = 0; i < this->size; i++) {
				if (this->blocks[i]->sndId == -1) {
					Trace::out("0:\t0\t0 ms\n");
					continue;
				}
				Snd* sound = VoiceManager::GetSndByUniqueId(this->blocks[i]->sndId);
				Trace::out("%d:\t%d\t%d ms\n", this->blocks[i]->sndId, this->blocks[i]->priority, sound->GetTimeElapsed());
			}
			Trace::out("\n\n\n");
		}

		int playId(int sndId, int priority) {
			//checking if there are any empty positions
			for (int i = 0; i < this->size; i++) {
				PriorityBlock* cur = this->blocks[i];

				if (cur->priority == -1) {
					cur->priority = priority;
					cur->sndId = sndId;
					Snd* sound = VoiceManager::GetSndByUniqueId(sndId);
					sound->Play();
					return 0;
				}
			}

			//check for lowest priority
			PriorityBlock* lowest = this->blocks[0];
			int lowest_priority = lowest->priority;

			for (int i = 1; i < size; i++) {
				PriorityBlock* cur = this->blocks[i];
				if (cur->priority < lowest_priority) {
					lowest = cur;
					lowest_priority = cur->priority;
				}
			}

			if (lowest_priority < priority) {
				Snd* curSound2 = VoiceManager::GetSndByUniqueId(lowest->sndId);
				curSound2->Stop();

				lowest->priority = priority;
				lowest->sndId = sndId;
				Snd* sound = VoiceManager::GetSndByUniqueId(sndId);
				sound->Play();
				return 0;
			}

			//table full, find oldest block of same priority
			bool found = false;
			PriorityBlock* oldest = nullptr;
			Snd* curSound;// = VoiceManager::GetSndByUniqueId(oldest->sndId);
			std::chrono::high_resolution_clock::time_point oldestTime;

			for (int i = 1; i < this->size; i++) {
				PriorityBlock* cur = this->blocks[i];
				if (priority != cur->priority) continue;
				curSound = VoiceManager::GetSndByUniqueId(cur->sndId);
				std::chrono::high_resolution_clock::time_point curTime = curSound->GetStartTime();

				if (oldest == nullptr) {
					oldestTime = curTime;
					oldest = cur;
					found = true;
					continue;
				}

				if (curTime < oldestTime) {
					found = true;
					oldestTime = curTime;
					oldest = cur;
				}
			}

			if (found) {
				Snd* curSound2 = VoiceManager::GetSndByUniqueId(oldest->sndId);
				curSound2->Stop();

				oldest->priority = priority;
				oldest->sndId = sndId;
				Snd* sound = VoiceManager::GetSndByUniqueId(sndId);
				sound->Play();
				return 0;
			}

			return 1;
		}

		int size;
		PriorityBlock** blocks;
	};

	static PriorityTable* pt;

	enum Type {
		PLAY,
		PLAYPRIORITY,
		PRINTPRIORITY,
		STOP,
		SETPAN,
		PANLEFTTORIGHT,
		PANRIGHTTOLEFT,
		SETVOLUME,
		VOLUMELOWTOHIGH,
		VOLUMEHIGHTOLOW,
		PRINTTIME,
	};

	Command(int uniqueId, Type type) {
		if (pt == nullptr) { pt = new PriorityTable(6); }

		this->id = uniqueId;
		this->t = type;
		this->v = 0;
	}

	Command(int uniqueId, Type type, int value) {
		if (pt == nullptr) { pt = new PriorityTable(6); }

		this->id = uniqueId;
		this->t = type;
		this->v = value;
	}



	void Execute2() {
		using namespace std::chrono_literals;
		Snd* sound = VoiceManager::GetSndByUniqueId(this->id);
		if (sound == nullptr) return;

		switch (t){
		case PRINTPRIORITY:
			pt->print();
			break;
		case PLAYPRIORITY:
			pt->playId(this->id, this->v);
			break;
		case PLAY:
			sound->Play();
			break;
		case STOP:
			sound->Stop();
			break;
		case SETPAN:
			sound->SetPan(this->v);
			break;
		case PANLEFTTORIGHT:
			sound->SetPan(0);
			sound->Play();
			for (int i = 0; i < 100; i++) {
				sound->SetPan(i);
				std::this_thread::sleep_for(10ms);
			}
			break;
		case PANRIGHTTOLEFT:
			sound->SetPan(100);
			sound->Play();
			for (int i = 0; i < 100; i++) {
				sound->SetPan(100 - i);
				std::this_thread::sleep_for(10ms);
			}
			break;
		case SETVOLUME:
			sound->SetVolume(this->v);
			break;
		case VOLUMEHIGHTOLOW:
			sound->SetVolume(100);
			sound->Play();
			for (int i = 0; i < 100; i++) {
				//if (!(i % 20)) {
					int curVolume = sound->GetVolume();
					sound->SetVolume(--curVolume);
				//}
				std::this_thread::sleep_for(10ms);
			}
			break;
		case VOLUMELOWTOHIGH:
			sound->SetVolume(0);
			sound->Play();
			for (int i = 0; i < 100; i++) {
				//if (!(i % 20)) {
					int curVolume = sound->GetVolume();
					sound->SetVolume(++curVolume);
				//} 
				std::this_thread::sleep_for(10ms);
			}
			break;
		case PRINTTIME:
			sound->PrintTimeElapsed();
			break;
		}
	}

	void Execute() {
		std::thread tt(&Command::Execute2, this);
		tt.detach();
	}
private:
	int id;
	Type t;
	int v;
};

Command::PriorityTable *Command::pt = nullptr;