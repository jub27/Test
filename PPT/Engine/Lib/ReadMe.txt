========================================================================
    STATIC LIBRARY : JEngine Project Overview
========================================================================

											author : classicaa@naver.com

JEngine은 게임아트학원 교육용으로 만들어진 엔진입니다.

WinAPI게임을 제작할수 있는 엔진으로
기본 화면 개념은 Scene개념으로 관리 됩니다.

엔진 객체중 SceneManager , ResoucesManager , InputManager 는 싱글톤으로 지원됩니다.

간단한 예제는 FrameWorkLib 프로젝트를 참조하세요.

Scene 객체는 
	virtual void Init(HWND hWnd) = 0;
	virtual bool Input() = 0;
	virtual void Update() = 0;
	virtual void Draw(HDC hdc) = 0;
	virtual void Release() = 0;
다섯가지의 순수 가상함수를 가지고 있습니다. Scene객체를 상속받아 구현해주세요.
(Input 메소드의 리턴값을 ture를 넘기면 게임이 종료됩니다.)

BitMap 객체는 컬러키 비트맵의 Draw를 지원하고 Anchor포인트를 지원합니다.

InputManager 객체는 기본 VK를 입력하여 해당키의 업 다운 프레스상태를 체크합니다.

POINT 객체 POINT RECT등의 객체를 클레스화한것 입니다.

ResoucesManager 객체는 Bitmap클레스의 할당과 해재를 담당하고 Back버퍼를 관리합니다.

/////////////////////////////////////////////////////////////////////////////
Other notes:


/////////////////////////////////////////////////////////////////////////////

