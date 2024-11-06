#pragma once
#include <EngineCore/Actor.h>

class ABall : public AActor
{
public:
	ABall();
	~ABall();

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	//void MoveFunction(FVector2D _Dir);

	USpriteRenderer* GetRender()
	{
		return SpriteRenderer;
	}

	FVector2D vel = {0.f,0.f};

	void MoveFunction(const FVector2D& velocity);
	void Reflect(const FVector2D& normal);

private:
	float Speed = 1.0f;
	int MySpriteIndex = 0;

	class USpriteRenderer* SpriteRenderer;

};

