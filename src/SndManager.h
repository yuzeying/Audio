//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef SND_MANAGER_H
#define SND_MANAGER_H

#include "CircularData.h"

class SndManager
{
public:
	// Defaults
	SndManager(const SndManager&) = delete;
	const SndManager& operator = (const SndManager&) = delete;
	~SndManager();

	// Public methods:
	static void Create();
	static void Update();
	static void SendData(int val);

	static CircularData* GetA2G();
	static CircularData* GetG2A();

private:
	static SndManager* privGetInstance();

	SndManager();


	CircularData* pA2G;  // Game to Audio
	CircularData* pG2A; // Audio to Game
};

#endif


// End of File 
