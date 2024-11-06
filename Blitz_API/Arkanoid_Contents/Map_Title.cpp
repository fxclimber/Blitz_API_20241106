#include "PreCompiledFile.h"
#include "Map_Title.h"


#include <EngineCore/EngineAPICore.h>
#include <EngineCore/SpriteRenderer.h>
#include "ContentsEnum.h"

Map_Title::Map_Title()
{
	{
		USpriteRenderer* SpriteRenderer = CreateDefaultSubObject<USpriteRenderer>();
		SpriteRenderer->SetOrder(ERenderOrder::BACKGROUND);
		SpriteRenderer->SetSprite("Map_Title_resize.png");

		FVector2D WinSize = UEngineAPICore::GetCore()->GetMainWindow().GetWindowSize();
		SpriteRenderer->SetComponentScale(WinSize);
		// 이미지의 원본크기의 비율. 1이면, 원본사이즈
		FVector2D MapScale = SpriteRenderer->SetSpriteScale(1.0f);
		SpriteRenderer->SetComponentLocation(MapScale.Half());
	}


}

Map_Title::~Map_Title()
{
}
