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
	void CheckStageClear();
	void HandleStageClear();
	bool AreAllBricksNonBreakable();
	bool GetGameClear()const
	{
		return GameClear;
	}

protected:
	void BeginPlay() override;

	void Tick(float _DeltaTime) override;


private:
	class BrickEditor* Editor;
	class BrickEditor* EditorLoad;
	class APaddle* Paddle;

	class AScore* score;
	FVector2D UIPos = {0,0};

	bool bIsBonusActive = false;
	class Brick* BonusA;
	bool GameClear = false;

	// sound

};

