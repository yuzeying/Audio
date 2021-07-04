//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "CircularData.h"
#include "SndManager.h"
#include "Snd.h"


Snd::Snd(SndId snd, int volume, int pan, int pitch, int uniqueId)
	: sndID(snd), volume(volume), pan(pan), pitch(pitch), uniqueID(uniqueId)
{
	// generates -> new handle automatically 
}

Snd::Snd(SndId snd, int uniqueId)
: sndID(snd), volume(70), pan(50), pitch(0), uniqueID(uniqueId)
{
	// generates -> new handle automatically 
}

Handle::Status Snd::SetVolume(int newVolume)
{
	//HANDLE_CHECK_BEGIN

	CircularData* pOut = SndManager::GetG2A();
	assert(pOut);

	pOut->PushBack((int)SndId::AdjustID);
	pOut->PushBack(this->uniqueID);
	pOut->PushBack((int)SndId::AdjustVolume);
	pOut->PushBack(newVolume);

	this->volume = newVolume;

	//HANDLE_CHECK_END
	return Handle::Status::SUCCESS;
}

Handle::Status Snd::SetPan(int newPan)
{
	//HANDLE_CHECK_BEGIN

	CircularData* pOut = SndManager::GetG2A();
	assert(pOut);

	pOut->PushBack((int)SndId::AdjustID);
	pOut->PushBack(this->uniqueID);
	pOut->PushBack((int)SndId::AdjustPan);
	pOut->PushBack(newPan);

	this->pan = newPan;

	//HANDLE_CHECK_END
	return Handle::Status::SUCCESS;
}

Handle::Status Snd::SetPitch(int newPitch)

{
	//HANDLE_CHECK_BEGIN

	CircularData* pOut = SndManager::GetG2A();
	assert(pOut);

	pOut->PushBack((int)SndId::AdjustID);
	pOut->PushBack(this->uniqueID);
	pOut->PushBack((int)SndId::AdjustPitch);
	pOut->PushBack(newPitch);

	this->pitch = newPitch;

	//HANDLE_CHECK_END
	return Handle::Status::SUCCESS;
}

Snd::Snd(const Snd &tmp)
: sndID(tmp.sndID), volume(70), pan(50), pitch(0), uniqueID(uniqueIDGlobal++)
{
	// generates -> new handle automatically 
}

const Snd &Snd::operator=(const Snd &tmp)
{
	// QUESTION: copy handle?  --> I say NO, its the data not the handle
	//           to prevent anyone, I deleted the copy operator in handle
	//           this->handle = r.handle;   <----- I say its a no no

	this->sndID = tmp.sndID;

	return *this;
}

Snd::~Snd()
{
	// Kill the Handle
	// Gets automagically killed by the ~Handle()
}

void Snd::PrintTimeElapsed() {
	std::chrono::high_resolution_clock::time_point curTime = std::chrono::high_resolution_clock::now();
	Trace::out("Time Elapsed: %d\n", std::chrono::duration_cast<std::chrono::seconds>(curTime - this->startTime).count());
}

Handle::Status Snd::Play()
{
	//HANDLE_CHECK_BEGIN

	// Send a play command to the Audio thread
	CircularData *pOut = SndManager::GetG2A();
	assert(pOut);
	pOut->PushBack((int)SndId::AdjustID);
	pOut->PushBack(this->uniqueID);
	pOut->PushBack((int)SndId::Play);
	pOut->PushBack( (int)this->sndID );

	startTime = std::chrono::high_resolution_clock::now();

	//HANDLE_CHECK_END
	return Handle::Status::SUCCESS;
}

Handle::Status Snd::Stop()
{
	//HANDLE_CHECK_BEGIN

	CircularData* pOut = SndManager::GetG2A();
	assert(pOut);

	pOut->PushBack((int)SndId::AdjustID);
	pOut->PushBack(this->uniqueID);
	pOut->PushBack(-1);
	pOut->PushBack(-1);

	//HANDLE_CHECK_END
	return Handle::Status::SUCCESS;
}


// End of File 
