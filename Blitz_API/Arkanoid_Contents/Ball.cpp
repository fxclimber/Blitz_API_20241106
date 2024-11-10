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
    SetActorLocation({ 385,850 });

	SpriteRenderer = CreateDefaultSubObject<USpriteRenderer>();
	SpriteRenderer->SetSprite("ball_red.png");
	SpriteRenderer->SetComponentScale({ 22, 22 });
	SpriteRenderer->SetOrder(ERenderOrder::UI);
    IsMove = false;
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
    // �� �浹 üũ �� �ӵ� �ݻ�
    int MaxTop = 101, MaxBottom = 980, MaxLeft = 46, MaxRight = 725;
    FVector2D ballPos = GetActorLocation();
    FVector2D ballScale = GetRender()->GetComponentScale();
    float tolerance = 0.02f + ballScale.X / 2;
    if (true == GetIsMove())
    {
        UpdatePosition(_DeltaTime);
        MoveFunction(Value);
    }
    else
    {
        MoveFunction({0,0});
    }
}

void ABall::MoveFunction(const FVector2D& velocity)
{
    float DeltaTime = UEngineAPICore::GetCore()->GetDeltaTime();
    AddActorLocation(velocity * DeltaTime * Speed);
}

void ABall::Reflect(const FVector2D& normal)
{
    float dotProduct = Value.Dot(normal);
    Value = Value - (normal * (2.0f * dotProduct));
}


void ABall::UpdatePosition(float deltaTime)
{

    // �� �浹 üũ �� �ӵ� �ݻ�
    int MaxTop = 101, MaxBottom = 999, MaxLeft = 46, MaxRight = 725;
    FVector2D ballPos = GetActorLocation();
    FVector2D ballScale = GetRender()->GetComponentScale();
    float tolerance = 0.02f + ballScale.X / 2;

    bool hasCollided = false; // �浹 ����

    // ���� ���� ��Ҵ�
    if (MaxLeft > ballPos.X && !hasCollided)
    {
        Reflect({ 1, 0 });
        ballPos.X = MaxLeft + 0.1f; // ������ �ణ �������� ��ġ ����
        hasCollided = true;
    }

    // ������ ���� ��Ҵ�
    if (MaxRight < ballPos.X && !hasCollided)
    {
        Reflect({ -1, 0 });
        ballPos.X = MaxRight - 0.1f;
        hasCollided = true;
    }

    // �Ʒ����� ��Ҵ�
    if (MaxBottom < ballPos.Y && !hasCollided)
    {
        Reflect({ 0, 1 });
        ballPos.Y = MaxBottom - 0.1f;
        hasCollided = true;
    }

    // ���� ���� ��Ҵ�
    if (MaxTop > ballPos.Y && !hasCollided)
    {
        Reflect({ 0, -1 });
        ballPos.Y = MaxTop + 0.1f;
        hasCollided = true;
    }

    // ��ġ ������Ʈ
    ballPos += Value * deltaTime;
}

