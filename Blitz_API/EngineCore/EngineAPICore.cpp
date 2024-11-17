#include "PreCompiledFile.h"
#include "EngineAPICore.h"
#include <EnginePlatform/EngineInput.h>
#include <EnginePlatform/EngineSound.h>
#include <EnginePlatform/EngineWindow.h>
#include <EngineBase/EngineDelegate.h>
#include <EngineBase/EngineDebug.h>



// 엔진을 통틀어서 1번 만들어지기 때문에.
// 1번 세팅되고 절대로 바뀌지 않을거다.
UEngineAPICore* UEngineAPICore::MainCore = nullptr;
UContentsCore* UEngineAPICore::UserCore = nullptr;

#include <Windows.h>

UEngineAPICore::UEngineAPICore()
{

}

UEngineAPICore::~UEngineAPICore()
{
	std::map<std::string, class ULevel*>::iterator StartIter = Levels.begin();
	std::map<std::string, class ULevel*>::iterator EndIter = Levels.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		if (nullptr != StartIter->second)
		{
			delete StartIter->second;
			StartIter->second = nullptr;
		}
	}

	Levels.clear();
	UEngineSound::Release();
}


int UEngineAPICore::EngineStart(HINSTANCE _Inst, UContentsCore* _UserCore)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	UserCore = _UserCore;

	UEngineWindow::EngineWindowInit(_Inst);

	// 엔진의 기능이 집약되어 있다. CreateLevel;
	UEngineAPICore Core = UEngineAPICore();
	Core.EngineMainWindow.Open();
	MainCore = &Core;

	EngineDelegate Start = EngineDelegate(std::bind(EngineBeginPlay));
	EngineDelegate FrameLoop = EngineDelegate(std::bind(EngineTick));;
	return UEngineWindow::WindowMessageLoop(Start, FrameLoop);
}

void UEngineAPICore::EngineBeginPlay()
{
	UserCore->BeginPlay();
}

// 이 함수가 1초에 몇번 실행되냐가 프레임
void UEngineAPICore::EngineTick()
{

	// 지금 전혀 용도를 찾지 못해서 사용하지 않는 함수
	UserCore->Tick();

	MainCore->Tick();
}

void UEngineAPICore::Tick()
{
	if (nullptr != NextLevel)
	{
		if (nullptr != CurLevel)
		{
			CurLevel->LevelChangeEnd();
		}

		CurLevel = NextLevel;

		NextLevel->LevelChangeStart();

		NextLevel = nullptr;
		// 델타타임이 지연될수 있으므로 델타타임을 초기화시켜주는것이 좋다.
		DeltaTimer.TimeStart();
	}

	// 시간을 잴겁니다. 현재시간 
	DeltaTimer.TimeCheck();
	float DeltaTime = DeltaTimer.GetDeltaTime();

	// 사운드 제거
	UEngineSound::Update();

	// 키체크
	UEngineInput::GetInst().KeyCheck(DeltaTime);

	if (nullptr == CurLevel)
	{
		MSGASSERT("엔진 코어에 현재 레벨이 지정되지 않았습니다");
		return;
	}

	UEngineInput::GetInst().EventCheck(DeltaTime);
	CurLevel->Tick(DeltaTime);
	CurLevel->Render(DeltaTime);
	CurLevel->Release(DeltaTime);
}


void UEngineAPICore::OpenLevel(std::string_view _LevelName)
{
	std::string ChangeName = _LevelName.data();

	std::map<std::string, class ULevel*>::iterator FindIter = Levels.find(ChangeName);
	std::map<std::string, class ULevel*>::iterator EndIter = Levels.end();

	if (EndIter == FindIter)
	{
		MSGASSERT(ChangeName + "라는 이름의 레벨은 존재하지 않습니다.");
		return;
	}

	NextLevel = FindIter->second;
}