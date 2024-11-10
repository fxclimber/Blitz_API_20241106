#pragma once
#include <EngineCore/GameMode.h>

// ���� :
class ATestGameMode : public AGameMode
{
public:
	// constrcuter destructer
	ATestGameMode(){}
	~ATestGameMode(){}


protected:
	void BeginPlay() override;

	void Tick(float _DeltaTime) override;


private:
	class BrickEditor* Editor;
	class ABall* Ball;
	class APaddle* Paddle;

	class AScore* score;
	FVector2D UIPos = {0,0};
};

