#include "PreCompiledFile.h"
#include "DieGameMode.h"
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineAPICore.h>
//#include <EngineBase/EngineDebug.h>
#include <EngineCore/EngineCoreDebug.h>
#include "Score.h"
#include "Arkanoid_Contents/ContentsEnum.h"

DieGameMode::DieGameMode()
{
}

void DieGameMode::BeginPlay()
{
	//UI
	score = GetWorld()->SpawnActor<AScore>();
	score->SetActorLocation({ 755,50 });
	score->SetTextSpriteName("Text_Num.png");
	score->SetOrder(ERenderOrder::UI);
	score->SetTextScale({ 32, 32 });

	GetWorld()->SetCameraToMainPawn(false);

	// score
	score->SetValue(AScore::ScoreUI);//여기에 점수넣기
	score->SetFinalValue(AScore::FinalScoreUI);
	//UEngineAPICore::GetCore()->OpenLevel("Die");

}

void DieGameMode::Tick(float _DeltaTime)
{
	if (UEngineInput::GetInst().IsDown('C'))
	{
		//IsOpenLevel == true;
		UEngineAPICore::GetCore()->OpenLevel("Test");

	}

}
