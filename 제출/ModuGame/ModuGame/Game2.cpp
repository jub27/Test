#include "Game2.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "UIManager.h"
#include "ResoucesManager.h"
#include <Windows.h>
#include "defines.h"

Game2::Game2()
{
}

Game2::~Game2()
{
}

void Game2::Init(HWND hWnd)
{
	JEngine::InputManager::GetInstance()->Clear();
	JEngine::InputManager::GetInstance()->RegistKeyCode(VK_SPACE);
	JEngine::InputManager::GetInstance()->RegistKeyCode(VK_ESCAPE);
	JEngine::InputManager::GetInstance()->RegistKeyCode(VK_LEFT);
	JEngine::InputManager::GetInstance()->RegistKeyCode(VK_RIGHT);
	JEngine::InputManager::GetInstance()->RegistKeyCode(VK_UP);
	JEngine::InputManager::GetInstance()->RegistKeyCode(VK_DOWN);
	JEngine::InputManager::GetInstance()->RegistKeyCode(VK_LBUTTON);

	m_pBack = JEngine::ResoucesManager::GetInstance()->GetBitmap("res\\FlightGameBack.bmp");
	m_pTimeBar = JEngine::ResoucesManager::GetInstance()->GetBitmap("res\\ColoredPaperTimeBar.bmp");
	m_pFlight = JEngine::ResoucesManager::GetInstance()->GetBitmap("res\\FlightGameFlight.bmp");
	m_pBullet = JEngine::ResoucesManager::GetInstance()->GetBitmap("res\\FlightGameBullet.bmp");
	m_pExplosion[0] = JEngine::ResoucesManager::GetInstance()->GetBitmap("res\\explosion1.bmp");
	m_pExplosion[1] = JEngine::ResoucesManager::GetInstance()->GetBitmap("res\\explosion2.bmp");

	gameTime = GetTickCount() + 45000;
	bulletTime = GetTickCount() + 1000;
	starTime = GetTickCount() + 2000;

	explosion = false;

	flightPoint = JEngine::InputManager::GetInstance()->GetMousePoint();
}

bool Game2::Input(float fETime)
{
	if (JEngine::InputManager::GetInstance()->isKeyUp(VK_ESCAPE)) {
		JEngine::SceneManager::GetInstance()->LoadScene(SCENE_INDEX_TITLE);
	}

	return false;
}

void Game2::Update(float fETime)
{
	if (gameTime <= GetTickCount())
		JEngine::SceneManager::GetInstance()->LoadScene(SCENE_INDEX_TITLE);

	if (explosion == false && collisionCheck()) {
		explosion = true;
		explosionTime = GetTickCount() + 250;
	}
	flightPoint = JEngine::InputManager::GetInstance()->GetMousePoint();
	UpdateBullet();
}

void Game2::Draw(HDC hdc)
{
	m_pBack->DrawBitblt(0, 0);
	if (explosion) {
		if (GetTickCount() <= explosionTime + 125)
			m_pExplosion[0]->Draw(explosionX, explosionY);
		else if (GetTickCount() <= explosionTime + 250)
			m_pExplosion[1]->Draw(explosionX, explosionY);
		else
			explosion = false;
	}
	else
		m_pFlight->Draw(flightPoint);
	m_pTimeBar->StretchDraw(25, 620, (gameTime - GetTickCount()) / 45000, 1);
	for (int i = 0; i < bulletList.size(); i++) {
		m_pBullet->Draw(bulletList[i]->x, bulletList[i]->y);
	}
}

void Game2::Release()
{

}

void Game2::UpdateBullet() {
	if (bulletTime <= GetTickCount()) {
		Bullet* tempBullet;
		if (rand() % 2 == 0) {
			float bulletX;
			float bulletY = rand() % 641;
			if (rand() % 2 == 0)
				bulletX = 0;
			else
				bulletX = 414;
			tempBullet = new Bullet(bulletX, bulletY, (flightPoint.x - bulletX) / 2000.0f, (flightPoint.y - bulletY) / 2000.0f);
		}
		else {
			float bulletX = rand() % 414;;
			float bulletY = 0;
			if (rand() % 2 == 0)
				bulletY = 0;
			else
				bulletY = 614;
			tempBullet = new Bullet(bulletX, bulletY, (flightPoint.x - bulletX) / 2000.0f, (flightPoint.y - bulletY) / 2000.0f);
		}
		bulletList.push_back(tempBullet);
		bulletTime += 300;
	}
	for (int i = 0; i < bulletList.size(); i++) {
		bulletList[i]->Move();
	}
	for (int i = 0; i < bulletList.size();) {
		if (bulletList[i]->rangeOut()) {
			delete bulletList[i];
			bulletList.erase(bulletList.begin() + i);
		}
		else
			i++;
	}
}

void Game2::UpdateStar() {///////////////////////////////////BULLET을 STAR로 수정
	if (starTime <= GetTickCount()) {
		Bullet* tempBullet;
		if (rand() % 2 == 0) {
			float bulletX;
			float bulletY = rand() % 641;
			if (rand() % 2 == 0)
				bulletX = 0;
			else
				bulletX = 414;
			tempBullet = new Bullet(bulletX, bulletY, (flightPoint.x - bulletX) / 2000.0f, (flightPoint.y - bulletY) / 2000.0f);
		}
		else {
			float bulletX = rand() % 414;;
			float bulletY = 0;
			if (rand() % 2 == 0)
				bulletY = 0;
			else
				bulletY = 614;
			tempBullet = new Bullet(bulletX, bulletY, (flightPoint.x - bulletX) / 2000.0f, (flightPoint.y - bulletY) / 2000.0f);
		}
		bulletList.push_back(tempBullet);
		bulletTime += 300;
	}
	for (int i = 0; i < bulletList.size(); i++) {
		bulletList[i]->Move();
	}
	for (int i = 0; i < bulletList.size();) {
		if (bulletList[i]->rangeOut()) {
			delete bulletList[i];
			bulletList.erase(bulletList.begin() + i);
		}
		else
			i++;
	}
}

bool Game2::collisionCheck() {
	RECT in_Rect = { flightPoint.x + 10, flightPoint.y + 10, flightPoint.x + 30, flightPoint.y + 30 };
	POINT in_Pt;
	for (int i = 0; i < bulletList.size(); i++) {
		in_Pt.x = bulletList[i]->x;
		in_Pt.y = bulletList[i]->y;
		if (PtInRect(&in_Rect, in_Pt)) {
			delete bulletList[i];
			bulletList.erase(bulletList.begin() + i);
			explosionX = flightPoint.x - 30;
			explosionY = flightPoint.y - 30;
			return true;
		}
	}
	return false;
}
