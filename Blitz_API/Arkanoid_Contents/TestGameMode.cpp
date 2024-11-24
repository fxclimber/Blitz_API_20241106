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
#include "Map_Play.h"
#include "UI.h"
#include "UI_P.h"

bool ATestGameMode::IsEnd = false;

ATestGameMode::ATestGameMode()
{

}

void ATestGameMode::BeginPlay()
{
	Super::BeginPlay();

	Bottom = GetWorld()->SpawnActor<Map_Bottom>();
	Bottom->GetRender()->SetActive(false);

	// UI text
	//UILetters = GetWorld()->SpawnActor<ALetter>();
	//UILetters->SetActorLocation({ 100,600 });
	//UILetters->SetTextSpriteName("Text_Letters.png");
	//UILetters->SetOrder(ERenderOrder::UI);
	//UILetters->SetTextScale({ 40, 40 });
	//UILetters->SetText("please");
	//UILetters->SetText("press");
	//UILetters->SetText("L");
	UILetters = GetWorld()->SpawnActor<UI>();
	UILettersP = GetWorld()->SpawnActor<UI_P>();
	IsUIMove = false;
	IsEditMode = false;

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

	FVector2D Size = { 57,26 };
	FIntPoint Num = { 11,2 };
	int score = 0;
	Editor->Create("Brick", Num, Size);



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
		//UEngineFile NewFile = SaveFilePath;
		//NewFile.FileOpen("rb");
		//UEngineSerializer Ser;
		//NewFile.Read(Ser);
		//Editor->DeSerialize(Ser);
	}
	// 그 경로의 파일 존재하지 않는다면 디폴트 방식으로 생성
	else 
	{
		//FVector2D Size = { 57,26 };
		//FIntPoint Num = { 11,2 };
		//int score = 0;
		//Editor->Create("Brick", Num, Size);

		for (int y = 0; y < Num.Y; y++)
		{
			for (int x = 0; x < Num.X; x++)
			{
				int randomIndex = std::rand() % 5;//일반벽돌색 5개
				//Editor->SetBrickIndex({ x,y }, { 0, 0 }, Size, 1, 1);
				//Editor->setBrickType({ x,y }, BrickType::Default);
			}
		}
	}



	//BreakCountTotal = CountBreakableBricks();



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
	GameClear = false;
}

void ATestGameMode::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	// ResetLevel
	if (UEngineInput::GetInst().IsDown('R'))
	{
		//UEngineAPICore::GetCore()->ResetLevel<ATestGameMode, Map_Play>("Play");
	}




	// 엔터누르면 바닥 생기기
	{
		static bool wasReturnDown = false; // 이전 프레임 엔터상태
		bool isReturnDown = UEngineInput::GetInst().IsDown(VK_RETURN); // 현재 키 상태

		// 키상태,처음만 실행
		if (!wasReturnDown && isReturnDown) {
			bool isActive = Bottom->GetRender()->IsActive();
			Bottom->GetRender()->SetActive(!isActive); // 상태 반전
		}
		// 지금상태를 다음프레임의 이전 상태로 저장
		wasReturnDown = isReturnDown;
	}


	if (UEngineInput::GetInst().IsDown('Q'))
	{
		UEngineAPICore::GetCore()->OpenLevel("Tmp");
	}



	if (UEngineInput::GetInst().IsDown('P'))
	{
		UEngineAPICore::GetCore()->OpenLevel("Editor");
		IsEditMode = true;
		IsEditMode = true;
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
	bool BrokenCount = AllBricksNonBreakable();

	if (true == GameClear)
	{
		if (nullptr != Editor)
		{
				// 클리어처리
				if (true == BrokenCount)
				{
					score->SetFinalValue(AScore::ScoreUI);
					UEngineAPICore::GetCore()->OpenLevel("Die");
				}
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

	// 벽돌로드 
	if (true == UEngineInput::GetInst().IsPress('L'))
	{
		UEngineDirectory Dir;
		if (false == Dir.MoveParentToDirectory("Resources"))
		{
			MSGASSERT("리소스 폴더를 찾지 못했습니다.");
			return;
		}
		Dir.Append("Data");
		std::string SaveFilePath = Dir.GetPathToString() + "\\MapData.Data";
		UEngineFile NewFile = SaveFilePath;
		NewFile.FileOpen("rb");
		UEngineSerializer Ser;
		NewFile.Read(Ser);
		Editor->DeSerialize(Ser);

		{
			float center = 100.f;
			float height = 840.f;
			float smallFontsize = 20;
			float bigFontSize = 35;

			ALetter* letter3 = GetWorld()->SpawnActor<ALetter>();
			letter3->SetActorLocation({ center,height });
			letter3->SetTextSpriteName("Text_Letters.png");
			letter3->SetOrder(ERenderOrder::UI);
			letter3->SetTextScale({ bigFontSize, bigFontSize });
			letter3->SetText("enter");

			ALetter* letter2 = GetWorld()->SpawnActor<ALetter>();
			letter2->SetActorLocation({ center,height + 40.f });
			letter2->SetTextSpriteName("Text_Letters.png");
			letter2->SetOrder(ERenderOrder::UI);
			letter2->SetTextScale({ smallFontsize, smallFontsize });
			letter2->SetText("can");

			ALetter* letter1 = GetWorld()->SpawnActor<ALetter>();
			letter1->SetActorLocation({ center,height + 80.f });
			letter1->SetTextSpriteName("Text_Letters.png");
			letter1->SetOrder(ERenderOrder::UI);
			letter1->SetTextScale({ smallFontsize, smallFontsize });
			letter1->SetText("helpyou");
		}

		BreakCountTotal = CountBreakableBricks();
		//if (nullptr != UILetters)
		{
			IsUIMove = true;
			//UILetters->Destroy();
			//UILetters->SetActive(false);

			//UILetters->GetLetter()[0]->GetRenders()[0]->SetAlphafloat(0.f);
			//UILetters->GetLetter()[0]->GetRenders()[0]->SetActive(false);
			//UILetters->GetLetter()[1]->GetRenders()[0]->SetActive(false);
			//UILetters->GetLetter()[2]->GetRenders()[0]->SetActive(false);
			return;
		}
	}

	if (true==IsUIMove)// 왜 안되!!!!
	{
		FVector2D dir = { 200.f,300.f };
		UILetters->AddActorLocation(dir * 500.f * _DeltaTime);
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
	int DefaultCount = 0;
	int HPBrickCount = 0;
	GameClear = true;

	std::vector<std::vector<ABrick>>& GetAllBricks = Editor->GetAllBricks();

	if (!Editor->GetAllBricks().empty()) {
		for (int y = 0; y < GetAllBricks.size(); y++) {
			for (int x = 0; x < GetAllBricks[y].size(); x++) {
				const auto& brick = GetAllBricks[y][x];
				switch (brick.BrickType) {
				case BrickType::Default:
					DefaultCount++;
					break;
				case BrickType::HPBrick:
					HPBrickCount++;
					break;
				default:
					break; 
				}
			}
		}
		BreakCountTotal = DefaultCount + HPBrickCount;
		return BreakCountTotal;
	}
}



bool ATestGameMode::AllBricksNonBreakable()
{
	DeathCount = Editor->GetDeathCount();
	if (DeathCount == BreakCountTotal)
	{
		return true;
	}

	UEngineDebug::CoreOutPutString("GameMode-Number of Bricks : " + std::to_string(BreakCountTotal));
	UEngineDebug::CoreOutPutString("GameMode-Number of DeathCount : " + std::to_string(DeathCount));
	return false;
}

