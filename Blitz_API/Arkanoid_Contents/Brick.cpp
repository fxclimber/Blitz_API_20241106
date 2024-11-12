#include "PreCompiledFile.h"
#include "Brick.h"

#include <EngineBase/EngineRandom.h>
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/EngineAPICore.h>
#include <EngineCore/EngineCoreDebug.h>
#include "ContentsEnum.h"
#include <thread>
#include <chrono>

Brick::Brick()
{
	SpriteRenderer = CreateDefaultSubObject<USpriteRenderer>();
    MakeBonusRenderer();
}

Brick::~Brick()
{
}

void Brick::BeginPlay()
{
}

void Brick::Tick(float _DeltaTime)
{
	AddActorLocation(Value * _DeltaTime * Speed);
}


//FVector2D Brick::CheckCollision(const FVector2D& playerPos, const FVector2D& playerSize)
//{
//    FVector2D brickPos = GetActorLocation();
//    FVector2D brickSize = GetRender()->GetComponentScale();
//    FVector2D HitResult = (playerPos - brickPos) / brickSize;
//
//    if (HitResult.X > 0 && HitResult.Y > 0 && HitResult.X < 1 && HitResult.Y < 1) {
//        if (HitResult.X < HitResult.Y) {
//            if (HitResult.X > 1 - HitResult.Y) {
//                UEngineDebug::CoreOutPutString("HitResult : BOTTOM" + HitResult.ToString(), { 100, 600 });
//                return FVector2D(0.0f, -1.0f); // 아래쪽 반사 벡터
//            }
//            else {
//                UEngineDebug::CoreOutPutString("HitResult : LEFT" + HitResult.ToString(), { 100, 600 });
//                return FVector2D(-1.0f, 0.0f); // 왼쪽 반사 벡터
//            }
//        }
//        else if(HitResult.X > HitResult.Y)
//        {
//            if (HitResult.Y > 1 - HitResult.X) {
//                UEngineDebug::CoreOutPutString("HitResult : RIGHT" + HitResult.ToString(), { 100, 600 });
//                return FVector2D(1.0f, 0.0f); // 오른쪽 반사 벡터
//            }
//            else {
//                UEngineDebug::CoreOutPutString("HitResult : TOP" + HitResult.ToString(), { 100, 600 });
//                return FVector2D(0.0f, 1.0f); // 위쪽 반사 벡터
//            }
//        }
//    }
//    return FVector2D(0.0f, 0.0f); // 충돌 없음
//}

BonusType Brick::GetRandomBonusType()
{
    UEngineRandom Random;
    int Value = Random.RandomInt(0, 6);
    UEngineDebug::CoreOutPutString("Value : " + Value , {200,300});
    return static_cast<BonusType>(Value);
}

void Brick::MakeBonus()
{
        BonusType randomBonus = GetRandomBonusType();

         //BonusType따라 다른 스프라이트 세팅
        switch (randomBonus)
        {
        case BonusType::LIFE:
            SpriteRenderer->SetSprite("powerup_life");
            SpriteRenderer->SetComponentLocation(GetActorLocation());
            SpriteRenderer->CreateAnimation("powerup_life", "powerup_life", 1, 7, 0.1f, true);
            SpriteRenderer->ChangeAnimation("powerup_life");
            spriteScale = SpriteRenderer->SetSpriteScale(1.4f);
            SpriteRenderer->SetComponentLocation({ spriteScale.X / 2, 0.f });
            break;
        case BonusType::COPY:
            SpriteRenderer->SetSprite("powerup_duplicate");
            SpriteRenderer->SetComponentLocation(GetActorLocation());
            SpriteRenderer->CreateAnimation("powerup_duplicate", "powerup_duplicate", 1, 7, 0.1f, true);
            SpriteRenderer->ChangeAnimation("powerup_duplicate");
            spriteScale = SpriteRenderer->SetSpriteScale(1.4f);
            SpriteRenderer->SetComponentLocation({ spriteScale.X / 2, 0.f });
            break;
        case BonusType::SLOW:
            SpriteRenderer->SetSprite("powerup_slow");
            SpriteRenderer->SetComponentLocation(GetActorLocation());
            SpriteRenderer->CreateAnimation("powerup_slow", "powerup_slow", 1, 7, 0.1f, true);
            SpriteRenderer->ChangeAnimation("powerup_slow");
            spriteScale = SpriteRenderer->SetSpriteScale(1.4f);
            SpriteRenderer->SetComponentLocation({ spriteScale.X / 2, 0.f });

            break;
        case BonusType::LASER:
            SpriteRenderer->SetSprite("powerup_laser");
            SpriteRenderer->SetComponentLocation(GetActorLocation());
            SpriteRenderer->CreateAnimation("powerup_laser", "powerup_laser", 1, 7, 0.1f, true);
            SpriteRenderer->ChangeAnimation("powerup_laser");
            spriteScale = SpriteRenderer->SetSpriteScale(1.4f);
            SpriteRenderer->SetComponentLocation({ spriteScale.X / 2, 0.f });
            break;
        case BonusType::EXPAND:
            SpriteRenderer->SetSprite("powerup_expand");
            SpriteRenderer->SetComponentLocation(GetActorLocation());
            SpriteRenderer->CreateAnimation("powerup_expand", "powerup_expand", 1, 7, 0.1f, true);
            SpriteRenderer->ChangeAnimation("powerup_expand");
            spriteScale = SpriteRenderer->SetSpriteScale(1.4f);
            SpriteRenderer->SetComponentLocation({ spriteScale.X / 2, 0.f });
            break;
        case BonusType::CATCH:
            SpriteRenderer->SetSprite("powerup_catch");
            SpriteRenderer->SetComponentLocation(GetActorLocation());
            SpriteRenderer->CreateAnimation("powerup_catch", "powerup_catch", 1, 7, 0.1f, true);
            SpriteRenderer->ChangeAnimation("powerup_catch");
            spriteScale = SpriteRenderer->SetSpriteScale(1.4f);
            SpriteRenderer->SetComponentLocation({ spriteScale.X / 2, 0.f });
            break;
        case BonusType::WARP:
            SpriteRenderer->SetSprite("powerup_warp");
            SpriteRenderer->SetComponentLocation(GetActorLocation());
            SpriteRenderer->CreateAnimation("powerup_warp", "powerup_warp", 1, 7, 0.1f, true);
            SpriteRenderer->ChangeAnimation("powerup_warp");
            spriteScale = SpriteRenderer->SetSpriteScale(1.4f);
            SpriteRenderer->SetComponentLocation({ spriteScale.X / 2, 0.f });
            break;
        default:
        {
            int a = 0;
            break;
        }
        }
}

void Brick::MakeBonusRenderer()
{
    SetSpeed(200.f);
    SetDir({ 0,1 });

    MakeBonus();
    SpriteRenderer->SetOrder(ERenderOrder::UI);

}
