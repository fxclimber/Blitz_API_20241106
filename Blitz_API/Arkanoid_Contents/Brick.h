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

	USpriteRenderer* GetRender()
	{
		return SpriteRenderer;
	}
	FVector2D CheckCollision(const FVector2D& playerPos, const FVector2D& playerSize);

	class USpriteRenderer* SpriteRenderer;


protected:

private:
	int MySpriteIndex = 0;


};

