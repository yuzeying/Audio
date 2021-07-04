//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

// Game App stuff
#include "AzulCore.h"
#include "Game.h"
#include "GameApp.h"
#include "InputTest.h"
#include "XAudio2Wrapper.h"

#include "SndManager.h"
#include "Snd.h"

// Thread stuff
#include <thread>
#include <chrono>
#include "ThreadHelper.h"
#include "AudioThread.h"
#include "TestThread.h"
#include "CircularData.h"
#include "BufferManager.h"
#include "VoiceCallback.h"
#include "VoiceManager.h"
#include "TimeManager.h"

int Snd::uniqueIDGlobal = 0;
IXAudio2* VoiceManager::pX2 = nullptr;
VoiceManager::VoiceBlock* VoiceManager::head = nullptr;
int VoiceManager::uniqueId = 0;

//-----------------------------------------------------------------------------
// Game::Initialize()
//		Allows the engine to perform any initialization it needs to before 
//      starting to run.  This is where it can query for any required services 
//      and load any non-graphic related content. 
//-----------------------------------------------------------------------------
void Game::Initialize()
{ 
	// Game Window Device setup
	this->setWindowName("Audio Engine");
	this->setWidthHeight(400, 300);
	this->SetClearColor(0.74f, 0.74f, 0.86f, 1.0f);
}

//-----------------------------------------------------------------------------
// Game::LoadContent()
//		Allows you to load all content needed for your engine,
//	    such as objects, graphics, etc.
//-----------------------------------------------------------------------------
void Game::LoadContent()
{	
	// Load up the demo application
	GameApp::LoadDemo(this->getWidth(), this->getHeight());

	// Name the current main thread
	ThreadHelper::SetCurrentThreadName("--- Game ---");
	ThreadHelper::DebugBegin(0);

	//---------------------------------------------------------------------------------------------------------
	// Launch a Thread
	//---------------------------------------------------------------------------------------------------------
	InitializeXAudio2();
	SndManager::Create();

	BufferManager::LoadFileIntoBuffer("../../MS2_AudioFiles/Intro_mono.wav", (int)SndId::Intro);
	BufferManager::LoadFileIntoBuffer("../../MS2_AudioFiles/A_mono.wav", (int)SndId::A);
	BufferManager::LoadFileIntoBuffer("../../MS2_AudioFiles/AtoB_mono.wav", (int)SndId::AtoB);
	BufferManager::LoadFileIntoBuffer("../../MS2_AudioFiles/B_mono.wav", (int)SndId::B);
	BufferManager::LoadFileIntoBuffer("../../MS2_AudioFiles/BtoC_mono.wav", (int)SndId::BtoC);
	BufferManager::LoadFileIntoBuffer("../../MS2_AudioFiles/C_mono.wav", (int)SndId::C);
	BufferManager::LoadFileIntoBuffer("../../MS2_AudioFiles/CtoA_mono.wav", (int)SndId::CtoA);
	BufferManager::LoadFileIntoBuffer("../../MS2_AudioFiles/End_mono.wav", (int)SndId::Exit);

	BufferManager::LoadFileIntoBuffer("../../MS2_AudioFiles/Bassoon_mono.wav", (int)SndId::Bassoon);
	BufferManager::LoadFileIntoBuffer("../../MS2_AudioFiles/Calliope_mono.wav", (int)SndId::Calliope);
	BufferManager::LoadFileIntoBuffer("../../MS2_AudioFiles/Fiddle_mono.wav", (int)SndId::Fiddle);
	BufferManager::LoadFileIntoBuffer("../../MS2_AudioFiles/Oboe_mono.wav", (int)SndId::Oboe);
	BufferManager::LoadFileIntoBuffer("../../MS2_AudioFiles/SongA.wav", (int)SndId::SongA);
	BufferManager::LoadFileIntoBuffer("../../MS2_AudioFiles/SongB.wav", (int)SndId::SongB);


	BufferManager::LoadFileIntoBuffer("../../MS2_AudioFiles/Dial_mono.wav", (int)SndId::Dial);
	BufferManager::LoadFileIntoBuffer("../../MS2_AudioFiles/MoonPatrol_mono.wav", (int)SndId::Moon);
	BufferManager::LoadFileIntoBuffer("../../MS2_AudioFiles/Sequence1_mono.wav", (int)SndId::Sequence);
	BufferManager::LoadFileIntoBuffer("../../MS2_AudioFiles/Donkey_mono.wav", (int)SndId::Donkey);

	// Create an Audio thread...
	std::thread  audioThread(AudioMain, SndManager::GetG2A(), SndManager::GetA2G());
	ThreadHelper::SetThreadName(audioThread, "--- Audio ---");
	audioThread.detach();
}

