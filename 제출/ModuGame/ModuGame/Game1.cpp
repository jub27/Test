#include "Game1.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "UIManager.h"
#include "ResoucesManager.h"
#include <Windows.h>
#include "defines.h"

Game1::Game1()
{
}

Game1::~Game1()
{
}

void Game1::Init(HWND hWnd)
{
	JEngine::InputManager::GetInstance()->Clear();
	JEngine::InputManager::GetInstance()->RegistKeyCode(VK_SPACE);
	JEngine::InputManager::GetInstance()->RegistKeyCode(VK_ESCAPE);
	JEngine::InputManager::GetInstance()->RegistKeyCode(VK_LEFT);
	JEngine::InputManager::GetInstance()->RegistKeyCode(VK_RIGHT);
	JEngine::InputManager::GetInstance()->RegistKeyCode(VK_UP);
	JEngine::InputManager::GetInstance()->RegistKeyCode(VK_DOWN);
	JEngine::InputManager::GetInstance()->RegistKeyCode(VK_LBUTTON);

	m_pBack = JEngine::ResoucesManager::GetInstance()->GetBitmap("res\\ColoredPaperBack.bmp");
	m_pColorList[BLUE] = JEngine::ResoucesManager::GetInstance()->GetBitmap("res\\ColoredPaperBlue.bmp");
	m_pColorList[GREEN] = JEngine::ResoucesManager::GetInstance()->GetBitmap("res\\ColoredPaperGreen.bmp");
	m_pColorList[RED] = JEngine::ResoucesManager::GetInstance()->GetBitmap("res\\ColoredPaperRed.bmp");
	m_pColorList[YELLOW] = JEngine::ResoucesManager::GetInstance()->GetBitmap("res\\ColoredPaperYellow.bmp");
	m_pTimeBar = JEngine::ResoucesManager::GetInstance()->GetBitmap("res\\ColoredPaperTimeBar.bmp");
	m_pFever[0] = JEngine::ResoucesManager::GetInstance()->GetBitmap("res\\Fever1.bmp");
	m_pFever[1] = JEngine::ResoucesManager::GetInstance()->GetBitmap("res\\Fever2.bmp");
	m_pFever[2] = JEngine::ResoucesManager::GetInstance()->GetBitmap("res\\Fever3.bmp");
	m_pStar[0] = JEngine::ResoucesManager::GetInstance()->GetBitmap("res\\FlightGameStar1.bmp");
	m_pStar[1] = JEngine::ResoucesManager::GetInstance()->GetBitmap("res\\FlightGameStar2.bmp");
	m_pStar[2] = JEngine::ResoucesManager::GetInstance()->GetBitmap("res\\FlightGameStar3.bmp");
	m_pFeverEffect = JEngine::ResoucesManager::GetInstance()->GetBitmap("res\\FeverEffect3.bmp");

	m_pPoint = new JEngine::Label();
	m_pPaperPoint = new JEngine::Label();

	is_moving = false;

	Board[0] = ((Color)(rand()%4));
	Board[1] = ((Color)(rand()%4));

	point = 0;
	paperPoint = 90;
	comboCount = 0;
	is_star = false;

	feverLevel = 0;
	feverGauge = 0;

	moveSpeed = 10;
	paperX = 150;
	paperY = 300;
	gameTime = GetTickCount() + 45000;
}

bool Game1::Input(float fETime)
{
	if (!is_moving) {
		if (JEngine::InputManager::GetInstance()->isKeyDown(VK_UP)) {
			paperDirection = UP;
			is_moving = true;
		}
		else if (JEngine::InputManager::GetInstance()->isKeyDown(VK_DOWN)) {
			paperDirection = DOWN;
			is_moving = true;
		}
		else if (JEngine::InputManager::GetInstance()->isKeyDown(VK_LEFT)) {
			paperDirection = LEFT;
			is_moving = true;
		}
		else if (JEngine::InputManager::GetInstance()->isKeyDown(VK_RIGHT)) {
			paperDirection = RIGHT;
			is_moving = true;
		}
	}
	if (JEngine::InputManager::GetInstance()->isKeyUp(VK_ESCAPE)) {
		JEngine::SceneManager::GetInstance()->LoadScene(SCENE_INDEX_TITLE);
	}
	return false;
}

