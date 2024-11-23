#include "PreCompiledFile.h"
#include "Map_Play.h"

#include <EngineCore/EngineAPICore.h>
#include <EngineCore/SpriteRenderer.h>
#include "ContentsEnum.h"
#include <EnginePlatform/EngineInput.h>

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
	
}

