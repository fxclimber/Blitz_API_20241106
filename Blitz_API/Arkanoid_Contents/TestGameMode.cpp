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
	Editor->SetBricksHeight(160);

	FVector2D Size = { 57,26 };
	FIntPoint Num = { 6,10 };
	Editor->Create("Brick", Num, Size);
	for (int y = 0; y < Num.X; y++)
	{
		for (int x = 0; x < Num.Y; x++)
		{
			Editor->SetBrickIndex({ y,x }, { 0, 0 }, Size, 2);
		}
	}

	Ball = GetWorld()->SpawnActor<ABall>();
	Ball->SetDir({1, 1});
	Ball->SetSpeed(700.0f);

	Paddle = GetWorld()->SpawnActor<APaddle>();
	Paddle->SetActorLocation({385,950});
	FVector2D ballPos = Ball->GetActorLocation();
	FVector2D ballsize = Ball->GetRender()->GetComponentScale();
	FVector2D HitNormal =Paddle->CheckCollision(ballPos, ballsize);
	UEngineDebug::CoreOutPutString("HitNormal :  " + HitNormal.ToString());

	//Ball->SetDir(HitNormal);
}

void ATestGameMode::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
	Editor->CheckCollision(Ball, {0, 0});
	//Editor->RemoveBlock({1, 0});

}






