#include "PreCompiledFile.h"
#include "Map_Play.h"

#include <EngineCore/EngineAPICore.h>
#include <EngineCore/SpriteRenderer.h>
#include "ContentsEnum.h"
#include <EnginePlatform/EngineInput.h>

#include "TestGameMode.h"

Map_Play::Map_Play()
{
	{
		SpriteRender = CreateDefaultSubObject<USpriteRenderer>();
		SpriteRender->SetOrder(ERenderOrder::BACKGROUND);
		SpriteRender->SetSprite("Map_Play003_1000.png");

		FVector2D WinSize = UEngineAPICore::GetCore()->GetMainWindow().GetWindowSize();
		SpriteRender->SetComponentScale(WinSize);

		FVector2D MapScale = SpriteRender->SetSpriteScale(1.0f);
		SpriteRender->SetComponentLocation(MapScale.Half());
	}
	{
		SpriteRenderScore = CreateDefaultSubObject<USpriteRenderer>();
		SpriteRenderScore->SetOrder(ERenderOrder::Bricks);
		SpriteRenderScore->SetSprite("Text_Time.png"); 

		FVector2D WinSize = UEngineAPICore::GetCore()->GetMainWindow().GetWindowSize();
		SpriteRenderScore->SetComponentScale(WinSize);

		FVector2D MapScale = SpriteRenderScore->SetSpriteScale(0.8f);
		//SpriteRenderScore->SetComponentLocation(MapScale.Half());
		SpriteRenderScore->SetComponentLocation({ 70.f,50.f });

	}
	{
		SpriteRenderTime = CreateDefaultSubObject<USpriteRenderer>();
		SpriteRenderTime->SetOrder(ERenderOrder::Bricks);
		SpriteRenderTime->SetSprite("Text_Score.png");

		FVector2D WinSize = UEngineAPICore::GetCore()->GetMainWindow().GetWindowSize();
		SpriteRenderTime->SetComponentScale(WinSize);

		FVector2D MapScale = SpriteRenderTime->SetSpriteScale(0.8f);
		//SpriteRenderTime->SetComponentLocation(MapScale.Half());
		SpriteRenderTime->SetComponentLocation({ 420.f,50.f });

	}


}

void Map_Play::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->SetCameraToMainPawn(false);

}

void Map_Play::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	if (UEngineInput::GetInst().IsDown('R'))
	{
		UEngineAPICore::GetCore()->ResetLevel<ATestGameMode, Map_Play>("Play");
	}

}

