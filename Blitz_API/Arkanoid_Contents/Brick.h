#pragma once
#include <EngineCore/Actor.h>
#include <EngineBase/EngineSerializer.h>

#include <EnginePlatform/EngineInput.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/EngineAPICore.h>
#include <EngineCore/EngineCoreDebug.h>
#include "ContentsEnum.h"


class Brick : public AActor
{
public:
	Brick();
	~Brick();

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	USpriteRenderer* GetRender()
	{
		return SpriteRenderer;
	}
	//FVector2D CheckCollision(const FVector2D& playerPos, const FVector2D& playerSize);

	class USpriteRenderer* SpriteRenderer;

	void SetSpeed(float _Speed)
	{
		Speed = _Speed;
	}

	void SetDir(FVector2D _Value)
	{
		Value = _Value;
		Value.Normalize();
	}

	BonusType GetRandomBonusType();

	void MakeBonus();
	void MakeBonusRenderer();


protected:

private:
	int MySpriteIndex = 0;
	FVector2D Value;
	float Speed = 1.0f;
	FVector2D spriteScale = { 0,0 };

};

