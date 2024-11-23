#include "PreCompiledFile.h"
#include "Map_Title.h"
#include <EngineCore/EngineAPICore.h>
#include <EngineCore/SpriteRenderer.h>
#include "ContentsEnum.h"
#include <EnginePlatform/EngineInput.h>

#include "TestGameMode.h"

Map_Title::Map_Title()
{
	{
		SpriteRender = CreateDefaultSubObject<USpriteRenderer>();
		SpriteRender->SetOrder(ERenderOrder::BACKGROUND);
		SpriteRender->SetSprite("Map_Title_resize_1000.png");

		FVector2D WinSize = UEngineAPICore::GetCore()->GetMainWindow().GetWindowSize();
		SpriteRender->SetComponentScale(WinSize);

		FVector2D MapScale = SpriteRender->SetSpriteScale(1.0f);
		SpriteRender->SetComponentLocation(MapScale.Half());
	}



}

void Map_Title::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->SetCameraToMainPawn(false);
	BGM = UEngineSound::Play("jingle-bells.wav");

}

void Map_Title::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);


}
