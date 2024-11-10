#pragma once
#include <EngineCore/GameMode.h>

// Ό³Έν :
class ATestGameMode : public AGameMode
{
public:
	// constrcuter destructer
	ATestGameMode(){}
	~ATestGameMode(){}


protected:
	void BeginPlay() override;

	void Tick(float _DeltaTime) override;

	void StartStage();
	float GetElapsedTime() const;

private:
	class BrickEditor* Editor;
	class ABall* Ball;
	class APaddle* Paddle;

};

