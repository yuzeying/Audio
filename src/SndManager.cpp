//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "SndManager.h"
#include "Snd.h"

SndManager::SndManager()
{
	Snd::uniqueIDGlobal = 0;

	this->pA2G = new CircularData();
	assert(pA2G);

	this->pG2A = new CircularData();
	assert(pG2A);
}

SndManager::~SndManager()
{
	delete this->pA2G;
	delete this->pG2A;
}

CircularData* SndManager::GetA2G()
{
	SndManager* pMan = SndManager::privGetInstance();
	return pMan->pA2G;
}

CircularData* SndManager::GetG2A()
{
	SndManager* pMan = SndManager::privGetInstance();
	return pMan->pG2A;
}

void SndManager::SendData(int data)
{
	SndManager* pMan = SndManager::privGetInstance();

	Trace::out("Game->Audio: %x \n", data);
	pMan->pG2A->PushBack(data);
}

void SndManager::Update()
{
	SndManager* pMan = SndManager::privGetInstance();

	int val;

	if (pMan->pA2G->PopFront(val) == true)
	{
		if (val == 0x1234)
		{
			Trace::out("Audio->Game: %x snd ended callback\n", val);
		}
	}
}

void SndManager::Create()
{
	SndManager* pMan = SndManager::privGetInstance();
	assert(pMan);
}



SndManager* SndManager::privGetInstance()
{
	static SndManager sndMan;
	return &sndMan;
}


// End of File 
