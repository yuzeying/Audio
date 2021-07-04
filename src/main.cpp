//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "Game.h"

int CALLBACK WinMain(HINSTANCE , HINSTANCE ,  LPSTR , int)                  
{          
	// create Game
	Game *pGame = new Game();    
	assert(pGame);

	// start the game
	pGame->run();                                  

	delete pGame;

	return 0;                                       
}

 // End of File 
 