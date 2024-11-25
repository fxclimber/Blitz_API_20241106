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

	// �������ھ� ��ü�� �����, ���� EngineStart�Լ��� ���ڷ� ����
	//�� ���� Ŭ���� 
	ArkanoidContentsCore User;

	// ���� ���� Ŭ���� Core, hInstance�� ��������ü �����͸� ���ڷ�.
	return UEngineAPICore::EngineStart(hInstance, &User);

}

