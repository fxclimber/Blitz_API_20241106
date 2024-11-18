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
    //AFade* MainFade = GetWorld()->SpawnActor<AFade>();
    //MainFade->LevelChangeStart();
    //MainFade->FadeIn();
    //MainFade->FadeOut();
}

void ABall::Tick(float _DeltaTime)
{
    Super::Tick(_DeltaTime);

    SavePos = GetActorLocation();

    //if (true == UEngineInput::GetInst().IsDown('U'))
    //{
    //    AFade::MainFade->FadeIn();
    //}

    //if (true == UEngineInput::GetInst().IsDown('Y'))
    //{
    //    AFade::MainFade->FadeOut();
    //}





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

        // DeltaTime ����
        EndTime += _DeltaTime;  // ����� ��� ����

        if (EndTime > 20.f)
        {
            // ���� ���� �� �ʱ�ȭ
            //Value = { 0, -1 };
            //IsMove = true;  // ���� ����
            //EndTime = 0.0f; // Ÿ�̸� �ʱ�ȭ
            //SetActorLocation(SavePos);  // ��ġ ����
            //SpriteRenderer->SetActive(true);  // ��������Ʈ Ȱ��ȭ
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
        int MaxTop = 101, MaxBottom = 1000, MaxLeft = 46, MaxRight = 725;
        FVector2D ballPos = GetActorLocation();
        FVector2D ballScale = GetRender()->GetComponentScale();
        float tolerance = 0.02f + ballScale.X / 2;

        bool hasCollided = false; // �浹 ����

        // ���� ���� ��Ҵ�
        if (MaxLeft >= ballPos.X && !hasCollided)
        {
            Reflect({ 1, 0 });
            ballPos.X = MaxLeft + 0.1f; // ������ �ణ �������� ��ġ ����
            hasCollided = true;
        }

        // ������ ���� ��Ҵ�
        if (MaxRight <= ballPos.X && !hasCollided)
        {
            Reflect({ -1, 0 });
            ballPos.X = MaxRight - 0.1f;
            hasCollided = true;
        }

        // �Ʒ����� ��Ҵ�
        if (MaxBottom <= ballPos.Y && !hasCollided)
        {
            //Destroy();
            //Reflect({0.f,0.1f});
            //IsMove = false;
            //SetIsMove(false);

            //DeltaTimer.TimeCheck();
            //float DeltaTime = DeltaTimer.GetDeltaTime();

            //EndTime = 0.f;

            Value = { 0,0 };
            SpriteRenderer->SetActive(false);
            SavePos = GetActorLocation();
            SetActorLocation({0,0});


            //StopTime = std::chrono::steady_clock::now();

            SpriteRenderer = CreateDefaultSubObject<USpriteRenderer>();
            SpriteRenderer->SetSprite("Map_Ending001_1000.png");
            SpriteRenderer->SetOrder(ERenderOrder::UI);


            FVector2D WinSize = UEngineAPICore::GetCore()->GetMainWindow().GetWindowSize();
            SpriteRenderer->SetComponentScale(WinSize);
            FVector2D MapScale = SpriteRenderer->SetSpriteScale(1.0f);
            SpriteRenderer->SetComponentLocation(MapScale.Half());


            //SpriteRenderer->
            //UEngineAPICore::GetCore()->OpenLevel("Die");
            //Reflect({ 0, 1 });
            //ballPos.Y = MaxBottom - 0.1f;
            //hasCollided = true;
            //IsEndPos = true;
            return;
        }

        // ���� ���� ��Ҵ�
        if (MaxTop >= ballPos.Y && !hasCollided)
        {
            Reflect({ 0, -1 });
            ballPos.Y = MaxTop + 0.1f;
            hasCollided = true;
        }

        // ��ġ ������Ʈ
        ballPos += Value * deltaTime;

    }
}

float ABall::GetElapsedTime() const
{
    auto Now = std::chrono::steady_clock::now();
    auto Duration = std::chrono::duration<float>(Now - StopTime);  // �� ������ ��ȯ
    return Duration.count();  // float Ÿ������ ��ȯ
}
