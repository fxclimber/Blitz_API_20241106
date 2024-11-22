#include "PreCompiledFile.h"
#include "Map_Bottom.h"
#include <EngineCore/EngineAPICore.h>
#include <EngineCore/SpriteRenderer.h>
#include "ContentsEnum.h"
#include <EnginePlatform/EngineInput.h>



Map_Bottom::Map_Bottom()
{
	USpriteRenderer* SpriteRenderer = CreateDefaultSubObject<USpriteRenderer>();
	SpriteRenderer->SetOrder(ERenderOrder::BACKGROUND);
	SpriteRenderer->SetSprite("Map_Play003_Bottom.png");

	FVector2D WinSize = UEngineAPICore::GetCore()->GetMainWindow().GetWindowSize();
	SpriteRenderer->SetComponentScale(WinSize);

	FVector2D MapScale = SpriteRenderer->SetSpriteScale(1.0f);
	//SpriteRenderer->SetComponentLocation(MapScale.Half());
	SpriteRenderer->SetComponentLocation({ 385,956 });
}

