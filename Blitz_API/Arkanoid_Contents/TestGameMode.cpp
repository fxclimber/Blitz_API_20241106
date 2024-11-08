#include "PreCompiledFile.h"
#include "TestGameMode.h"
#include "BrickEditor.h"
#include "Ball.h"
#include "Brick.h"
#include "Paddle.h"

ATestGameMode::ATestGameMode()
{
}

ATestGameMode::~ATestGameMode()
{
}

void ATestGameMode::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->SetCameraToMainPawn(false);

	Editor = GetWorld()->SpawnActor<BrickEditor>();
	Editor->SetBricksHeight(210);

	FVector2D Size = { 57,26 };
	FIntPoint Num = { 6,4 };
	Editor->Create("Brick", Num, Size);
	for (int y = 0; y < Num.X; y++)
	{
		for (int x = 0; x < Num.Y; x++)
		{
			Editor->SetBrickIndex({ y,x }, { 0, 0 }, Size, 2);
		}
	}

	Ball = GetWorld()->SpawnActor<ABall>();
	Ball->SetDir({0.1f, 1.f});
	Ball->SetSpeed(350.0f);

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






