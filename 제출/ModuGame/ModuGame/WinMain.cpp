#include <Windows.h>
#include <time.h>
#include "EngineMain.h"
#include "SceneManager.h"
#include "TitleScene.h"
#include "Game1.h"
#include "Game2.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	//���� ��ü�� �����ϰ� Ÿ��Ʋ�� ������ ����� �־��ش�.
	srand(time(NULL));
	JEngine::EngineMain engine("ModuGame", 414, 641);
	//���� ����Ѵ�. ù��° ����ϴ� ���� �ʱ�ȭ���� �ȴ�.
	JEngine::SceneManager::GetInstance()->RegistScene(new TitleScene);
	JEngine::SceneManager::GetInstance()->RegistScene(new Game1);
	JEngine::SceneManager::GetInstance()->RegistScene(new Game2);

	//���� ����
	return engine.StartEngine(hInstance);
}