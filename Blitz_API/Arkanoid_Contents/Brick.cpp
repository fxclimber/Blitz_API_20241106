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
#include <EngineBase/TimeEvent.h>

#include "Paddle.h"
#include "TestGameMode.h"


Brick::Brick()
{
	SpriteRenderer = CreateDefaultSubObject<USpriteRenderer>();
    MakeBonusRenderer();

    
}


void Brick::BeginPlay()
{
}

void Brick::Tick(float _DeltaTime)
{
    Super::Tick(_DeltaTime);

	AddActorLocation(Value * _DeltaTime * Speed);

    
    FTransform TransLeft = SpriteRenderer->GetActorTransform();
    FTransform TransRight = APaddle::MainPaddle->GetSpriteRenderer()->GetActorTransform();

    if (spriteName == "powerup_duplicate")
    {
        if (FTransform::RectToRect(TransLeft, TransRight))
        {
            if (nullptr != this)
            {
                ATestGameMode* GameMode = GetWorld()->GetGameMode<ATestGameMode>();
                GameMode->SpawnBall();
                Destroy();
                //TimeEvent();
            }
        }
    }
}



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

    // 각 BonusType에 따른 스프라이트 이름
    //std::string spriteName;
    switch (randomBonus)
    {
    case BonusType::LIFE:     spriteName = "powerup_life";      break;
    case BonusType::COPY:     spriteName = "powerup_duplicate"; break;
    case BonusType::SLOW:     spriteName = "powerup_slow";      break;
    case BonusType::LASER:    spriteName = "powerup_laser";     break;
    case BonusType::EXPAND:   spriteName = "powerup_expand";    break;
    case BonusType::CATCH:    spriteName = "powerup_catch";     break;
    case BonusType::WARP:     spriteName = "powerup_warp";      break;
    default:                  return; // 기본 케이스에서 종료
    }

    // 공통 설정 수행
    SpriteRenderer->SetSprite(spriteName);
    SpriteRenderer->SetComponentLocation(GetActorLocation());
    SpriteRenderer->CreateAnimation(spriteName, spriteName, 1, 7, 0.1f, true);
    SpriteRenderer->ChangeAnimation(spriteName);

    // 스프라이트 크기 및 위치 설정
    spriteScale = SpriteRenderer->SetSpriteScale(1.2f);
    SpriteRenderer->SetComponentLocation({ spriteScale.X / 2, 0.f });
}

void Brick::MakeBonusRenderer()
{
    SetSpeed(200.f);
    SetDir({ 0,1 });

    MakeBonus();
    SpriteRenderer->SetOrder(ERenderOrder::UI);

}

void Brick::TimeEvent()
{
    ATestGameMode* GameMode = GetWorld()->GetGameMode<ATestGameMode>();
    std::vector<ABall*> Balls = GameMode->Balls;
    
    ABall* BallInstance = Balls[0] ;
        BallInstance->Destroy(1.f);
}


