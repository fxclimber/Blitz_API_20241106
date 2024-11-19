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

#include "Paddle.h"


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

    if (FTransform::RectToRect(TransLeft, TransRight))
    {
        if (nullptr != this)
        {
            SpriteRenderer->SetActive(false);
            //Destroy();
            int a = 0;
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

    // �� BonusType�� ���� ��������Ʈ �̸�
    std::string spriteName;
    switch (randomBonus)
    {
    case BonusType::LIFE:     spriteName = "powerup_life";      break;
    case BonusType::COPY:     spriteName = "powerup_duplicate"; break;
    case BonusType::SLOW:     spriteName = "powerup_slow";      break;
    case BonusType::LASER:    spriteName = "powerup_laser";     break;
    case BonusType::EXPAND:   spriteName = "powerup_expand";    break;
    case BonusType::CATCH:    spriteName = "powerup_catch";     break;
    case BonusType::WARP:     spriteName = "powerup_warp";      break;
    default:                  return; // �⺻ ���̽����� ����
    }

    // ���� ���� ����
    SpriteRenderer->SetSprite(spriteName);
    SpriteRenderer->SetComponentLocation(GetActorLocation());
    SpriteRenderer->CreateAnimation(spriteName, spriteName, 1, 7, 0.1f, true);
    SpriteRenderer->ChangeAnimation(spriteName);

    // ��������Ʈ ũ�� �� ��ġ ����
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
