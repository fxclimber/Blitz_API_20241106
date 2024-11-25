#include "PreCompiledFile.h"
#include "DieGameMode.h"
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineAPICore.h>
//#include <EngineBase/EngineDebug.h>
#include <EngineCore/EngineCoreDebug.h>
#include "Score.h"
#include "Arkanoid_Contents/ContentsEnum.h"
#include <EngineCore/Level.h>
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineAPICore.h>
#include <EngineBase/EngineFile.h>
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineRandom.h>
#include "ContentsEnum.h"
#include "Map_Play.h"
#include "TestGameMode.h"
#include "Map_Die.h"
#include "Letter.h"

DieGameMode::DieGameMode()
{
	score = nullptr;
}

void DieGameMode::BeginPlay()
{
	// ------------상단 리셋 메세지

	float center = 40.f;
	float height = 20.f;
	float smallFontsize = 20;
	float bigFontSize = 35;

	ALetter* letterReset = GetWorld()->SpawnActor<ALetter>();
	letterReset->SetActorLocation({ center,height });
	letterReset->SetTextSpriteName("Text_Letters1.png");
	letterReset->SetOrder(ERenderOrder::UI);
	letterReset->SetTextScale({ smallFontsize, smallFontsize });
	letterReset->SetText("reset");

	ALetter* letterResetAll = GetWorld()->SpawnActor<ALetter>();
	letterResetAll->SetActorLocation({ center + 120.f,height });
	letterResetAll->SetTextSpriteName("Text_Letters1.png");
	letterResetAll->SetOrder(ERenderOrder::UI);
	letterResetAll->SetTextScale({ smallFontsize, smallFontsize });
	letterResetAll->SetText("all");

	ALetter* letterResetR = GetWorld()->SpawnActor<ALetter>();
	letterResetR->SetActorLocation({ center + 212.f,height });
	letterResetR->SetTextSpriteName("Text_Letters1.png");
	letterResetR->SetOrder(ERenderOrder::UI);
	letterResetR->SetTextScale({ bigFontSize, bigFontSize });
	letterResetR->SetText("r");

	//----------------------------------------------------------------

	//UI
	score = GetWorld()->SpawnActor<AScore>();
	score->SetActorLocation({ 385,500 });
	score->SetTextSpriteName("Text_Num.png");
	score->SetOrder(ERenderOrder::UI);
	score->SetTextScale({ 32, 32 });

	GetWorld()->SetCameraToMainPawn(false);

	// score
	score->SetValue(AScore::ScoreUI,8);//여기에 점수넣기
	score->SetFinalValue(AScore::FinalScoreUI);
	//UEngineAPICore::GetCore()->OpenLevel("Die");

}

void DieGameMode::Tick(float _DeltaTime)
{
	if (UEngineInput::GetInst().IsDown('P'))
	{
		UEngineAPICore::GetCore()->OpenLevel("Editor");
	}

	if (UEngineInput::GetInst().IsDown('R'))
	{
		//UEngineAPICore::GetCore()->ResetLevel<DieGameMode, Map_Die>("Die");
		UEngineAPICore::GetCore()->ResetLevel<ATestGameMode, Map_Play>("Play");

	}


}
