#include "PreCompiledFile.h"
#include "TitleGameMode.h"
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineAPICore.h>
#include "TestGameMode.h"
#include "Map_Title.h"

TitleGameMode::TitleGameMode()
{
}

void TitleGameMode::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->SpawnActor<Map_Title>();


}

void TitleGameMode::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	if (UEngineInput::GetInst().IsPress(VK_SPACE))
	{
		UEngineAPICore::GetCore()->OpenLevel("Play");
	}

}
