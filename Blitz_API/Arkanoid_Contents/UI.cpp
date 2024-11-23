#include "PreCompiledFile.h"
#include "UI.h"
#include "ContentsEnum.h"
#include "Fade.h"
#include "TestGameMode.h"


UI::UI()
{
}

void UI::BeginPlay()
{
	Super::BeginPlay();
	float center = 300.f;
	float height = 500.f;

	letter1 = GetWorld()->SpawnActor<ALetter>();
	letter1->SetActorLocation({ center,height });
	//letter1->GetRenders()[0]->SetComponentLocation({0,0});
	letter1->SetTextSpriteName("Text_Letters.png");
	letter1->SetOrder(ERenderOrder::UI);
	letter1->SetTextScale({ 40, 40 });
	letter1->SetText("please");

	letter2 = GetWorld()->SpawnActor<ALetter>();
	//letter2->GetRenders()[1]->SetComponentLocation({ 0,0 });
	letter2->SetActorLocation({ center,height+70.f });
	letter2->SetTextSpriteName("Text_Letters.png");
	letter2->SetOrder(ERenderOrder::UI);
	letter2->SetTextScale({ 40, 40 });
	letter2->SetText("press");

	letter3 = GetWorld()->SpawnActor<ALetter>();
	letter3->SetActorLocation({ center+90.f,height+170.f });
	letter3->SetTextSpriteName("Text_Letters.png");
	letter3->SetOrder(ERenderOrder::UI);
	letter3->SetTextScale({ 70, 70 });
	letter3->SetText("L");

	Fade = GetWorld()->SpawnActor<AFade>();
	Fade->GetBackSpriteRenderer()->SetAlphafloat(0.f);

	gamemode = GetWorld()->GetGameMode<ATestGameMode>();







}

void UI::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	if(true == gamemode->IsUIMove)
	{
		letter1->SetActive(false);		
		letter2->SetActive(false);		
		letter3->SetActive(false);		
	}

}
