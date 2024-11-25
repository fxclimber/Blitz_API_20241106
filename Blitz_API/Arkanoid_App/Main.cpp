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
	}

	// 컨텐츠코어 객체를 만들어, 엔진 EngineStart함수의 인자로 전달
	//내 게임 클래스 
	ArkanoidContentsCore User;

	// 엔진 시작 클래스 Core, hInstance와 컨텐츠객체 포인터를 인자로.
	return UEngineAPICore::EngineStart(hInstance, &User);

}