bool played1 = false;
void Demo1() {
	if (played1) return;
	played1 = true;

	int fiddle1 = VoiceManager::CreateVoice((int)SndId::Fiddle);
	int fiddle2 = VoiceManager::CreateVoice((int)SndId::Fiddle);
	int fiddle3 = VoiceManager::CreateVoice((int)SndId::Fiddle);
	int bassoon1 = VoiceManager::CreateVoice((int)SndId::Bassoon);
	int bassoon2 = VoiceManager::CreateVoice((int)SndId::Bassoon);
	int oboe1 = VoiceManager::CreateVoice((int)SndId::Oboe);
	int oboe2 = VoiceManager::CreateVoice((int)SndId::Oboe);
	int songA = VoiceManager::CreateVoice((int)SndId::SongA);
	int songB = VoiceManager::CreateVoice((int)SndId::SongB);
	int sndAid = VoiceManager::CreateVoice((int)SndId::Bassoon);
	int sndBid = VoiceManager::CreateVoice((int)SndId::Bassoon);
	int sndCid = VoiceManager::CreateVoice((int)SndId::Bassoon);
	int sndDid = VoiceManager::CreateVoice((int)SndId::Bassoon);
	
	// Part B
	TimeManager::AddNextEvent(0, new Command(fiddle1, Command::Type::PLAY));
	TimeManager::AddNextEvent(3000, new Command(fiddle2, Command::Type::SETPAN, 0));
	TimeManager::AddNextEvent(3000, new Command(fiddle2, Command::Type::PLAY));
	TimeManager::AddNextEvent(3000, new Command(fiddle3, Command::Type::SETPAN, 100));
	TimeManager::AddNextEvent(6000, new Command(fiddle3, Command::Type::PLAY));

	// Part C
	TimeManager::AddNextEvent(10000, new Command(bassoon1, Command::Type::PANLEFTTORIGHT));
	TimeManager::AddNextEvent(15000, new Command(bassoon2, Command::Type::PANRIGHTTOLEFT));

	// Part D
	TimeManager::AddNextEvent(20000, new Command(oboe1, Command::Type::VOLUMELOWTOHIGH));
	TimeManager::AddNextEvent(25000, new Command(oboe2, Command::Type::VOLUMEHIGHTOLOW));

	// Part E
	TimeManager::AddNextEvent(30000, new Command(songA, Command::Type::SETPAN, 0));
	TimeManager::AddNextEvent(30000, new Command(songA, Command::Type::PLAY));
	TimeManager::AddNextEvent(30000, new Command(songB, Command::Type::SETPAN, 100));
	TimeManager::AddNextEvent(30000, new Command(songB, Command::Type::PLAY));
	TimeManager::AddNextEvent(35000, new Command(songA, Command::Type::PRINTTIME));
	TimeManager::AddNextEvent(38000, new Command(songB, Command::Type::PRINTTIME));
	TimeManager::AddNextEvent(60000, new Command(songA, Command::Type::PRINTTIME));
	TimeManager::AddNextEvent(60000, new Command(songA, Command::Type::STOP));
	TimeManager::AddNextEvent(72000, new Command(songB, Command::Type::PRINTTIME));
	TimeManager::AddNextEvent(72000, new Command(songB, Command::Type::STOP));

	// Part F
	TimeManager::AddNextEvent(80000, new Command(sndAid, Command::Type::SETVOLUME, 40));
	TimeManager::AddNextEvent(80000, new Command(sndAid, Command::Type::PLAY));
	TimeManager::AddNextEvent(80500, new Command(sndBid, Command::Type::SETVOLUME, 40));
	TimeManager::AddNextEvent(80500, new Command(sndBid, Command::Type::PLAY));
	TimeManager::AddNextEvent(81000, new Command(sndCid, Command::Type::SETVOLUME, 40));
	TimeManager::AddNextEvent(81000, new Command(sndCid, Command::Type::PLAY));
	TimeManager::AddNextEvent(81500, new Command(sndDid, Command::Type::SETVOLUME, 40));
	TimeManager::AddNextEvent(81500, new Command(sndDid, Command::Type::PLAY));
	TimeManager::AddNextEvent(81500, new Command(sndAid, Command::Type::STOP));
	TimeManager::AddNextEvent(81500, new Command(sndBid, Command::Type::STOP));
	TimeManager::AddNextEvent(81500, new Command(sndCid, Command::Type::STOP));

	TimeManager::Run();
	played1 = false;
	TimeManager::Destroy();
}

