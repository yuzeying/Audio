//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GAME_APP_H
#define GAME_APP_H

class GameApp
{
public:
	static void LoadDemo(int width, int height);
	static void UpdateDemo();
	static void DrawDemo();
	
	
	GameApp();
	~GameApp();

private:
	static GameApp *privGameApp();

	Camera* pCam3D;
	GraphicsObject_TextureLight* pSpaceFrigateLight;
	Texture* pSpaceFrigateTexture;
	Model* pModelSpaceFrigate;
	ShaderObject* pShaderObject_textureLight;
};

#endif

// End of File 

