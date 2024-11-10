#include "PreCompiledFile.h"
#include "Paddle.h"

#include <EnginePlatform/EngineInput.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/EngineAPICore.h>
#include <EngineCore/EngineCoreDebug.h>
#include "ContentsEnum.h"


APaddle::APaddle()
{
	//SetActorLocation({ 370,920 });

	{
		//SpriteRenderer = CreateDefaultSubObject<USpriteRenderer>();		
		//SpriteRenderer->SetOrder(ERenderOrder::Bricks);

		//SpriteRenderer->CreateAnimation("paddle_materialize", "paddle_materialize", 0, 14, 0.12f);
		//SpriteRenderer->ChangeAnimation("paddle_materialize");
		//SpriteRenderer->SetComponentScale({ 125, 34 });
	}

	this;
	{
	// static sprite
	SpriteRenderer = CreateDefaultSubObject<USpriteRenderer>();
	SpriteRenderer->SetOrder(ERenderOrder::Bricks);
	SpriteRenderer->SetSprite("paddle_small.png");
	PaddleScale = SpriteRenderer->SetSpriteScale(1.0f);
	}
}

APaddle::~APaddle()
{
}

void APaddle::BeginPlay()
{
}

void APaddle::Tick(float _DeltaTime)
{
	if (true == UEngineInput::GetInst().IsPress('D'))
	{
		AddActorLocation(FVector2D::RIGHT * _DeltaTime * Speed);
	}
	if (true == UEngineInput::GetInst().IsPress('A'))
	{
		AddActorLocation(FVector2D::LEFT * _DeltaTime * Speed);
	}
}

void APaddle::MoveFunction(FVector2D _Dir)
{
}

void APaddle::RunSoundPlay()
{
}

//
//FVector2D APaddle::CheckCollision(ABall* Ball)
//{
//	FVector2D Normal = CheckCollision(Ball->GetActorLocation(), Ball->GetRender()->GetComponentScale());
//	Ball->Reflect(Normal);
//	return Normal;
//}
//
//
//FVector2D APaddle::CheckCollision(const FVector2D& ballPos, const FVector2D& ballSize)
//{
//    FVector2D paddlePos = GetActorLocation();
//    FVector2D paddlesize = SpriteRenderer->GetComponentScale();
//    FVector2D HitResult = (ballPos - paddlePos) / paddlesize;
//
//    if (HitResult.X > 0 && HitResult.Y > 0 && HitResult.X < 1 && HitResult.Y < 1) {
//        if (HitResult.X < HitResult.Y) {
//            if (HitResult.X > 1 - HitResult.Y) {
//                return Reflect({ 0, -1 });
//            }
//            else {
//                return Reflect({ -1, 0 });
//            }
//        }
//        else if(HitResult.X > HitResult.Y)
//        {
//            if (HitResult.Y > 1 - HitResult.X) {
//                return Reflect({ 1, 0 });
//            }
//            else {
//                return Reflect({ 0, 1 });
//            }
//        }
//    }
//}

FVector2D APaddle::CheckCollision(const FVector2D& ballPos)
{
	FVector2D paddlesize = SpriteRenderer->GetComponentScale();
	FVector2D paddlePos(GetActorLocation().X - paddlesize.X/2, GetActorLocation().Y - paddlesize.Y);
	FVector2D HitResult = (ballPos - paddlePos) / paddlesize;

	if (HitResult.X > 0 && HitResult.Y > 0 && HitResult.X < 1 && HitResult.Y < 1) {
		if (HitResult.X < HitResult.Y) {
			if (HitResult.X > 1 - HitResult.Y) {
				return { 0.f, -1.f };  // 아래쪽 반사
			}
			else {
				return { -1.f, -0.7f };  // 왼쪽 반사
			}
		}
		else if (HitResult.X > HitResult.Y) {
			if (HitResult.Y > 1 - HitResult.X) {
				return { 1.f, -0.7f };  // 오른쪽 반사
			}
		}
	}
	return { 0, 0 };  // 충돌하지 않은 경우
}
