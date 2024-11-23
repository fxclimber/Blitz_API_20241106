#include "PreCompiledFile.h"
#include "Map_Bottom.h"
#include <EngineCore/EngineAPICore.h>
#include <EngineCore/SpriteRenderer.h>
#include "ContentsEnum.h"
#include <EnginePlatform/EngineInput.h>



Map_Bottom::Map_Bottom()
{
	SpriteRender = CreateDefaultSubObject<USpriteRenderer>();
	SpriteRender->SetOrder(ERenderOrder::BACKGROUND);
	SpriteRender->SetSprite("Map_Play003_Bottom.png");

	FVector2D WinSize = UEngineAPICore::GetCore()->GetMainWindow().GetWindowSize();
	SpriteRender->SetComponentScale(WinSize);

	FVector2D MapScale = SpriteRender->SetSpriteScale(1.0f);
	SpriteRender->SetComponentLocation({ 385,956 });
}

