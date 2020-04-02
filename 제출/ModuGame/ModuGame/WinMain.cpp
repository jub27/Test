#include <Windows.h>
#include <time.h>
#include "EngineMain.h"
#include "SceneManager.h"
#include "TitleScene.h"
#include "Game1.h"
#include "Game2.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	//엔진 객체를 생성하고 타이틀과 윈도우 사이즈를 넣어준다.
	srand(time(NULL));
	JEngine::EngineMain engine("ModuGame", 414, 641);
	//씬을 등록한다. 첫번째 등록하는 씬이 초기화면이 된다.
	JEngine::SceneManager::GetInstance()->RegistScene(new TitleScene);
	JEngine::SceneManager::GetInstance()->RegistScene(new Game1);
	JEngine::SceneManager::GetInstance()->RegistScene(new Game2);

	//엔진 시작
	return engine.StartEngine(hInstance);
}