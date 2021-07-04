//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "AzulCore.h"
#include "Game.h"
#include "GameApp.h"

GameApp * GameApp::privGameApp()
{
	static GameApp gameApp;
	return &gameApp;
}
   
GameApp::GameApp()
{     
	this->pCam3D = nullptr;
	this->pSpaceFrigateLight = nullptr;
	this->pSpaceFrigateTexture = nullptr;
	this->pModelSpaceFrigate = nullptr;
	this->pShaderObject_textureLight = nullptr;
}

void GameApp::LoadDemo( int width, int height  )
{
	GameApp* pGameApp = GameApp::privGameApp();

	//---------------------------------------------------------------------------------------------------------
	// Setup the current 3D perspective Camera
	//---------------------------------------------------------------------------------------------------------

	pGameApp->pCam3D = new Camera(Camera::Type::PERSPECTIVE_3D);
	assert(pGameApp->pCam3D);

	pGameApp->pCam3D->setViewport(0, 0, width, height);
	pGameApp->pCam3D->setPerspective(35.0f, float(width) / float(height), 1.0f, 500.0f);

	// Orient Camera
	Vect up3DCam(0.0f, 1.0f, 0.0f);
	Vect pos3DCam(0.0f, 80.0f, 120.0f);
	Vect lookAt3DCam(0.0f, 0.0f, -1.0f);
	pGameApp->pCam3D->setOrientAndPosition(up3DCam, lookAt3DCam, pos3DCam);

	//---------------------------------------------------------------------------------------------------------
	// Load the Models
	//---------------------------------------------------------------------------------------------------------

	pGameApp->pModelSpaceFrigate = new Model("space_frigate.azul");

	//---------------------------------------------------------------------------------------------------------
	// Load the Shaders
	//---------------------------------------------------------------------------------------------------------

	pGameApp->pShaderObject_textureLight = new ShaderObject("textureLightRender");

	//---------------------------------------------------------------------------------------------------------
	// Load the Textures
	//---------------------------------------------------------------------------------------------------------

	pGameApp->pSpaceFrigateTexture = new Texture("space_frigate.tga");

	//---------------------------------------------------------------------------------------------------------
	// Graphics Objects
	//---------------------------------------------------------------------------------------------------------

	// Light
	Vect LightColor(2.0f, 2.0f, 2.0f, 1.0f);
	Vect LightPos(1.0f, 3.0f, 1.0f, 1.0f);
	pGameApp->pSpaceFrigateLight = new GraphicsObject_TextureLight(pGameApp->pModelSpaceFrigate, pGameApp->pShaderObject_textureLight, pGameApp->pSpaceFrigateTexture, LightColor, LightPos);
}



void GameApp::UpdateDemo()
{
	GameApp* pGameApp = GameApp::privGameApp();
	
	// Update cameras - make sure everything is consistent
	pGameApp->pCam3D->updateCamera();

	// Add your update below this line: ----------------------------

	// update objects
	Matrix world;

	Matrix RotY;
	Matrix RotZ;
	Matrix Scale;
	Matrix Trans;

	static float angle = 0.0f;
	angle += 0.04f;

	// Space Frigate Texture: 
	RotY.set(ROT_Y, 1.3f * angle);
	Trans.set(TRANS, 0.0f, 0.0f, 0.0f);
	world = RotY * Trans;
	pGameApp->pSpaceFrigateLight->SetWorld(world);
}

void GameApp::DrawDemo()
{
	GameApp* pGameApp = GameApp::privGameApp();

	// draw all objects
	pGameApp->pSpaceFrigateLight->Render(pGameApp->pCam3D);

}

GameApp::~GameApp()
{
	delete pCam3D;
	delete pSpaceFrigateLight;
	delete pSpaceFrigateTexture;
	delete pModelSpaceFrigate;
	delete pShaderObject_textureLight;
}

// End of File 
 