#include "PreCompiledFile.h"
#include "EndGameMode.h"
#include "Map_Ending.h"

#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineAPICore.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/EngineCoreDebug.h>


AEndGameMode::AEndGameMode()
{
}

AEndGameMode::~AEndGameMode()
{
}

void AEndGameMode::BeginPlay()
{
	Super::BeginPlay();

	//GetWorld()->SetCameraToMainPawn(false);
	Map_Ending* NewActor = GetWorld()->SpawnActor<Map_Ending>();


}

void AEndGameMode::Tick(float _DeltaTime)
{
	if (true == UEngineInput::GetInst().IsDown('R'))
	{
		UEngineAPICore::GetCore()->OpenLevel("Tile");
	}
}
