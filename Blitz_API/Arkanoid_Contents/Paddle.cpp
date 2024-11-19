#include "PreCompiledFile.h"
#include "Paddle.h"

#include <EnginePlatform/EngineInput.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/EngineAPICore.h>
#include <EngineCore/EngineCoreDebug.h>
#include "ContentsEnum.h"

APaddle* APaddle::MainPaddle = nullptr;

APaddle::APaddle()
{
	MainPaddle = this;
	{
	// static sprite
	SpriteRenderer = CreateDefaultSubObject<USpriteRenderer>();
	SpriteRenderer->SetOrder(ERenderOrder::Bricks);
	SpriteRenderer->SetSprite("paddle_small.png");
	PaddleScale = SpriteRenderer->SetSpriteScale(1.0f);

	//SpriteRenderer->SetAlphafloat(static_cast<float>(0.2f));//SetAlphafloat �׽�Ʈ

	}
	Vel = { 0.f,0.f };
}


void APaddle::Tick(float _DeltaTime)
{

	// �� �浹 üũ �� �ӵ� �ݻ�
	int MaxTop = 101, MaxBottom = 980, MaxLeft = 46, MaxRight = 725;
	if ((GetActorLocation().X - PaddleScale.X/2) <= MaxLeft)
	{
		AddActorLocation(FVector2D::RIGHT * _DeltaTime * Speed);
	}
	if ((GetActorLocation().X + PaddleScale.X/2 )>= MaxRight)
	{
		AddActorLocation(FVector2D::LEFT * _DeltaTime * Speed);
	}


	if (true == UEngineInput::GetInst().IsPress('D'))
	{
		AddActorLocation(FVector2D::RIGHT * _DeltaTime * Speed);
		Vel = (FVector2D::RIGHT * _DeltaTime * Speed);
		UEngineDebug::CoreOutPutString("Vel : " + std::to_string(Vel.X));
	}
	if (true == UEngineInput::GetInst().IsPress('A'))
	{
		AddActorLocation(FVector2D::LEFT * _DeltaTime * Speed);
		Vel = (FVector2D::LEFT * _DeltaTime * Speed);
		UEngineDebug::CoreOutPutString("Vel : " + std::to_string(Vel.X));

	}

}


FVector2D APaddle::CheckCollision(const FVector2D& ballPos)
{
	FVector2D paddlesize = SpriteRenderer->GetComponentScale();
	FVector2D paddlePos(GetActorLocation().X - paddlesize.X/2, GetActorLocation().Y - paddlesize.Y);
	FVector2D HitResult = (ballPos - paddlePos) / paddlesize;

	if (HitResult.X > 0 && HitResult.Y > 0 && HitResult.X < 1 && HitResult.Y < 1) {
		if (HitResult.X < HitResult.Y) {
			if (HitResult.X > 1 - HitResult.Y) {
				return { 0.f, -1.f };  // �Ʒ��� �ݻ�
			}
			else {
				return { -1.f, -0.7f };  // ���� �ݻ�
			}
		}
		else if (HitResult.X > HitResult.Y) {
			if (HitResult.Y > 1 - HitResult.X) {
				return { 1.f, -0.7f };  // ������ �ݻ�
			}
		}
	}
	return { 0, 0 };  // �浹���� ���� ���
}
