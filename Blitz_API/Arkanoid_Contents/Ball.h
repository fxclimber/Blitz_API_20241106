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

	void SetSpeed(float _Speed)
	{
		Speed = _Speed;
	}

	void SetDir(FVector2D _Value)
	{
		Value = _Value;
		Value.Normalize();
	}

	void MoveFunction(const FVector2D& velocity);
	void Reflect(const FVector2D& normal);


	void UpdatePosition(float deltaTime);

private:
	float Speed = 1.0f;
	int MySpriteIndex = 0;

	FVector2D Value;

	class USpriteRenderer* SpriteRenderer;

};

