#include "PreCompiledFile.h"
#include <Arkanoid_Contents/ArkanoidContentsCore.h>
#include <EngineCore/EngineAPICore.h>

#include <windows.h>
#include <iostream>

#pragma comment (lib, "EngineCore.lib")
#pragma comment(lib,"Arkanoid_Contents.lib")

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	{
		//// 콘솔 창을 생성
		//AllocConsole();
		//// 콘솔 출력 스트림을 표준 출력에 연결
		//FILE* fp;
		////freopen_s(&fp, "file.txt", "w", stdout);//파일오픈
		//freopen_s(&fp, "CONOUT$", "w", stdout); // 콘솔 창에 직접 출력되도록 설정
		//std::cout << "Hello, Eunjeong!" << std::endl;

		//// 메인 프로그램 실행
		//MessageBox(NULL, "Hello, Eunjeong!", "Title", MB_OK);
		//// 콘솔 창 해제
		//FreeConsole();
	}

	// 컨텐츠코어 객체를 만들어, 엔진 EngineStart함수의 인자로 전달
	//내 게임 클래스 
	ArkanoidContentsCore User;

	//UEngineWindow win1;
	//win1.EngineWindowInit(hInstance);
	//win1.Open("Window2 open");//Create에 이름 바꿔야한다...지금 안됨.

	// 엔진 시작 클래스 Core, hInstance와 컨텐츠객체 포인터를 인자로.
	return UEngineAPICore::EngineStart(hInstance, &User);

	
}

