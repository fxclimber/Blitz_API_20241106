#pragma once
#include <EngineCore/GameMode.h>
#include <vector>
#include <EnginePlatform/EngineSound.h>


class ATestGameMode : public AGameMode
{
public:
	ATestGameMode(){}
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
	Map_Bottom* GetBt()
	{
		return bt;
	}

protected:
	void BeginPlay() override;

	void Tick(float _DeltaTime) override;


private:
	class Map_Bottom* bt = nullptr;

	class BrickEditor* Editor;
	class BrickEditor* EditorLoad;
	class APaddle* Paddle;

	class AScore* score;
	FVector2D UIPos = {0,0};

	bool bIsBonusActive = false;
	class Brick* BonusA;
	bool GameClear = false;
	int TotalCount = 100;
	int BreakCountTotal = 1000;
	int DeathCount = 650;


};

