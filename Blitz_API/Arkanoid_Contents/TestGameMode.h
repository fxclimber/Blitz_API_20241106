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

};

