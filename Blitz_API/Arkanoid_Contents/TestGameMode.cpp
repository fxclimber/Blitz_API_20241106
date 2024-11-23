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
#include "Map_Bottom.h"

bool ATestGameMode::IsEnd = false;

ATestGameMode::ATestGameMode()
{
}

void ATestGameMode::BeginPlay()
{
	Super::BeginPlay();

	Bottom = GetWorld()->SpawnActor<Map_Bottom>();
	Bottom->GetRender()->SetActive(false);

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

	UEngineDirectory Dir;
	if (false == Dir.MoveParentToDirectory("Resources"))
	{
		MSGASSERT("리소스 폴더를 찾지 못했습니다.");
		return;
	}
	Dir.Append("Data");
	std::string SaveFilePath = Dir.GetPathToString() + "\\MapData.Data";
	std::string_view Ptr = SaveFilePath.c_str();
	// 경로를 만들고
	UEnginePath Path = Ptr;

	// 그 경로의 파일 존재한다면 로드
	if (true == Path.IsExists())
	{
		UEngineFile NewFile = SaveFilePath;
		NewFile.FileOpen("rb");
		UEngineSerializer Ser;
		NewFile.Read(Ser);
		Editor->DeSerialize(Ser);
	}
	// 그 경로의 파일 존재하지 않는다면 디폴트 방식으로 생성
	else 
	{
		FVector2D Size = { 57,26 };
		FIntPoint Num = { 11,2 };
		int score = 0;
		EditorLoad = Editor->Create("Brick", Num, Size);

		for (int y = 0; y < Num.Y; y++)
		{
			for (int x = 0; x < Num.X; x++)
			{
				int randomIndex = std::rand() % 5;//일반벽돌색 5개
				Editor->SetBrickIndex({ x,y }, { 0, 0 }, Size, 1, 1);
				Editor->setBrickType({ x,y }, BrickType::Default);
			}
		}
	}



	BreakCountTotal = CountBreakableBricks();
	UEngineDebug::CoreOutPutString("GameMode-Number of Bricks : " + std::to_string(BreakCountTotal));



	// Paddle
	Paddle = GetWorld()->SpawnActor<APaddle>();
	Paddle->SetActorLocation({ 385,950 });

	// 시작 볼 1개 
	Balls.push_back(GetWorld()->SpawnActor<ABall>());
	for (ABall* Ball : Balls)
	{
		Ball->SetDir({ 0.01f, 1.f });
		Ball->SetSpeed(700.f);
		Ball->SetActorLocation({ Paddle->GetActorLocation().X, Paddle->GetActorLocation().Y - Paddle->PaddleScale.Y });
	}

}

