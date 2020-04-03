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
	m_pExplosion[2] = JEngine::ResoucesManager::GetInstance()->GetBitmap("res\\explosion3.bmp");
	m_pStar[0] = JEngine::ResoucesManager::GetInstance()->GetBitmap("res\\FlightGameStar1.bmp");
	m_pStar[1] = JEngine::ResoucesManager::GetInstance()->GetBitmap("res\\FlightGameStar2.bmp");
	m_pStar[2] = JEngine::ResoucesManager::GetInstance()->GetBitmap("res\\FlightGameStar3.bmp");
	m_pFever[0] = JEngine::ResoucesManager::GetInstance()->GetBitmap("res\\Fever1.bmp");
	m_pFever[1] = JEngine::ResoucesManager::GetInstance()->GetBitmap("res\\Fever2.bmp");
	m_pFever[2] = JEngine::ResoucesManager::GetInstance()->GetBitmap("res\\Fever3.bmp");
	m_pFeverEffect = JEngine::ResoucesManager::GetInstance()->GetBitmap("res\\FeverEffect3.bmp");
	m_pTimeOver = JEngine::ResoucesManager::GetInstance()->GetBitmap("res\\TimeOver.bmp");

	m_pPoint = new JEngine::Label();
	m_pStarPoint = new JEngine::Label();

	gameTime = GetTickCount() + 45000;
	bulletTime = GetTickCount() + 1000;
	starTime = GetTickCount() + 2000;

	explosion = false;

	flightPoint = JEngine::InputManager::GetInstance()->GetMousePoint();
	flightPoint.x -= 30;
	flightPoint.y -= 30;

	feverLevel = 0;
	feverGauge = 0;

	starPoint = 100;
	point = 0;

	bulletDealay = 250;
	starDealay = 500;

	feverUp = false;

	gameOver = false;
	timeOver = false;
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
	if (gameOver) {
		int bestScore;
		HANDLE hFile = CreateFile(saveData, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		DWORD readB;
		ReadFile(hFile, &bestScore, sizeof(int), &readB, NULL);
		CloseHandle(hFile);
		if (point >= bestScore)
			SaveScore();
		JEngine::SceneManager::GetInstance()->LoadScene(SCENE_INDEX_TITLE);
	}
	if (gameTime <= GetTickCount() && !timeOver) {
		timeOver = true;
		timeOverTime = GetTickCount() + 1000;
	}


	StarCollisionCheck();
	if (explosion == false && BulletCollisionCheck()) {
		explosion = true;
		explosionTime = GetTickCount() + 270;
	}
	flightPoint = JEngine::InputManager::GetInstance()->GetMousePoint();
	flightPoint.x -= 30;
	flightPoint.y -= 30;
	UpdateBullet();
	UpdateStar();
}

void Game2::Draw(HDC hdc)
{
	if (timeOver) {
		if (timeOverTime > GetTickCount())
			m_pTimeOver->Draw(120, 300);
		else
			gameOver = true;
		return;
	}
	m_pBack->DrawBitblt(0, 0);
	if (explosion) {
		if (GetTickCount() <= explosionTime + 60)
			m_pExplosion[0]->Draw(explosionX, explosionY);
		else if (GetTickCount() <= explosionTime + 180)
			m_pExplosion[1]->Draw(explosionX, explosionY);
		else if (GetTickCount() <= explosionTime + 270)
			m_pExplosion[2]->Draw(explosionX, explosionY);
		else
			explosion = false;
	}
	else
		m_pFlight->Draw(flightPoint);
	m_pTimeBar->StretchDraw(25, 620, (gameTime - GetTickCount()) / 45000, 1);
	for (int i = 0; i < bulletList.size(); i++) {
		m_pBullet->Draw(bulletList[i]->x, bulletList[i]->y);
	}
	for (int i = 0; i < starList.size(); i++) {
		m_pStar[feverLevel % 3]->Draw(starList[i]->x, starList[i]->y);
	}
	if (feverLevel >= 1) {
		m_pFever[(feverLevel - 1) % 3]->StretchDraw(20, 55, 1, 1);
	}
	m_pFever[feverLevel % 3]->StretchDraw(20, 55, feverGauge / 100.0f, 1);

	if (feverLevel > 3) {
		int a = 0;
	}

	m_pPoint->Init(to_string(point), 200, 20, DT_CENTER | DT_WORDBREAK);
	for (int i = 0; i < starList.size(); i++) {
		m_pStarPoint->Init(to_string(starPoint), starList[i]->x + 15, starList[i]->y + 20, DT_CENTER | DT_WORDBREAK);
		m_pStarPoint->Draw();
	}
	m_pPoint->Draw();

	if (feverLevel >= 1) {
		if (GetTickCount() % 1000 < 250)
			m_pFeverEffect->Draw(0, 0);
	}
}

void Game2::Release()
{
	delete m_pPoint;
	delete m_pStarPoint;
	for (int i = 0; i < starList.size();i++) {
		delete starList[i];
	}
	starList.clear();
	for (int i = 0; i < bulletList.size(); i++)
		delete bulletList[i];
	bulletList.clear();
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
		bulletTime += bulletDealay;
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
		int starNums;
		if (feverUp) {
			starNums = 7;
			feverUp = false;
		}
		else
			starNums = 1;
		for (int i = 0; i < starNums; i++) {
			Star* tempStar;
			if (rand() % 2 == 0) {
				float starX;
				float starY = rand() % 641;
				if (rand() % 2 == 0)
					starX = 0;
				else
					starX = 414;
				tempStar = new Star(starX, starY, (flightPoint.x - starX) / 2000.0f, (flightPoint.y - starY) / 2000.0f);
			}
			else {
				float starX = rand() % 414;;
				float starY = 0;
				if (rand() % 2 == 0)
					starY = 0;
				else
					starY = 614;
				tempStar = new Star(starX, starY, (flightPoint.x - starX) / 2000.0f, (flightPoint.y - starY) / 2000.0f);
			}
			starList.push_back(tempStar);
		}
		starTime += starDealay;
	}
	for (int i = 0; i < starList.size(); i++) {
		starList[i]->Move();
	}
	for (int i = 0; i < starList.size();) {
		if (starList[i]->rangeOut()) {
			delete starList[i];
			starList.erase(starList.begin() + i);
		}
		else
			i++;
	}
}

bool Game2::BulletCollisionCheck() {
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
			starPoint = 100;
			feverLevel = 0;
			feverGauge = 0;
			return true;
		}
	}
	return false;
}

bool Game2::StarCollisionCheck() {
	if (explosion)
		return false;
	RECT flightRect = { flightPoint.x + 3, flightPoint.y + 3, flightPoint.x + 67, flightPoint.y + 63 };
	RECT starRect;
	RECT temp;
	for (int i = 0; i < starList.size(); i++) {
		starRect = { (LONG)starList[i]->x, (LONG)starList[i]->y, (LONG)starList[i]->x + 50, (LONG)starList[i]->y + 50 };
		if (IntersectRect(&temp,&flightRect, &starRect)) {
			delete starList[i];
			starList.erase(starList.begin() + i);
			feverGauge += 8;
			if (feverGauge > 100) {
				feverLevel++;
				feverGauge = feverGauge -= 100;
				feverUp = true;
				starTime -= starDealay;
			}
			point += starPoint;
			starPoint += 100;
			return true;
		}
	}
	return false;
}

void Game2::SaveScore() {
	HANDLE hFile = CreateFile(saveData, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	DWORD writeB;
	WriteFile(hFile, &point, sizeof(int), &writeB, NULL);
	CloseHandle(hFile);
}