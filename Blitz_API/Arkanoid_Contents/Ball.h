#pragma once
#include <EngineCore/Actor.h>
#include <chrono>

class ABall : public AActor
{
public:
	ABall();
	~ABall(){}

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

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

	void MoveFunction(const FVector2D& _dir);
	void Reflect(const FVector2D& normal);
	void UpdatePosition(float deltaTime);
	bool Stop=false;
	//float GetElapsedTime() const;
	FVector2D SavePos = {0,0};
	float EndTime = 0.0f;
	bool GetFadeOver()
	{
		return FadeOver;
	}


private:
	float Speed = 1.0f;
	int MySpriteIndex = 0;
	bool IsMove;
	// 볼의 이동속도
	FVector2D Value;
	class USpriteRenderer* SpriteRenderer;
	class AFade* Fade;
	bool FadeOver=true;
};

