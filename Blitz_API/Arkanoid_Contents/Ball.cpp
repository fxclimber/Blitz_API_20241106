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



    // �� �浹 üũ �� �ӵ� �ݻ�
    int MaxTop = 101, MaxBottom = 980, MaxLeft = 46, MaxRight = 725;
    FVector2D ballPos = GetActorLocation();
    FVector2D ballScale = GetRender()->GetComponentScale();
    float tolerance = 0.02f + ballScale.X / 2;

    if (true==GetIsMove())
    {
        UpdatePosition(_DeltaTime);
        MoveFunction(Value);
    }
    else if(false == GetIsMove())
    {
        MoveFunction({ 0, 0 });
    }

    if (false == GetIsMove())
    {
        float a = Fade->GetBackSpriteRenderer()->GetAlpha();

        if (a>0.99f)
        {
            SetActorLocation(SavePos);
            Fade->FadeOut();
            FadeOver = true;
            SpriteRenderer->SetActive(true);
        }
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
    if (IsMove == true)
    {
        // �� �浹 üũ �� �ӵ� �ݻ�
        int MaxTop = 101, MaxBottom = 1000, MaxLeft = 49, MaxRight = 722;
        FVector2D ballPos = GetActorLocation();
        FVector2D ballScale = GetRender()->GetComponentScale();
        float tolerance = 0.05f + ballScale.X / 2;

        bool hasCollided = false; // �浹 ����

        // ���� ���� ��Ҵ�
        if (MaxLeft >= ballPos.X && !hasCollided)
        {
            Reflect({ 1.f, 0.05f });
            ballPos.X = MaxLeft + 0.1f; // ������ �ణ �������� ��ġ ����
            hasCollided = true;
        }

        // ������ ���� ��Ҵ�
        if (MaxRight <= ballPos.X && !hasCollided)
        {
            Reflect({ -1.f, -0.05f });
            ballPos.X = MaxRight - 0.1f;
            hasCollided = true;
        }

        // �Ʒ����� ��Ҵ�
        if (MaxBottom <= ballPos.Y && !hasCollided)
        {
            {
                SpriteRenderer->SetActive(false);
                SavePos = GetActorLocation();
                SetActorLocation({0,0});
                IsMove = false;

                Fade->FadeIn();
                FadeOver = false;


            }
            {
                //Reflect({ 0.03f, 1.f });
                //ballPos.Y = MaxBottom - 0.1f;
                //hasCollided = true;
            }

            return;
        }

        // ���� ���� ��Ҵ�
        if (MaxTop >= ballPos.Y && !hasCollided)
        {
            Reflect({ -0.03f, -1.f });
            ballPos.Y = MaxTop + 0.1f;
            hasCollided = true;
        }

        // ��ġ ������Ʈ
        ballPos += Value * deltaTime;

    }
}

