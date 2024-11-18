#include "PreCompiledFile.h"
#include "Fade.h"
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/EngineAPICore.h>
#include "ContentsEnum.h"

AFade* AFade::MainFade = nullptr;

AFade::AFade()
{
	{
		//BackSpriteRenderer = CreateDefaultSubObject<USpriteRenderer>();
		//BackSpriteRenderer->SetOrder(ERenderOrder::FADE);
		//BackSpriteRenderer->SetSprite("Fade.bmp");
		//FVector2D MapScale = BackSpriteRenderer->SetSpriteScale(1.0f);
		//BackSpriteRenderer->SetComponentLocation(MapScale.Half());
		//BackSpriteRenderer->SetCameraEffect(false);
		//BackSpriteRenderer->SetAlphaChar(200);
	}

	{
		BackSpriteRenderer = CreateDefaultSubObject<USpriteRenderer>();
		BackSpriteRenderer->SetSprite("Map_Ending001_1000.png");
		BackSpriteRenderer->SetOrder(ERenderOrder::FADE);

		FVector2D WinSize = UEngineAPICore::GetCore()->GetMainWindow().GetWindowSize();
		BackSpriteRenderer->SetComponentScale(WinSize);
		FVector2D MapScale = BackSpriteRenderer->SetSpriteScale(1.0f);
		BackSpriteRenderer->SetComponentLocation(MapScale.Half());

		BackSpriteRenderer->SetCameraEffect(false);
		BackSpriteRenderer->SetAlphaChar(200);
	}
}

AFade::~AFade()
{
}

void AFade::FadeChange()
{
	// 델타타임 얻어왔고
	float DeltaTime = UEngineAPICore::GetCore()->GetDeltaTime();
	FadeValue += DeltaTime * 0.5F * FadeDir;
	BackSpriteRenderer->SetAlphafloat(FadeValue);

}

// 0 => 1
void AFade::FadeIn()
{
	FadeValue = 0.0f;
	FadeDir = 1.0f;
	TimeEventer.PushEvent(5.0f, std::bind(&AFade::FadeChange, this), true, false);
}

void AFade::FadeOut()
{
	FadeValue = 1.0f;
	FadeDir = -1.0f;
	TimeEventer.PushEvent(5.0f, std::bind(&AFade::FadeChange, this), true, false);
}

void AFade::LevelChangeStart()
{
	// BeginPlay나 생성자에서 했는데.
	// 이제부터 내가 mainFade야 라고 설정하게 됩니다.
	MainFade = this;
}