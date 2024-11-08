#pragma once
#include <EngineCore/Actor.h>
#include "Ball.h"

class APaddle : public AActor
{
public:
	APaddle();
	~APaddle();

	void BeginPlay()override;
	void Tick(float _DeltaTime) override;

	void MoveFunction(FVector2D _Dir);

	void RunSoundPlay();


	FVector2D CheckCollision(const FVector2D& ballPos);



protected:

private:
	float Speed = 700.f;
	int MySpriteIndex = 0;
	FVector2D Value;

	class ABall* ball;
	class USpriteRenderer* SpriteRenderer;

};
