#include <Windows.h>
#include "EngineMain.h"
#include "SceneManager.h"
#include "TitleScene.h"
#include "GameScene.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	//엔진 객체를 생성하고 타이틀과 윈도우 사이즈를 넣어준다.
	JEngine::EngineMain engine("EngineTest", 1024, 768);
	//씬을 등록한다. 첫번째 등록하는 씬이 초기화면이 된다.
	JEngine::SceneManager::GetInstance()->RegistScene(new TitleScene);
	JEngine::SceneManager::GetInstance()->RegistScene(new GameScene);

	//엔진 시작
	return engine.StartEngine(hInstance);
}