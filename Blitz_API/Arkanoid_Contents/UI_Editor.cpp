#include "PreCompiledFile.h"
#include "UI_Editor.h"

#include "ContentsEnum.h"
#include "Fade.h"
#include "BrickEditorGameMode.h"

#include <EngineCore/EngineAPICore.h>
#include <EngineCore/SpriteRenderer.h>
#include <EnginePlatform/EngineInput.h>

UI_Editor::UI_Editor()
{
}

void UI_Editor::BeginPlay()
{
	Super::BeginPlay();

	gamemode = GetWorld()->GetGameMode<BrickEditorGameMode>();


	float center = 100.f;
	float height = 140.f;
	float smallFontsize = 20;
	float bigFontSize = 35;

	letter1 = GetWorld()->SpawnActor<ALetter>();
	letter1->SetActorLocation({ center,height });
	letter1->SetTextSpriteName("Text_Letters1.png");
	letter1->SetOrder(ERenderOrder::UI);
	letter1->SetTextScale({ smallFontsize, smallFontsize });
	letter1->SetText("rightclick");

	letter2 = GetWorld()->SpawnActor<ALetter>();
	letter2->SetActorLocation({ center+400.f,height });
	letter2->SetTextSpriteName("Text_Letters1.png");
	letter2->SetOrder(ERenderOrder::UI);
	letter2->SetTextScale({ bigFontSize, bigFontSize });
	letter2->SetText("draw");



	center = 100.f;
	height = 180.f;

	letter3 = GetWorld()->SpawnActor<ALetter>();
	letter3->SetActorLocation({ center,height });
	letter3->SetTextSpriteName("Text_Letters1.png");
	letter3->SetOrder(ERenderOrder::UI);
	letter3->SetTextScale({ smallFontsize , smallFontsize });
	letter3->SetText("leftclick");

	ALetter* letter4 = GetWorld()->SpawnActor<ALetter>();
	letter4->SetActorLocation({ center+400.f,height });
	letter4->SetTextSpriteName("Text_Letters1.png");
	letter4->SetOrder(ERenderOrder::UI);
	letter4->SetTextScale({ bigFontSize, bigFontSize });
	letter4->SetText("erase");


	center = 100.f;
	height = 220.f;

	ALetter* letter5 = GetWorld()->SpawnActor<ALetter>();
	letter5->SetActorLocation({ center,height });
	letter5->SetTextSpriteName("Text_Letters1.png");
	letter5->SetOrder(ERenderOrder::UI);
	letter5->SetTextScale({ bigFontSize , bigFontSize });
	letter5->SetText("M");

	ALetter* letter6 = GetWorld()->SpawnActor<ALetter>();
	letter6->SetActorLocation({ center + 400.f,height });
	letter6->SetTextSpriteName("Text_Letters1.png");
	letter6->SetOrder(ERenderOrder::UI);
	letter6->SetTextScale({ smallFontsize, smallFontsize });
	letter6->SetText("save");

	center = 100.f;
	height = 260.f;

	ALetter* letter8 = GetWorld()->SpawnActor<ALetter>();
	letter8->SetActorLocation({ center,height });
	letter8->SetTextSpriteName("Text_Letters1.png");
	letter8->SetOrder(ERenderOrder::UI);
	letter8->SetTextScale({ bigFontSize , bigFontSize });
	letter8->SetText("p");

	ALetter* letter7 = GetWorld()->SpawnActor<ALetter>();
	letter7->SetActorLocation({ center+400.f,height });
	letter7->SetTextSpriteName("Text_Letters1.png");
	letter7->SetOrder(ERenderOrder::UI);
	letter7->SetTextScale({ smallFontsize , smallFontsize });
	letter7->SetText("GoToPlay");






}

void UI_Editor::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);


}
