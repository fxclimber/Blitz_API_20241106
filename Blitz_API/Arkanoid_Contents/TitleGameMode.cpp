#include "PreCompiledFile.h"
#include "TitleGameMode.h"

#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineAPICore.h>

#include "TitleLogo.h"
#include "Ball.h"
#include "Map_Title.h"

ATitleGameMode::ATitleGameMode()
{
}

ATitleGameMode::~ATitleGameMode()
{
}


// 언리얼에서는 MainPawn 주인공 무조건 지정하게 해요.
void ATitleGameMode::BeginPlay()
{
	Super::BeginPlay();
	//GetWorld()->SpawnActor<ABall>();
	Map_Title* NewActor = GetWorld()->SpawnActor<Map_Title>();
	// TitleLogo* NewActor = GetWorld()->SpawnActor<TitleLogo>();
}


void ATitleGameMode::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);


	if (true == UEngineInput::GetInst().IsDown('R'))
	{
		UEngineAPICore::GetCore()->OpenLevel("Play");
	}

}