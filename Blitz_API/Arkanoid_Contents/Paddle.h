#pragma once
#include <EngineCore/Actor.h>
#include "Ball.h"

class APaddle : public AActor
{
public:
	APaddle();
	~APaddle(){}

	void Tick(float _DeltaTime) override;

	FVector2D CheckCollision(const FVector2D& ballPos);
	FVector2D PaddleScale = { 0,0 };
	FVector2D GetVel()
	{
		return Vel;
	}

protected:

private:
	float Speed = 700.f;
	int MySpriteIndex = 0;
	FVector2D Vel;

	class ABall* ball=nullptr;
	class USpriteRenderer* SpriteRenderer;

};
