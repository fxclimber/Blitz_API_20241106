#include "PreCompiledFile.h"
#include "TestGameMode.h"
#include "BrickEditor.h"
#include "Ball.h"
#include "Brick.h"
#include "Paddle.h"

#include <format>
#include <EngineCore/Level.h>
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineAPICore.h>
#include <EngineBase/EngineFile.h>
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineRandom.h>
#include <EngineBase/EngineMath.h>

#include "ContentsEnum.h"
#include "Score.h"
#include "Fade.h"

void ATestGameMode::BeginPlay()
{
	Super::BeginPlay();

	//UI
	score = GetWorld()->SpawnActor<AScore>();
	score->SetActorLocation({ 755,50 });
	score->SetTextSpriteName("Text_Num.png");
	score->SetOrder(ERenderOrder::UI);
	score->SetTextScale({ 32, 32 });

	GetWorld()->SetCameraToMainPawn(false);

	// BrickEditor
	Editor = GetWorld()->SpawnActor<BrickEditor>();
	Editor->SetBricksHeight(210);

// �÷��� ���Ӹ�忡�� ���Ϸε� 
{
	UEngineDirectory Dir;
	if (false == Dir.MoveParentToDirectory("Resources"))
	{
		MSGASSERT("���ҽ� ������ ã�� ���߽��ϴ�.");
		return;
	}
	Dir.Append("Data");
	std::string SaveFilePath = Dir.GetPathToString() + "\\MapData.Data";
	UEngineFile NewFile = SaveFilePath;
	NewFile.FileOpen("rb");
	UEngineSerializer Ser;
	NewFile.Read(Ser);
	Editor->DeSerialize(Ser);
	Editor->SetActorLocation(FVector2D{50,300});
}



	FVector2D Size = { 57,26 };
	FIntPoint Num = { 5,8 };
	int score = 0;
	Editor->Create("Brick", Num, Size );

	for (int y = 0; y < Num.Y; y++)
	{
		for (int x = 0; x < Num.X; x++)
		{
			int randomIndex = std::rand() % 5;//�Ϲݺ����� 5��

			//Editor->SetBrickIndex({ x,y }, { 0, 0 }, Size, 1);
			//Editor->setBrickType({ x,y }, BrickType::Default);
		}
	}

	//for (int x = 0; x < Num.X; x++)
	//{
	//	Editor->setBrickType({x,3}, BrickType::HPBrick);
	//}
	//for (int x = 0; x < Num.X; x++)
	//{
	//	Editor->setBrickType({x,2}, BrickType::NotBreak);
	//}




	// Paddle
	Paddle = GetWorld()->SpawnActor<APaddle>();
	Paddle->SetActorLocation({385,950});

	// ���� �� 1�� 
	Balls.push_back(GetWorld()->SpawnActor<ABall>());
	for (ABall* Ball : Balls)
	{
		Ball->SetDir({ 0.02f, 1.f });
		Ball->SetSpeed(650.0f);
		Ball->SetActorLocation({ Paddle->GetActorLocation().X, Paddle->GetActorLocation().Y - Paddle->PaddleScale.Y });
	}
}

void ATestGameMode::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	float paddleVel = Paddle->GetVel().X;

	// ���� �������϶�
	for (ABall* Ball : Balls)
	{
		if (Ball == nullptr) continue; // null üũ

		if (true == Ball->GetFadeOver())
		{
			if (false == Ball->GetIsMove())
			{
				Ball->SetActorLocation({ Paddle->GetActorLocation().X, Paddle->GetActorLocation().Y - Paddle->PaddleScale.Y });

				if (UEngineInput::GetInst().IsDown(VK_SPACE))
				{
					Ball->SetIsMove(true);
					Ball->SetSpeed(650.0f);
					Ball->SetDir({ (paddleVel)*0.5f+0.01f, -1.0f }); // �е��� �ӵ��� �ݿ��Ͽ� ���� ����
				}
			}
			// �� Ball�� ���� �浹 üũ
			Editor->CheckCollision(Ball, { 0, 0 });

			FVector2D ballPos = Ball->GetActorLocation();
			FVector2D normal = Paddle->CheckCollision(ballPos);
			if (normal != FVector2D{ 0, 0 })
			{
				Ball->SetDir(normal);
			}
		}
	}
	


	// score
	AScore::ScoreUI = Editor->GetScore()*32;
	score->SetValue(AScore::ScoreUI);//���⿡ �����ֱ�

	// Ȯ�ο� �α׵� 
	{
		float fps = 1/_DeltaTime;
		UEngineDebug::CoreOutPutString("FPS : " + std::to_string(1.0f / _DeltaTime));

		float EleapsedTime = Editor->GetElapsedTime();
		UEngineDebug::CoreOutPutString("EleapsedTime : " + std::to_string(EleapsedTime));

		int TotalScore = Editor->GetScore();
		UEngineDebug::CoreOutPutString("TotalScore : " + std::to_string(TotalScore*32));
	}

}

void ATestGameMode::SpawnBall()
{
	if (Balls.size() > 3)
	{
		return;
	}
	else
	{
		UEngineRandom Random;

		for (size_t i = 0; i < 3; i++)
		{
			float randomf = Random.Randomfloat(0.01f,.02f);

			ABall* Ball0 = GetWorld()->SpawnActor<ABall>();
			Ball0->SetDir({ randomf, 1.f });
			Ball0->SetSpeed(350.0f);
			Ball0->SetActorLocation({ Paddle->GetActorLocation().X, Paddle->GetActorLocation().Y - Paddle->PaddleScale.Y });
			Ball0->GetRender()->SetSprite("ball_blue.png");
			Balls.push_back(Ball0);
		}

	}
}
