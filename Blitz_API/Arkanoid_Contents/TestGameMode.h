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
	bool AllBricksNonBreakable();
	bool GetGameClear()const
	{
		return GameClear;
	}

	int CountBreakableBricks();
	static bool IsEnd;

	Map_Bottom* GetBottom()	{
		if (nullptr != Bottom){return Bottom;}
		else{return nullptr;}
	}
	bool IsUIMove=false;
	bool IsEditMode=false;

protected:
	void BeginPlay() override;

	void Tick(float _DeltaTime) override;


private:
	class BrickEditor* Editor=nullptr;
	//class BrickEditor* EditorLoad = nullptr;
	class APaddle* Paddle = nullptr;
	class AScore* score = nullptr;
	class Map_Bottom* Bottom = nullptr;
	FVector2D UIPos = {0,0};

	bool bIsBonusActive = false;
	class Brick* BonusA;

	bool GameClear = false;
	int BreakCountTotal = 11;
	int DeathCount = 2;
	std::string LoadFileName;

	class UI* UILetters = nullptr;
	class UI_P* UILettersP = nullptr;
};

