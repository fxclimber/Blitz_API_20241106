#include "PreCompiledFile.h"
#include "Ball.h"
#include <EngineCore/EngineAPICore.h>
#include <EnginePlatform/EngineWinImage.h>
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineBase/EngineDebug.h>
#include <EngineCore/EngineCoreDebug.h>

#include "ContentsEnum.h"


ABall::ABall()
{
    SetActorLocation({ 400,800 });

	SpriteRenderer = CreateDefaultSubObject<USpriteRenderer>();
	SpriteRenderer->SetSprite("ball_red.png");
	SpriteRenderer->SetComponentScale({ 22, 22 });
	SpriteRenderer->SetOrder(ERenderOrder::UI);

    // 상수 세팅은 좋지 않다.
}

ABall::~ABall()
{
}

void ABall::BeginPlay()
{
	Super::BeginPlay();
}

void ABall::Tick(float _DeltaTime)
{
    // 벽 충돌 체크 및 속도 반사
    int MaxTop = 101, MaxBottom = 980, MaxLeft = 46, MaxRight = 725;
    FVector2D ballPos = GetActorLocation();
    FVector2D ballScale = GetRender()->GetComponentScale();
    float tolerance = 0.02f + ballScale.X / 2;
    
    // 왼쪽벽에 닿았다
    if (MaxLeft > ballPos.X)
    {
        Reflect({1, 0});
    }

    // 오른쪽 벽에 닿았다.
    if (MaxRight < ballPos.X)
    {
        Reflect({ -1, 0 });
    }

    // 아랫벽에 닿았다.
    if (MaxBottom < ballPos.Y)
    {
        Reflect({ 0, 1 });
    }

    // 위쪽벽에 닿았다.
    if (MaxTop > ballPos.Y)
    {
        Reflect({ 0, -1 });
    }


    //if (MaxLeft < ballPos.X && ballPos.X < MaxRight && MaxTop < ballPos.Y && ballPos.Y < MaxBottom) {
    //    if (ballPos.X <= MaxLeft + tolerance || ballPos.X >= MaxRight - tolerance) {
    //        Value.X *= -1.0f;
    //    }
    //    if (ballPos.Y <= MaxTop + tolerance || ballPos.Y >= MaxBottom - tolerance) {
    //        Value.Y *= -1.0f;
    //    }
    //}

    MoveFunction(Value);
    //UEngineDebug::CoreOutPutString("ballVel : " + Value.ToString(), { 100, 200 });

}

void ABall::MoveFunction(const FVector2D& velocity)
{
    float DeltaTime = UEngineAPICore::GetCore()->GetDeltaTime();
    //SetActorLocation(GetActorLocation() + velocity * DeltaTime);
    AddActorLocation(velocity * DeltaTime * Speed);
}

void ABall::Reflect(const FVector2D& normal)
{
    float dotProduct = Value.Dot(normal);
    Value = Value - normal * (2 * dotProduct);
}


//void ABall::MoveFunction(FVector2D _Dir)
//{
//	float DeltaTime = UEngineAPICore::GetCore()->GetDeltaTime();
//	AddActorLocation(_Dir * DeltaTime * Speed);
//}
//
