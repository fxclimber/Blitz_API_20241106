#include "PreCompiledFile.h"
#include "TestGameMode.h"
#include "BrickEditor.h"
#include "Ball.h"

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

	FVector2D Size = { 57,26 };
	FIntPoint Num = { 10,3 };
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
	Ball->SetSpeed(400.0f);
}

void ATestGameMode::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
	Editor->CheckCollision(Ball, {0, 0});
	// Editor->RemoveBlock({1, 0});

}






