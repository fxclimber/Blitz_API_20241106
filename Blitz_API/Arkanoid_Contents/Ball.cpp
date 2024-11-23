#include "PreCompiledFile.h"
#include "Ball.h"
#include <EngineCore/EngineAPICore.h>
#include <EnginePlatform/EngineWinImage.h>
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineBase/EngineDebug.h>
#include <EngineCore/EngineCoreDebug.h>
//#include <chrono>
#include <EngineBase/EngineTimer.h>
#include "ContentsEnum.h"
#include "Fade.h"
#include <EngineBase/EngineMath.h>

#include "TestGameMode.h"
#include "Map_Bottom.h"

//BallType ABall::ballType = BallType::Basic;
//BallType ballType = Basic;

ABall::ABall()
{
    SetActorLocation({ 385,850 });
	SpriteRenderer = CreateDefaultSubObject<USpriteRenderer>();
	SpriteRenderer->SetSprite("ball_red.png");
	SpriteRenderer->SetComponentScale({ 20, 20 });
	SpriteRenderer->SetOrder(ERenderOrder::UI);
    IsMove = false;
    SavePos = GetActorLocation();
    ballType = BallType::Basic;

}


void ABall::BeginPlay()
{

	Super::BeginPlay();

    {
        Fade = GetWorld()->SpawnActor<AFade>();
        Fade->GetBackSpriteRenderer()->SetAlphafloat(0.f);
    }
    gamemode = GetWorld()->GetGameMode<ATestGameMode>();
}

void ABall::Tick(float _DeltaTime)
{
    Super::Tick(_DeltaTime);

    SavePos = GetActorLocation();
    // 일반 다이나믹캐스트하려면
    // gamemode = dynamic_cast<ATestGameMode*>(GetWorld()->GetGameMode());



    if (nullptr == gamemode)
    {
        return;
    }
    bottom = nullptr;
    bottom = gamemode->GetBottom();
    bottomSpriteRender = bottom->GetRender();

    // 벽 충돌 체크 및 속도 반사
    int MaxTop = 101, MaxBottom = 1000, MaxLeft = 49, MaxRight = 722;
    FVector2D ballPos = GetActorLocation();
    FVector2D ballScale = GetRender()->GetComponentScale();
    //float tolerance = 0.02f + ballScale.X / 2;

    if (true==GetIsMove())
    {
        UpdatePosition(_DeltaTime);
        MoveFunction(Value);
    }
    else if(false == GetIsMove())
    {
        float a = Fade->GetBackSpriteRenderer()->GetAlpha();

        if (a > 0.99f)
        {
            SetActorLocation(SavePos);
            Fade->FadeOut();
            FadeOver = true;
            SpriteRenderer->SetActive(true);
        }

        MoveFunction({ 0, 0 });
    }


}

void ABall::MoveFunction(const FVector2D& _dir)
{
    float DeltaTime = UEngineAPICore::GetCore()->GetDeltaTime();
    AddActorLocation(_dir * DeltaTime * Speed);
}

void ABall::Reflect(const FVector2D& normal)
{
    float dotProduct = Value.Dot(normal);
    Value = Value - (normal * (2.0f * dotProduct));
    Value.Normalize();
}


void ABall::UpdatePosition(float deltaTime)
{
    if (IsMove)
    {
        // 벽 충돌 체크 및 속도 반사
        int MaxTop = 101, MaxBottom = 960, MaxLeft = 49, MaxRight = 722;
        FVector2D ballPos = GetActorLocation();
        FVector2D ballScale = GetRender()->GetComponentScale();
        float tolerance = 0.15f + ballScale.X / 2;
        bool hasCollided = false;

        // 왼쪽 벽에 닿았다
        if (MaxLeft > ballPos.X)
        {
            Reflect({ 1.f, 0.f });
            ballPos.X = MaxLeft + ballScale.X;
            hasCollided = true;
        }

        // 오른쪽 벽에 닿았다
        else if (MaxRight < ballPos.X)
        {
            Reflect({ -1.f, 0.f });
            ballPos.X = MaxRight - ballScale.X;
            hasCollided = true;
        }

        // 아랫벽에 닿았다
        else if (MaxBottom < ballPos.Y)
        {

            if (false==bottomSpriteRender->IsActive())
            {
                if (ballType == BallType::Basic)
                {
                    SpriteRenderer->SetActive(false);
                    SavePos = GetActorLocation();
                    SetActorLocation({ 0,0 });// Fade 위치때문에.
                    IsMove = false;

                    Fade->FadeIn();
                    FadeOver = false;
                }
                else if (ballType == BallType::Bonus)
                {
                    return;
                }
            }
            else
            {
                Reflect({ 0.f, -1.f });
                ballPos.Y = MaxBottom - ballScale.Y;
                hasCollided = true;
            }


        }


        // 위쪽 벽에 닿았다
        else if (MaxTop > ballPos.Y)
        {
            Reflect({ 0.f, 1.f });
            ballPos.Y = MaxTop + ballScale.Y;
            hasCollided = true;
        }

        if (hasCollided)
        {
            SetActorLocation(ballPos); // 위치 보정
            float max = sqrt(Value.X)+sqrt(Value.Y);
            if (max < 0.f)
            {
                Value = {0.1f,0.1f};
            }
            return;
        }

        // 위치 업데이트
        ballPos += Value * deltaTime;
        SetActorLocation(ballPos);
    }
}
