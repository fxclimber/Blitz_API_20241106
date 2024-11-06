#pragma once
#include <string>
// 여러분들이 여기에다가 이렇게 특정 헤더를 넣으면
// F5를 누를때마다. #include <Windows.h>가 재빌드 된다.
// 미리컴파일된 헤더를 사용하면
// 미리컴파일된 헤더에 넣어진 헤더는 빌드를하면 .pch파일에 빌드결과가 남고
// 그후에는 빌드되지 않는다.
// 컴파일 시간이 
#include <Windows.h>
#include <EnginePlatform/EngineWindow.h>
#include <EngineBase/EngineTimer.h>

#pragma comment (lib, "EngineBase.lib")
#pragma comment (lib, "EnginePlatform.lib")

#include "Level.h"

// 함수포인터
// 가상함수

class UContentsCore
{
public:
	virtual void BeginPlay() = 0;
	virtual void Tick() = 0;
};

// UEngineAPICore => 언리얼로 보면 world라고 볼수 있다.

// 설명 :
class UEngineAPICore
{
public:
	// constrcuter destructer
	UEngineAPICore();
	~UEngineAPICore();

	// delete Function
	UEngineAPICore(const UEngineAPICore& _Other) = delete;
	UEngineAPICore(UEngineAPICore&& _Other) noexcept = delete;
	UEngineAPICore& operator=(const UEngineAPICore& _Other) = delete;
	UEngineAPICore& operator=(UEngineAPICore&& _Other) noexcept = delete;

	static int EngineStart(HINSTANCE _Inst, UContentsCore* _UserCore);

	static class UEngineAPICore* GetCore()
	{
		return MainCore;
	}

	UEngineWindow& GetMainWindow()
	{
		return EngineMainWindow;
	}

	float GetDeltaTime()
	{
		return DeltaTimer.GetDeltaTime();
	}

	template<typename GameModeType, typename MainPawnType>
	ULevel* CreateLevel(std::string_view _LevelName)
	{
		ULevel* NewLevel = new ULevel();

		// 게임모드가 Level의 특성을 설정하는 중요객체이기 때문이다.
		NewLevel->CreateGameMode<GameModeType, MainPawnType>();

		// 관리란 뭐냐?
		// 삭제되는 객체를 만들고.
		// 그 객체안에 자료구조 넣은다음
		// 그 자료구조안에 새롭게 만들어지는 객체들을 보관하는것.ㄴ
		Levels.insert({ _LevelName.data() , NewLevel });

		return NewLevel;
	}

	void OpenLevel(std::string_view _LevelName);



protected:

private:
	static void EngineBeginPlay();
	static void EngineTick();
	static UEngineAPICore* MainCore;
	static UContentsCore* UserCore;

	UEngineTimer DeltaTimer = UEngineTimer();
	UEngineWindow EngineMainWindow = UEngineWindow(); // 엔진 메인 윈도우

	// 누가 레벨의 소유자라고 개념을 잡는게 좋냐?

	// 만들어진 모든 레벨
	std::map<std::string, class ULevel*> Levels;

	// 현재 내가 눈으로 보고 있어야하는 레벨
	// 돌아가고 있는 레벨
	// 포인터 체인지 방식
	class ULevel* CurLevel = nullptr;
	class ULevel* NextLevel = nullptr;

	void Tick();

};