VoiceCallback* v1 = nullptr;
VoiceCallback* v2 = nullptr;
VoiceCallback* v3 = nullptr;
VoiceCallback* v4 = nullptr;
VoiceCallback* v5 = nullptr;
VoiceCallback* v6 = nullptr;
VoiceCallback* v7 = nullptr;

bool played2 = false;
void Demo2() {
	if (played2) return;
	played2 = true;
	int nextId = VoiceManager::GetNextID() + 1;

	v1 = new VoiceCallback(SndManager::GetG2A(), "A_mono", nextId++);
	v2 = new VoiceCallback(SndManager::GetG2A(), "AtoB_mono", nextId++);
	v3 = new VoiceCallback(SndManager::GetG2A(), "B_mono", nextId++);
	v4 = new VoiceCallback(SndManager::GetG2A(), "BtoC_mono", nextId++);
	v5 = new VoiceCallback(SndManager::GetG2A(), "C_mono", nextId++);
	v6 = new VoiceCallback(SndManager::GetG2A(), "CtoA_mono", nextId++);
	v7 = new VoiceCallback(SndManager::GetG2A(), "End_mono", nextId++);

	int stichedId = VoiceManager::CreateVoice((int)SndId::Intro, v1);
	VoiceManager::CreateVoice((int)SndId::A, v2);
	VoiceManager::CreateVoice((int)SndId::AtoB, v3);
	VoiceManager::CreateVoice((int)SndId::B, v4);
	VoiceManager::CreateVoice((int)SndId::BtoC, v5);
	VoiceManager::CreateVoice((int)SndId::C, v6);
	VoiceManager::CreateVoice((int)SndId::CtoA, v7);
	VoiceManager::CreateVoice((int)SndId::Exit);

	Snd* stichedSnd = VoiceManager::GetSndByUniqueId(stichedId);
	stichedSnd->Play();

	std::this_thread::sleep_for(std::chrono::milliseconds(250));
	played2 = false;
}