void ATestGameMode::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	if (false == Bottom-> GetRender()->IsActive())
	{
		if (UEngineInput::GetInst().IsDown(VK_RETURN))
		{
			Bottom->GetRender()->SetActive(true);
		}
	}


	if (UEngineInput::GetInst().IsDown('P'))
	{
		UEngineAPICore::GetCore()->OpenLevel("Editor");
	}

	float paddleVel = Paddle->GetVel().X;

	// 볼이 여러개일때
	for (ABall* Ball : Balls)
	{
		if (Ball == nullptr) continue; // null 체크

		if (true == Ball->GetFadeOver())
		{
			if (false == Ball->GetIsMove())
			{
				Ball->SetActorLocation({ Paddle->GetActorLocation().X, Paddle->GetActorLocation().Y - Paddle->PaddleScale.Y });

				if (UEngineInput::GetInst().IsDown(VK_SPACE))
				{
					Ball->SetIsMove(true);
					Ball->SetSpeed(650.0f);
					Ball->SetDir({ (paddleVel) * 0.5f + 0.01f, -1.0f }); // 패들의 속도를 반영하여 방향 설정
				}
			}
			// 각 Ball에 대해 충돌 체크
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
	AScore::ScoreUI = Editor->GetScore() * 32;
	score->SetValue(AScore::ScoreUI);//여기에 점수넣기

	DeathCount = Editor->GetDeathCount();

	int BrokenCount = AllBricksNonBreakable();

	if (nullptr != Editor)
	{
			// 클리어처리
			if (1 == BrokenCount)
			{
				score->SetFinalValue(AScore::ScoreUI);
				//UEngineAPICore::GetCore()->OpenLevel("Die");
			}
	}

	// 확인용 로그들 
	{
		float fps = 1 / _DeltaTime;
		UEngineDebug::CoreOutPutString("FPS : " + std::to_string(1.0f / _DeltaTime));

		float EleapsedTime = Editor->GetElapsedTime();
		UEngineDebug::CoreOutPutString("EleapsedTime : " + std::to_string(EleapsedTime));

		int TotalScore = Editor->GetScore();
		UEngineDebug::CoreOutPutString("TotalScore : " + std::to_string(TotalScore * 32));


	}

}


void ATestGameMode::SpawnBall()
{
		UEngineRandom Random;

	for (size_t i = 0; i < 3; i++)
	{
		float randomf = Random.Randomfloat(-0.03f, 0.01f);

		ABall* Ball0 = GetWorld()->SpawnActor<ABall>();
		Ball0->SetDir({ randomf, 1.f });
		Ball0->SetSpeed(550.0f);
		Ball0->SetActorLocation({ Paddle->GetActorLocation().X, Paddle->GetActorLocation().Y - Paddle->PaddleScale.Y });
		Ball0->GetRender()->SetSprite("ball_blue.png");
		Ball0->ballType = BallType::Bonus;
		Balls.push_back(Ball0);
	}

}

void ATestGameMode::CheckScore()
{
	{
		// 점수 합산
		int finalScore = Editor->GetScore() * 32;
		score->SetValue(finalScore); // 점수 업데이트

		// 최종 점수 저장
		score->SetFinalValue(finalScore);
	}
}


int ATestGameMode::CountBreakableBricks()
{
	int NotBreakcount = 0;
	int SpriteCount = 0;
	std::vector<std::vector<ABrick>>& Test = Editor->GetAllBricks();

	if (nullptr != Editor)
	{
		if (0 != Editor->GetAllBricks().size())
		{
			for (int y = 0; y < Test.size(); y++)
			{
				for (int x = 0; x < Test[y].size(); x++)
				{
					if (Test[y][x].BrickType == BrickType::NotBreak)
					{
						NotBreakcount++;
						BreakCountTotal = 0;
					}
				}
			}


			for (int y = 0; y < Test.size(); y++)
			{
				for (int x = 0; x < Test[y].size(); x++)
				{
					if (nullptr != Test[y][x].SpriteRenderer)
					{
						SpriteCount++;
					}
					else
					{
						SpriteCount = 10;
					}
				}
			}
			//BreakCountTotal = 0;
			BreakCountTotal = SpriteCount - NotBreakcount;
			return BreakCountTotal;
		}
		else
		{
			return 5;
		}

	}
}

int ATestGameMode::test()
{
	return 10;
}


int ATestGameMode::AllBricksNonBreakable()
{
	DeathCount = Editor->GetDeathCount();
	//DeathCount = test();
	//DeathCount = 11;
	BreakCountTotal = 1;


	const int CurrentDeathCount = DeathCount;
	const int CurrentBreakCountTotal = BreakCountTotal;

	if (Editor == nullptr || Editor->GetAllBricks().empty())
	{
		return 0;
	}
	else if (CurrentDeathCount-1 == CurrentBreakCountTotal)
	{
		return 1;
	}




	UEngineDebug::CoreOutPutString("GameMode-Number of Bricks : " + std::to_string(BreakCountTotal));
	UEngineDebug::CoreOutPutString("GameMode-Number of DeathCount : " + std::to_string(Editor->GetDeathCount()));	
}

