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

ABall::ABall()
{
    SetActorLocation({ 385,850 });

	SpriteRenderer = CreateDefaultSubObject<USpriteRenderer>();
	SpriteRenderer->SetSprite("ball_red.png");
	SpriteRenderer->SetComponentScale({ 20, 20 });
	SpriteRenderer->SetOrder(ERenderOrder::UI);
    IsMove = false;
    SavePos = GetActorLocation();
}


void ABall::BeginPlay()
{

	Super::BeginPlay();

    {
        Fade = GetWorld()->SpawnActor<AFade>();
        Fade->GetBackSpriteRenderer()->SetAlphafloat(0.f);
    }

}

void ABall::Tick(float _DeltaTime)
{
    Super::Tick(_DeltaTime);

    SavePos = GetActorLocation();



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


//void ABall::UpdatePosition(float deltaTime)
//{
//    if (IsMove == true)
//    {
//        // 벽 충돌 체크 및 속도 반사
//        int MaxTop = 101, MaxBottom = 1000, MaxLeft = 49, MaxRight = 722;
//        FVector2D ballPos = GetActorLocation();
//        FVector2D ballScale = GetRender()->GetComponentScale();
//        float tolerance = 0.15f + ballScale.X / 2;
//
//        bool hasCollided = false; // 충돌 여부
//
//        // 왼쪽 벽에 닿았다
//        if (MaxLeft >= ballPos.X && !hasCollided)
//        {
//            Reflect({ 1.f, 0.01f });
//            ballPos.X = MaxLeft + 0.3f; // 벽에서 약간 떨어지게 위치 조정
//            hasCollided = true;
//            return;
//        }
//
//        // 오른쪽 벽에 닿았다
//        if (MaxRight <= ballPos.X && !hasCollided)
//        {
//            Reflect({ -1.f, -0.01f });
//            ballPos.X = MaxRight - 0.3f;
//            hasCollided = true;
//            return;
//        }
//
//        // 아랫벽에 닿았다
//        if (MaxBottom <= ballPos.Y && !hasCollided)
//        {
//            {
//                Reflect({ 0.01f, 1.f });
//                ballPos.Y = MaxBottom - 0.3f;
//                hasCollided = true;
//                return;
//            }
//            {
//                //SpriteRenderer->SetActive(false);
//                //SavePos = GetActorLocation();
//                //SetActorLocation({0,0});// Fade 위치때문에.
//                //IsMove = false;
//
//                //Fade->FadeIn();
//                //FadeOver = false;
//            }
//
//        }
//
//        // 위쪽 벽에 닿았다
//        if (MaxTop > ballPos.Y && !hasCollided)
//        {
//            Reflect({ -0.01f, -1.f });
//            ballPos.Y = MaxTop + 0.3f;
//            hasCollided = true;
//            return;
//        }
//
//        // 위치 업데이트
//        ballPos += Value * deltaTime;
//
//    }
//}

void ABall::UpdatePosition(float deltaTime)
{
    if (IsMove)
    {
        // 벽 충돌 체크 및 속도 반사
        int MaxTop = 101, MaxBottom = 1000, MaxLeft = 49, MaxRight = 722;
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
            Reflect({ 0.f, -1.f });
            ballPos.Y = MaxBottom - ballScale.Y;
            hasCollided = true;
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