bool played3 = false;
void Demo3() {
	if (played3) return;
	played3 = true;

	int sndid_A = VoiceManager::CreateVoice((int)SndId::Bassoon);
	int sndid_B = VoiceManager::CreateVoice((int)SndId::Bassoon);
	int sndid_C = VoiceManager::CreateVoice((int)SndId::Bassoon);
	int sndid_D = VoiceManager::CreateVoice((int)SndId::Bassoon);
	int sndid_E = VoiceManager::CreateVoice((int)SndId::Bassoon);
	int sndid_F = VoiceManager::CreateVoice((int)SndId::Bassoon);
	int sndid_G = VoiceManager::CreateVoice((int)SndId::Bassoon);
	int sndid_H = VoiceManager::CreateVoice((int)SndId::Bassoon);
	int sndid_I = VoiceManager::CreateVoice((int)SndId::Bassoon);
	int sndid_J = VoiceManager::CreateVoice((int)SndId::Bassoon);
	int sndid_K = VoiceManager::CreateVoice((int)SndId::Bassoon);
	TimeManager::AddNextEvent(0, new Command(sndid_A, Command::Type::SETVOLUME, 10));
	TimeManager::AddNextEvent(0, new Command(sndid_B, Command::Type::SETVOLUME, 10));
	TimeManager::AddNextEvent(0, new Command(sndid_C, Command::Type::SETVOLUME, 10));
	TimeManager::AddNextEvent(0, new Command(sndid_D, Command::Type::SETVOLUME, 10));
	TimeManager::AddNextEvent(0, new Command(sndid_E, Command::Type::SETVOLUME, 10));
	TimeManager::AddNextEvent(0, new Command(sndid_F, Command::Type::SETVOLUME, 10));
	TimeManager::AddNextEvent(0, new Command(sndid_G, Command::Type::SETVOLUME, 10));
	TimeManager::AddNextEvent(0, new Command(sndid_H, Command::Type::SETVOLUME, 10));
	TimeManager::AddNextEvent(0, new Command(sndid_I, Command::Type::SETVOLUME, 10));
	TimeManager::AddNextEvent(0, new Command(sndid_J, Command::Type::SETVOLUME, 10));
	TimeManager::AddNextEvent(0, new Command(sndid_K, Command::Type::SETVOLUME, 10));

	//0 seconds
	TimeManager::AddNextEvent(0, new Command(sndid_A, Command::Type::PLAYPRIORITY, 10));
	TimeManager::AddNextEvent(0, new Command(sndid_B, Command::Type::PLAYPRIORITY, 50));
	TimeManager::AddNextEvent(0, new Command(sndid_C, Command::Type::PLAYPRIORITY, 150));
	TimeManager::AddNextEvent(0, new Command(sndid_A, Command::Type::PRINTPRIORITY));


	//1 seconds
	TimeManager::AddNextEvent(1000, new Command(sndid_D, Command::Type::PLAYPRIORITY, 50));
	TimeManager::AddNextEvent(1000, new Command(sndid_A, Command::Type::PRINTPRIORITY));


	//2 seconds
	TimeManager::AddNextEvent(2000, new Command(sndid_E, Command::Type::PLAYPRIORITY, 75));
	TimeManager::AddNextEvent(2000, new Command(sndid_A, Command::Type::PRINTPRIORITY));


	//3 seconds
	TimeManager::AddNextEvent(3000, new Command(sndid_F, Command::Type::PLAYPRIORITY, 100));
	TimeManager::AddNextEvent(3000, new Command(sndid_A, Command::Type::PRINTPRIORITY));


	//4 seconds
	TimeManager::AddNextEvent(4000, new Command(sndid_G, Command::Type::PLAYPRIORITY, 150));
	TimeManager::AddNextEvent(4000, new Command(sndid_A, Command::Type::PRINTPRIORITY));


	//5 seconds
	TimeManager::AddNextEvent(5000, new Command(sndid_H, Command::Type::PLAYPRIORITY, 75));
	TimeManager::AddNextEvent(5000, new Command(sndid_A, Command::Type::PRINTPRIORITY));


	//6 seconds
	TimeManager::AddNextEvent(6000, new Command(sndid_I, Command::Type::PLAYPRIORITY, 75));
	TimeManager::AddNextEvent(6000, new Command(sndid_A, Command::Type::PRINTPRIORITY));


	//7 seconds
	TimeManager::AddNextEvent(7000, new Command(sndid_J, Command::Type::PLAYPRIORITY, 75));
	TimeManager::AddNextEvent(7000, new Command(sndid_A, Command::Type::PRINTPRIORITY));


	//8 seconds
	TimeManager::AddNextEvent(8000, new Command(sndid_K, Command::Type::PLAYPRIORITY, 150));
	TimeManager::AddNextEvent(8000, new Command(sndid_A, Command::Type::PRINTPRIORITY));


	//13 seconds
	TimeManager::AddNextEvent(13000, new Command(sndid_K, Command::Type::PRINTTIME));

	TimeManager::Run();
	played3 = false;
}


VoiceCallback* GameCallback_A = nullptr;
VoiceCallback* GameCallback_B = nullptr;
VoiceCallback* GameCallback_C = nullptr;
VoiceCallback* GameCallback_D = nullptr;
CircularData* send1 = nullptr; CircularData* send2 = nullptr; CircularData* send3 = nullptr; CircularData* send4 = nullptr;

