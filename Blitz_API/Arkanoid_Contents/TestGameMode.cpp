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
	BonusA = nullptr;
	// tuto 
	{
		SpriteRenderTuto = CreateDefaultSubObject<USpriteRenderer>();
		SpriteRenderTuto->SetOrder(ERenderOrder::SURF);
		SpriteRenderTuto->SetSprite("tuto.png");

		FVector2D WinSize = UEngineAPICore::GetCore()->GetMainWindow().GetWindowSize();
		SpriteRenderTuto->SetComponentScale(WinSize);

		FVector2D MapScale = SpriteRenderTuto->SetSpriteScale(1.0f);
		SpriteRenderTuto->SetComponentLocation(MapScale.Half());
		SpriteRenderTuto->SetActive(true);
	}

}

void ATestGameMode::BeginPlay()
{
	Super::BeginPlay();

	Bottom = GetWorld()->SpawnActor<Map_Bottom>();
	Bottom->GetRender()->SetActive(false);

	// ------------��� ���� �޼���

	float center = 40.f;
	float height = 20.f;
	float smallFontsize = 20;
	float bigFontSize = 35;

	ALetter* letterReset = GetWorld()->SpawnActor<ALetter>();
	letterReset->SetActorLocation({ center,height });
	letterReset->SetTextSpriteName("Text_Letters1.png");
	letterReset->SetOrder(ERenderOrder::UI);
	letterReset->SetTextScale({ smallFontsize, smallFontsize });
	letterReset->SetText("reset");

	ALetter* letterResetAll = GetWorld()->SpawnActor<ALetter>();
	letterResetAll->SetActorLocation({ center +120.f,height });
	letterResetAll->SetTextSpriteName("Text_Letters1.png");
	letterResetAll->SetOrder(ERenderOrder::UI);
	letterResetAll->SetTextScale({ smallFontsize, smallFontsize });
	letterResetAll->SetText("all");

	ALetter* letterResetR = GetWorld()->SpawnActor<ALetter>();
	letterResetR->SetActorLocation({ center+212.f,height });
	letterResetR->SetTextSpriteName("Text_Letters1.png");
	letterResetR->SetOrder(ERenderOrder::UI);
	letterResetR->SetTextScale({ bigFontSize, bigFontSize });
	letterResetR->SetText("r");

	//----------------------------------------------------------------

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

	playTime = GetWorld()->SpawnActor<AScore>();
	playTime->SetActorLocation({ 290,50 });
	playTime->SetTextSpriteName("Text_Num.png");
	playTime->SetOrder(ERenderOrder::UI);
	playTime->SetTextScale({ 32, 32 });





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
		MSGASSERT("���ҽ� ������ ã�� ���߽��ϴ�.");
		return;
	}
	Dir.Append("Data");
	std::string SaveFilePath = Dir.GetPathToString() + "\\MapData.Data";
	std::string_view Ptr = SaveFilePath.c_str();
	// ��θ� �����
	UEnginePath Path = Ptr;

	// �� ����� ���� �����Ѵٸ� �ε�
	if (true == Path.IsExists())
	{
		//UEngineFile NewFile = SaveFilePath;
		//NewFile.FileOpen("rb");
		//UEngineSerializer Ser;
		//NewFile.Read(Ser);
		//Editor->DeSerialize(Ser);
	}
	// �� ����� ���� �������� �ʴ´ٸ� ����Ʈ ������� ����
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
				int randomIndex = std::rand() % 5;//�Ϲݺ����� 5��
				//Editor->SetBrickIndex({ x,y }, { 0, 0 }, Size, 1, 1);
				//Editor->setBrickType({ x,y }, BrickType::Default);
			}
		}
	}



	//BreakCountTotal = CountBreakableBricks();



	// Paddle
	Paddle = GetWorld()->SpawnActor<APaddle>();
	Paddle->SetActorLocation({ 385,950 });

	// ���� �� 1�� 
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
	// tuto ����
	{
		float EleapsedTime = Editor->GetElapsedTime();

		if (UEngineInput::GetInst().IsDown(VK_ESCAPE))
		{
			if (!SpriteRenderTuto)
			{
				MSGASSERT("SpriteRenderTuto ��ü�� ã�� ���߽��ϴ�.");
				return;
			}
			SpriteRenderTuto->SetActive(false);
		}

		if (UEngineInput::GetInst().IsDown(VK_F1))
		{
			if (!SpriteRenderTuto)
			{
				MSGASSERT("SpriteRenderTuto ��ü�� ã�� ���߽��ϴ�.");
				return;
			}
			SpriteRenderTuto->SetActive(true);
		}
		//if(UEngineInput::GetInst().IsDown(VK_ESCAPE))
		//{
		//	SpriteRenderTuto->SetActive(false);
		//}
		//if (UEngineInput::GetInst().IsDown(VK_F1))
		//{
		//	SpriteRenderTuto->SetActive(true);
		//}

	}

	// ���ʹ����� �ٴ� �����
	{
		static bool wasReturnDown = false; // ���� ������ ���ͻ���
		bool isReturnDown = UEngineInput::GetInst().IsDown(VK_RETURN); // ���� Ű ����

		// Ű����,ó���� ����
		if (!wasReturnDown && isReturnDown) {
			bool isActive = Bottom->GetRender()->IsActive();
			Bottom->GetRender()->SetActive(!isActive); // ���� ����
		}
		// ���ݻ��¸� ������������ ���� ���·� ����
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
					Ball->SetDir({ (paddleVel) * 0.5f + 0.01f, -1.0f }); // �е��� �ӵ��� �ݿ��Ͽ� ���� ����
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
	AScore::ScoreUI = Editor->GetScore() * 32;
	score->SetValue(AScore::ScoreUI , 8);//���⿡ �����ֱ�

	AScore::ElapsedTime = static_cast<int>(Editor->GetElapsedTime());
	playTime->SetValue(AScore::ElapsedTime ,5);


	DeathCount = Editor->GetDeathCount();
	bool BrokenCount = AllBricksNonBreakable();

	if (true == GameClear)
	{
		if (nullptr != Editor)
		{
				// Ŭ����ó��
				if (true == BrokenCount)
				{
					score->SetFinalValue(AScore::ScoreUI);
					UEngineAPICore::GetCore()->OpenLevel("Die");
				}
		}

	}

	// Ȯ�ο� �α׵� 
	{
		float fps = 1 / _DeltaTime;
		UEngineDebug::CoreOutPutString("FPS : " + std::to_string(1.0f / _DeltaTime));

		float EleapsedTime = Editor->GetElapsedTime();
		UEngineDebug::CoreOutPutString("EleapsedTime : " + std::to_string(EleapsedTime));

		int TotalScore = Editor->GetScore();
		UEngineDebug::CoreOutPutString("TotalScore : " + std::to_string(TotalScore * 32));
	}

	// �����ε� 
	if (true == UEngineInput::GetInst().IsPress('L'))
	{
		{
			//UEngineDirectory Dir;
			//if (false == Dir.MoveParentToDirectory("Resources"))
			//{
			//	MSGASSERT("���ҽ� ������ ã�� ���߽��ϴ�.");
			//	return;
			//}
			//Dir.Append("Data");
			//std::string SaveFilePath = Dir.GetPathToString() + "\\MapData.Data";
			//UEngineFile NewFile = SaveFilePath;
			//NewFile.FileOpen("rb");
			//UEngineSerializer Ser;
			//NewFile.Read(Ser);
			//Editor->DeSerialize(Ser);
		}
		// �������� ����ȭ �ε� �׽�Ʈ
		if (true == UEngineInput::GetInst().IsPress('L'))
		{
			UEngineDirectory Dir;
			if (false == Dir.MoveParentToDirectory("Resources"))
			{
				MSGASSERT("���ҽ� ������ ã�� ���߽��ϴ�.");
				return;
			}

			Dir.Append("Data");

			// ���� �ֽ� ������ ã�� ���� �߰�
			int FileIndex = 0;
			std::string SaveFilePath;

			// ���� ���� �ε����� ������ ã��
			do {
				SaveFilePath = Dir.GetPathToString() + "\\MapData" + std::to_string(FileIndex) + ".Data";
				FileIndex++;
			} while (std::filesystem::exists(SaveFilePath));

			// ������ ���� �ε��� ���� (�������� �ʴ� ���Ͽ��� �ϳ� �� ���Ϸ�)
			if (FileIndex > 1) {
				SaveFilePath = Dir.GetPathToString() + "\\MapData" + std::to_string(FileIndex - 2) + ".Data";
			}
			else {
				MSGASSERT("�ε��� ������ �����ϴ�.");
				return;
			}

			// ���� �б�
			UEngineFile NewFile = SaveFilePath;
			NewFile.FileOpen("rb");
			UEngineSerializer Ser;
			NewFile.Read(Ser);
			Editor->DeSerialize(Ser);
		}








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
			IsUIMove = true;
			return;
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
		// ���� �ջ�
		int finalScore = Editor->GetScore() * 32;
		score->SetValue(finalScore,4); // ���� ������Ʈ

		// ���� ���� ����
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

	return 0;
}



bool ATestGameMode::AllBricksNonBreakable()
{
	DeathCount = Editor->GetDeathCount();
	if (DeathCount == BreakCountTotal)
	{
		return true;
	}

	//UEngineDebug::CoreOutPutString("GameMode-Number of Bricks : " + std::to_string(BreakCountTotal));
	//UEngineDebug::CoreOutPutString("GameMode-Number of DeathCount : " + std::to_string(DeathCount));
	return false;
}

