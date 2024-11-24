#include "PreCompiledFile.h"
#include "UI_P.h"
#include "ContentsEnum.h"
#include "Fade.h"
#include "TestGameMode.h"


UI_P::UI_P()
{
}

void UI_P::BeginPlay()
{
	Super::BeginPlay();
	float center = 100.f;
	float height = 130.f;

	letter1 = GetWorld()->SpawnActor<ALetter>();
	letter1->SetActorLocation({ center,height });
	//letter1->GetRenders()[0]->SetComponentLocation({0,0});
	letter1->SetTextSpriteName("Text_Letters.png");
	letter1->SetOrder(ERenderOrder::UI);
	letter1->SetTextScale({ 23, 23 });
	letter1->SetText("Make");

	letter2 = GetWorld()->SpawnActor<ALetter>();
	//letter2->GetRenders()[1]->SetComponentLocation({ 0,0 });
	letter2->SetActorLocation({ center,height + 30.f });
	letter2->SetTextSpriteName("Text_Letters.png");
	letter2->SetOrder(ERenderOrder::UI);
	letter2->SetTextScale({ 23, 23 });
	letter2->SetText("YourStage");

	letter3 = GetWorld()->SpawnActor<ALetter>();
	letter3->SetActorLocation({ center+14.f,height + 70.f });
	letter3->SetTextSpriteName("Text_Letters.png");
	letter3->SetOrder(ERenderOrder::UI);
	letter3->SetTextScale({ 50, 50 });
	letter3->SetText("P");

	Fade = GetWorld()->SpawnActor<AFade>();
	Fade->GetBackSpriteRenderer()->SetAlphafloat(0.f);

	gamemode = GetWorld()->GetGameMode<ATestGameMode>();

	gamemode->IsEditMode = false;






}

void UI_P::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	if (true==gamemode->IsEditMode)
	{
		letter1->SetActive(false);
		letter2->SetActive(false);
		letter3->SetActive(false);
	}

}
