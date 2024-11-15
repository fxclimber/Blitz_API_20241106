#pragma once
#include <EngineCore/GameMode.h>
#include <vector>
#include <EnginePlatform/EngineSound.h>


class ATestGameMode : public AGameMode
{
public:
	ATestGameMode(){}
	~ATestGameMode(){}


protected:
	void BeginPlay() override;

	void Tick(float _DeltaTime) override;

	void SpawnBall();

private:
	class BrickEditor* Editor;
	class APaddle* Paddle;
	std::vector<class ABall*> Balls;

	class AScore* score;
	FVector2D UIPos = {0,0};

	bool bIsBonusActive = false;

	// sound

};

