//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef SND_H
#define SND_H

#include "HandleMan.h"
#include "SndCallRegistry.h"
#include <chrono>

class Snd
{
public:
	static int uniqueIDGlobal;
	// defaults
		Snd() = delete;
		Snd(SndId snd, int volume, int pan, int pitch, int uniqueId);
		Snd(const Snd &);
		const Snd &operator=(const Snd &);
		~Snd();

	// public
	Snd(SndId snd, int uniqueId);

	int GetVolume() { return volume; }
	std::chrono::high_resolution_clock::time_point GetStartTime() { return startTime; }
	void PrintTimeElapsed();



	int GetTimeElapsed() {
		const std::chrono::high_resolution_clock::time_point curTime2 = std::chrono::high_resolution_clock::now();
		return (int)std::chrono::duration_cast<std::chrono::seconds>(curTime2 - this->startTime).count();
	}

	int GetPan() {
		return this->pan;
	}

	Handle::Status SetVolume(int newVolume);
	Handle::Status SetPan(int newPan);
	Handle::Status SetPitch(int newPitch);
	Handle::Status Play();
	Handle::Status Stop();

private:
	int uniqueID;
	SndId	sndID;
	int volume;
	int pan;
	int pitch;

	std::chrono::high_resolution_clock::time_point startTime;
};
#endif


// End of File 