void Game1::Update(float fETime)
{
	feverGauge -= 0.01;
	if (feverGauge < 0) {
		if (feverLevel >= 1) {
			feverLevel -= 1;
			feverGauge = 99;
		}
		else
			feverGauge = 0;
	}
	if (gameTime <= GetTickCount()) {
		int bestScore;
		HANDLE hFile = CreateFile(saveData, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		DWORD readB;
		ReadFile(hFile, &bestScore, sizeof(int), &readB, NULL);
		CloseHandle(hFile);
		if (point >= bestScore)
			SaveScore();
		JEngine::SceneManager::GetInstance()->LoadScene(SCENE_INDEX_TITLE);
	}
	time = fETime;
	MovePaper();
}

void Game1::Draw(HDC hdc)
{
	m_pBack->DrawBitblt(0, 0);
	
	if(is_moving)
		m_pColorList[Board[0]]->Draw(paperX, paperY);
	m_pTimeBar->StretchDraw(25, 620, (gameTime - GetTickCount()) / 45000, 1);
	m_pColorList[Board[1]]->Draw(160, 310);
	m_pColorList[Board[0]]->Draw(150, 300);
	if(feverLevel == 1)
		m_pFever[0]->StretchDraw(20, 55, 1, 1);
	else if(feverLevel == 2)
		m_pFever[1]->StretchDraw(20, 55, 1, 1);
	m_pFever[feverLevel]->StretchDraw(20, 55, feverGauge / 100.0f , 1);
	if (is_star) {
		m_pStar[feverLevel]->Draw(paperX + 30, paperY + 30);
	}
	m_pPoint->Init(to_string(point), 200, 20, DT_CENTER | DT_WORDBREAK);
	m_pPaperPoint->Init(to_string(paperPoint), paperX + 45, paperY + 50, DT_CENTER | DT_WORDBREAK);
	m_pPoint->Draw();
	m_pPaperPoint->Draw();
	if (feverLevel >= 1) {
		if(GetTickCount() % 500 < 250)
			m_pFeverEffect->Draw(0, 0);
	}
}

void Game1::Release()
{
	JEngine::UIManager::GetInstance()->RelaaseUI();
}

void Game1::MovePaper() {
	if (!is_moving)
		return;
	switch (paperDirection) {
	case UP:
		paperY -= moveSpeed;
		if (paperY <= 120) {
			if (Board[0] == GREEN) {
				point += paperPoint;
				Board[0] = Board[1];
				Board[1] = (Color)(rand() % 4);
				if (comboCount != 0 && comboCount % 5 == 0)
					feverGauge += 30;
				else
					feverGauge += 10;
				comboCount++;
			}
			else {
				feverGauge = feverGauge > 30 ? feverGauge - 30 : 0;
				comboCount = 0;
			}
			is_moving = false;
			paperY = 300;
		}
		break;
	case DOWN:
		paperY += moveSpeed;
		if (paperY >= 480) {
			if (Board[0] == YELLOW) {
				point += paperPoint;
				Board[0] = Board[1];
				Board[1] = (Color)(rand() % 4);
				if (comboCount != 0 && comboCount % 5 == 0)
					feverGauge += 30;
				else
					feverGauge += 10;
				comboCount++;
			}
			else {
				feverGauge = feverGauge > 30 ? feverGauge - 30 : 0;
				comboCount = 0;
			}
			is_moving = false;
			paperY = 300;
		}
		break;
	case LEFT:
		paperX -= moveSpeed;
		if (paperX <= 10) {
			if (Board[0] == BLUE) {
				point += paperPoint;
				Board[0] = Board[1];
				Board[1] = (Color)(rand() % 4);
				if (comboCount != 0 && comboCount % 5 == 0)
					feverGauge += 30;
				else
					feverGauge += 10;
				comboCount++;
			}
			else {
				feverGauge = feverGauge > 30 ? feverGauge - 30 : 0;
				comboCount = 0;
			}
			is_moving = false;
			paperX = 150;
		}
		break;
	case RIGHT:
		paperX += moveSpeed;
		if (paperX >= 290) {
			if (Board[0] == RED) {
				point += paperPoint;
				Board[0] = Board[1];
				Board[1] = (Color)(rand() % 4);
				if (comboCount != 0 && comboCount % 5 == 0)
					feverGauge += 30;
				else
					feverGauge += 10;
				comboCount++;
			}
			else {
				feverGauge = feverGauge > 30 ? feverGauge - 30 : 0;
				comboCount = 0;
			}
			is_moving = false;
			paperX = 150;
		}
		break;
	}
	if (!is_moving) {
		if (feverGauge > 100) {
			if (feverLevel < 2) {
				feverLevel++;
				feverGauge = 15;
			}
			else
				feverGauge = 100;
		}
		if (feverLevel > 0) {
			is_star = true;
			if (comboCount > 0)
				paperPoint += 100;
			else
				paperPoint = 100;
		}
		else if (comboCount != 0 && comboCount % 5 == 0) {
			is_star = true;
			paperPoint = 100 * (comboCount / 5);
		}
		else {
			paperPoint = 90;
			is_star = false;
		}
	}
}

void Game1::SaveScore() {
	HANDLE hFile = CreateFile(saveData, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	DWORD writeB;
	WriteFile(hFile, &point, sizeof(int), &writeB, NULL);
	CloseHandle(hFile);
}