#include "PreCompiledFile.h"
#include "TestGameMode.h"
#include "BrickEditor.h"
#include "Ball.h"
#include "Brick.h"
#include "Paddle.h"




#include <EngineCore/Level.h>
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineAPICore.h>
#include <EngineBase/EngineFile.h>
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineRandom.h>

#include "ContentsEnum.h"




void ATestGameMode::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->SetCameraToMainPawn(false);

	Editor = GetWorld()->SpawnActor<BrickEditor>();
	Editor->SetBricksHeight(210);

	FVector2D Size = { 57,26 };
	FIntPoint Num = { 6,4 };
	Editor->Create("Brick", Num, Size);


	for (int y = 0; y < Num.Y; y++)
	{
		for (int x = 0; x < Num.X; x++)
		{
			int randomIndex = std::rand() % 5;//일반벽돌색 5개

			Editor->SetBrickIndex({ x,y }, { 0, 0 }, Size, 1);
			Editor->setBrickType({ x,y }, BrickType::Default);
		}
	}

	for (int x = 0; x < Num.X; x++)
	{
		Editor->setBrickType({x,3}, BrickType::HPBrick);
	}
	for (int x = 0; x < Num.X; x++)
	{
		Editor->setBrickType({x,2}, BrickType::NotBreak);
	}

	//저장 벽돌 로드
	{
		//UEngineDirectory Dir;

		//if (false == Dir.MoveParentToDirectory("Resources"))
		//{
		//	MSGASSERT("리소스 폴더를 찾지 못했습니다.");
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


	Ball = GetWorld()->SpawnActor<ABall>();
	Ball->SetDir({0.1f, 1.f});
	Ball->SetSpeed(550.0f);

	Paddle = GetWorld()->SpawnActor<APaddle>();
	Paddle->SetActorLocation({385,950});
}

void ATestGameMode::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
	Editor->CheckCollision(Ball, {0, 0});
	//Editor->RemoveBlock({1, 0});
	//Paddle->CheckCollision(Ball);

	FVector2D ballPos = Ball->GetActorLocation();
	FVector2D normal = Paddle->CheckCollision(ballPos);
	if (normal != FVector2D{ 0, 0 }) {
		//Ball->Reflect(normal);  // 공이 반사되도록 처리
		Ball->SetDir(normal);
		//Ball->Reflect(normal);
	}
	float fps = 1/_DeltaTime;
	UEngineDebug::CoreOutPutString("FPS : " + std::to_string(1.0f / _DeltaTime));

}






