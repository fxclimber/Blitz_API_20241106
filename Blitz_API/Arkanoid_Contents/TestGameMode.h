#pragma once
#include <EngineCore/GameMode.h>
#include <vector>
#include <EnginePlatform/EngineSound.h>

class Map_Bottom;

class ATestGameMode : public AGameMode
{
public:
	ATestGameMode();
	~ATestGameMode(){}

	void SpawnBall();
	std::vector<class ABall*> Balls;
	void CheckScore();
	int AllBricksNonBreakable();
	bool GetGameClear()const
	{
		return GameClear;
	}

	int CountBreakableBricks();
	static bool IsEnd;

	int test();
	Map_Bottom* GetBottom()	{
		if (nullptr != Bottom){return Bottom;}
		else{return nullptr;}
	}

protected:
	void BeginPlay() override;

	void Tick(float _DeltaTime) override;


private:
	class BrickEditor* Editor=nullptr;
	class BrickEditor* EditorLoad = nullptr;
	class APaddle* Paddle = nullptr;
	class AScore* score = nullptr;
	class Map_Bottom* Bottom = nullptr;
	FVector2D UIPos = {0,0};

	bool bIsBonusActive = false;
	class Brick* BonusA;
	bool GameClear = false;
	int TotalCount = 100;
	int BreakCountTotal = 1000;
	int DeathCount = 650;

};

