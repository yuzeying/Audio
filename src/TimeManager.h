#pragma once
#pragma once
#include <xaudio2.h>
#include "Command.h"

class TimeManager {
public:
	static void Destroy() {
		TimeBlock* cur = head, * temp = nullptr;
		while (cur != nullptr) {
			temp = cur->next;
			delete cur->command;
			delete cur;
			cur = temp;
		}
		head = nullptr;
	}

	static void AddNextEvent(long msSinceStart, Command *c) {
		new TimeBlock(msSinceStart, c);
	}

	static CircularData* q;
	static CircularData* getCircular() {
		if (q == nullptr) {
			q = new CircularData();
		}
		return q;
	}

	static void Run() {
		using namespace std::chrono_literals;
		std::chrono::high_resolution_clock::time_point startTime = std::chrono::high_resolution_clock::now();
	
		TimeBlock* cur = head;
		while (cur) {
			if (q != nullptr) {
				int id;
				CircularData* circ = nullptr;
				if (q->PopFront(id)) {
					int ptr_circ;
					q->PopFront(ptr_circ);
					circ = (CircularData*)ptr_circ;

					Snd* curSound = VoiceManager::GetSndByUniqueId(id);

					circ->PushBack(curSound->GetTimeElapsed());
					circ->PushBack(curSound->GetPan());

					continue;
				}
			}

			std::chrono::high_resolution_clock::time_point curTime = std::chrono::high_resolution_clock::now();
			long timePassed = (long)std::chrono::duration_cast<std::chrono::milliseconds>(curTime - startTime).count();
			long timeToPass = cur->ms - timePassed;
			if (timeToPass > 0) {
				//Trace::out("Sleeping for %d\n", timeToPass);
				std::this_thread::sleep_for(std::chrono::milliseconds(timeToPass));
			}
			cur->command->Execute();
			cur = cur->next;
		}




		int loopCount = 0;
		while (q != nullptr && loopCount < 2000) {
			int id;
			CircularData* circ = nullptr;
			if (q->PopFront(id)) {
				int ptr_circ;
				q->PopFront(ptr_circ);
				circ = (CircularData*)ptr_circ;

				Snd* curSound = VoiceManager::GetSndByUniqueId(id);

				circ->PushBack(curSound->GetTimeElapsed());
				circ->PushBack(curSound->GetPan());

				continue;
			}
			loopCount++;
			std::this_thread::sleep_for(std::chrono::milliseconds(10));

		}

		std::this_thread::sleep_for(5s);
	}

	class TimeBlock {
	public:
		TimeBlock(long msSinceStart, Command *c) {
			TimeBlock* cur = head;
			if (head == nullptr) {
				head = this;
			}
			else {
				while (cur->next) {
					cur = cur->next;
				}
				cur->next = this;
			}
			this->next = nullptr;
			this->ms = msSinceStart;
			this->command = c;
		}
		TimeBlock* next;
		Command* command;
		long ms;
	};
private:


	static TimeBlock* head;
};

TimeManager::TimeBlock* TimeManager::head = nullptr;
CircularData* TimeManager::q = nullptr;