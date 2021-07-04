//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "ThreadHelper.h"
#include "TestThread.h"

void Test_Main(CircularData *pIn_G2T, CircularData *pOut_T2G)
{
	ThreadHelper::DebugBegin(1);
	Trace::out("\n");

	int val;

	while (1)
	{
		if (pIn_G2T->PopFront(val) == true)
		{
			// Quit
			if (val == 0xFFFF)
			{
				break;
			}

			Trace::out("<--- Test(): %x \n", val);
			Trace::out("\n");

			// echo back the complement
			unsigned int data = ~val & 0xff;

			Trace::out("Test() ---> send: %x \n", data);
			bool status = pOut_T2G->PushBack(data);
			assert(status);
		}
	}

	ThreadHelper::DebugEnd(1);
	Trace::out("\n");

}

// End of File 
