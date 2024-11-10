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
	void SetIsMove(bool _value)
	{
		IsMove = _value; // 값을 할당하도록 수정
	}

	bool GetIsMove()
	{
		return IsMove;
	}

	FVector2D GetVel()
	{
		return Value;
	}
	void MoveFunction(const FVector2D& velocity);
	void Reflect(const FVector2D& normal);
	void UpdatePosition(float deltaTime);


private:
	float Speed = 1.0f;
	int MySpriteIndex = 0;
	bool IsMove=false;

	FVector2D Value;

	class USpriteRenderer* SpriteRenderer;

};