bool played4 = false;
void Demo4() {
	if (played4 || send1 || send2 || send3 || send4) return;
	played4 = true;
	TimeManager::Destroy();

	CircularData* d = TimeManager::getCircular();
	send1 = new CircularData(); send2 = new CircularData(); send3 = new CircularData(); send4 = new CircularData();

	GameCallback_A = new VoiceCallback(d, send1, "Dial");
	GameCallback_B = new VoiceCallback(d, send2, "Moon");
	GameCallback_C = new VoiceCallback(d, send3, "Sequence");
	GameCallback_D = new VoiceCallback(d, send4, "Donkey");

	int sndid_A = VoiceManager::CreateVoice((int)SndId::Dial, GameCallback_A);
	int sndid_B = VoiceManager::CreateVoice((int)SndId::Moon, GameCallback_B);
	int sndid_C = VoiceManager::CreateVoice((int)SndId::Sequence, GameCallback_C);
	int sndid_D = VoiceManager::CreateVoice((int)SndId::Donkey, GameCallback_D);

	send1->PushBack(sndid_A); send2->PushBack(sndid_B); send3->PushBack(sndid_C); send4->PushBack(sndid_D);

	// setting all of the pan values to 0 before running
	TimeManager::AddNextEvent(0, new Command(sndid_A, Command::Type::SETPAN, 0));
	TimeManager::AddNextEvent(0, new Command(sndid_B, Command::Type::SETPAN, 0));
	TimeManager::AddNextEvent(0, new Command(sndid_C, Command::Type::SETPAN, 0));
	TimeManager::AddNextEvent(0, new Command(sndid_D, Command::Type::SETPAN, 0));

	// time 0
	TimeManager::AddNextEvent(0, new Command(sndid_A, Command::Type::PLAY));
	TimeManager::AddNextEvent(0, new Command(sndid_B, Command::Type::PLAY));
	TimeManager::AddNextEvent(0, new Command(sndid_C, Command::Type::PLAY));

	// time 3.5
	TimeManager::AddNextEvent(3500, new Command(sndid_D, Command::Type::PLAY));
	//TimeManager::AddNextEvent(15000, new Command(sndid_D, Command::Type::PRINTTIME));

	TimeManager::Run();
	//TimeManager::Destroy();
	//delete send1; delete send2; delete send3; delete send4;
	played4 = false;
}

bool played5 = false;
void Demo5() {
	if (played5) return;
	played5 = true;


	TimeManager::Run();
	TimeManager::Destroy();
	played5 = false;
}
//-----------------------------------------------------------------------------
// Game::Update()
//      Called once per frame, update data, tranformations, etc
//      Use this function to control process order
//      Input, AI, Physics, Animation, and Graphics
//-----------------------------------------------------------------------------
void Game::Update()
{
	// Update the demo application
	GameApp::UpdateDemo();
	SndManager::Update();

	// Read Quit Key
	Keyboard* key = Keyboard::GetInstance();
	static bool QuitFlag = false;
	if (key->GetKeyState(AZUL_KEY::KEY_Q) && !QuitFlag)
	{
		// Quit - 0xABABABAB
		SndManager::SendData(0xABABABAB);
		QuitFlag = true;
	}

	if (key->GetKeyState(AZUL_KEY::KEY_1)) {
		Trace::out("Starting the audio playlist demo1!");

		std::thread audioPan(Demo1);
		audioPan.detach();
	}

	if (key->GetKeyState(AZUL_KEY::KEY_2)) {
		Trace::out("Starting the audio playlist demo2!");

		std::thread audioPan(Demo2);
		audioPan.detach();
	}

	if (key->GetKeyState(AZUL_KEY::KEY_3)) {
		Trace::out("Starting the audio playlist demo3!");

		std::thread audioPan(Demo3);
		audioPan.detach();
	}

	if (key->GetKeyState(AZUL_KEY::KEY_4)) {
		Trace::out("Starting the audio playlist demo4!");

		std::thread audioPan(Demo4);
		audioPan.detach();
	}
}

//-----------------------------------------------------------------------------
// Game::Draw()
//		This function is called once per frame
//	    Use this for draw graphics to the screen.
//      Only do rendering here
//-----------------------------------------------------------------------------
void Game::Draw()
{
	// Draw the demo application
	GameApp::DrawDemo();

}



//-----------------------------------------------------------------------------
// Game::UnLoadContent()
//       unload content (resources loaded above)
//       unload all content that was loaded before the Engine Loop started
//-----------------------------------------------------------------------------
void Game::UnLoadContent()
{
	ThreadHelper::DebugEnd(0);
	BufferManager::Destroy();
	VoiceManager::Destroy();
	TimeManager::Destroy();
	if (v1) delete v1;
	if (v2) delete v2;
	if (v3) delete v3;
	if (v4) delete v4;
	if (v5) delete v5;
	if (v6) delete v6;
	if (v7) delete v7;
	if (GameCallback_A) delete GameCallback_A;
	if (GameCallback_B) delete GameCallback_B;
	if (GameCallback_C) delete GameCallback_C;
	if (GameCallback_D) delete GameCallback_D;
	if (send1) delete send1;
	if (send2) delete send1;
	if (send3) delete send1;
	if (send4) delete send1;
	if (TimeManager::q) delete TimeManager::q;
}

// End of File 

