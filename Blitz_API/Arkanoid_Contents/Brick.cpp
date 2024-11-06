#include "PreCompiledFile.h"
#include "Brick.h"

#include <EnginePlatform/EngineInput.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/EngineAPICore.h>
#include <EngineCore/EngineCoreDebug.h>
#include "ContentsEnum.h"


Brick::Brick()
{
	{
		SpriteRenderer = CreateDefaultSubObject<USpriteRenderer>();
		SpriteRenderer->SetOrder(ERenderOrder::Bricks);
		SpriteRenderer->SetSprite("brick_white.png");

		FVector2D BrickScale = SpriteRenderer->SetSpriteScale(0.85f);
		SpriteRenderer->SetComponentLocation(BrickScale.Half());
        SpriteRenderer->SetOrder(ERenderOrder::NEW);

		FVector2D WinSize = UEngineAPICore::GetCore()->GetMainWindow().GetWindowSize().Half();
		SetActorLocation(WinSize);
	}
}

Brick::~Brick()
{
}

FVector2D Brick::CheckCollision(const FVector2D& playerPos, const FVector2D& playerSize)
{
    FVector2D brickPos = GetTransform().Location;
    FVector2D brickSize = GetRender()->GetComponentScale();
    FVector2D HitResult = (playerPos - brickPos) / brickSize;

    if (HitResult.X > 0 && HitResult.Y > 0 && HitResult.X < 1 && HitResult.Y < 1) {
        if (HitResult.X < HitResult.Y) {
            if (HitResult.X > 1 - HitResult.Y) {
                UEngineDebug::CoreOutPutString("HitResult : BOTTOM" + HitResult.ToString(), { 100, 600 });
                return FVector2D(0.0f, -1.0f); // 아래쪽 반사 벡터
            }
            else {
                UEngineDebug::CoreOutPutString("HitResult : LEFT" + HitResult.ToString(), { 100, 600 });
                return FVector2D(-1.0f, 0.0f); // 왼쪽 반사 벡터
            }
        }
        else if(HitResult.X > HitResult.Y)
        {
            if (HitResult.Y > 1 - HitResult.X) {
                UEngineDebug::CoreOutPutString("HitResult : RIGHT" + HitResult.ToString(), { 100, 600 });
                return FVector2D(1.0f, 0.0f); // 오른쪽 반사 벡터
            }
            else {
                UEngineDebug::CoreOutPutString("HitResult : TOP" + HitResult.ToString(), { 100, 600 });
                return FVector2D(0.0f, 1.0f); // 위쪽 반사 벡터
            }
        }
    }
    return FVector2D(0.0f, 0.0f); // 충돌 없음
}


