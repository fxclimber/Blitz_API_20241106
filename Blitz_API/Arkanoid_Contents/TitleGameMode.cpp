#include "PreCompiledFile.h"
#include "TitleGameMode.h"

#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineAPICore.h>

#include "Ball.h"
#include "Map_Title.h"



void ATitleGameMode::BeginPlay()
{
	Super::BeginPlay();
	Map_Title* NewActor = GetWorld()->SpawnActor<Map_Title>();
}


void ATitleGameMode::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);


	if (true == UEngineInput::GetInst().IsDown('R'))
	{
		UEngineAPICore::GetCore()->OpenLevel("Play");
	}

}