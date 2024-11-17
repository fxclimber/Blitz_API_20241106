#pragma once
#include <string>
// 미리컴파일된 헤더에 넣어진 헤더는 빌드를하면 .pch파일에 빌드결과가 남고
// 그후에는 빌드되지 않는다.
#include <Windows.h>
#include <EnginePlatform/EngineWindow.h>
#include <EngineBase/EngineTimer.h>

#pragma comment (lib, "EngineBase.lib")
#pragma comment (lib, "EnginePlatform.lib")

#include "Level.h"

class UContentsCore
{
public:
	virtual void BeginPlay() = 0;
	virtual void Tick() = 0;
};

// UEngineAPICore => 언리얼로 보면 world

class UEngineAPICore
{
public:
	// constrcuter destructer
	UEngineAPICore();
	~UEngineAPICore();

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

		// 게임모드가 Level의 특성을 설정하는 중요객체
		NewLevel->CreateGameMode<GameModeType, MainPawnType>();

		// 관리
		// 삭제되는 객체를 만들고.
		// 그 객체안에 자료구조 넣은다음
		// 그 자료구조안에 새롭게 만들어지는 객체들을 보관하는것.
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

	// 만들어진 모든 레벨
	std::map<std::string, class ULevel*> Levels;

	// 포인터 체인지 방식
	class ULevel* CurLevel = nullptr;
	class ULevel* NextLevel = nullptr;

	void Tick();

};