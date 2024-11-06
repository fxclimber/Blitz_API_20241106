#include "PreCompiledFile.h"
#include "PlayMap.h"
#include <EngineCore/EngineAPICore.h>
#include <EngineBase/EngineMath.h>
#include "Ball.h"
#include <iostream>
#include <EnginePlatform/EngineWinImage.h>
#include "Bullet.h"

#include <EngineCore/SpriteRenderer.h>


APlayMap::APlayMap()
{
	SetActorLocation({ 450,450 });
	//SetActorScale({ 300, 150 });

	SpriteRenderer = CreateDefaultSubObject<USpriteRenderer>();
	SpriteRenderer->SetSprite("characters.png");
	SpriteRenderer->SetComponentScale({ 100, 100 });

}

APlayMap::~APlayMap()
{
}

void APlayMap::Tick(float _DeltaTime)
{
	FVector2D Pos = GetActorLocation();
	FVector2D Scale = { 300, 150 };


	// 은정 로그 - 테스트 볼위치 출력
	//UEngineWindow& MainWindow = UEngineAPICore::GetCore()->GetMainWindow();
	//UEngineWinImage* WindowImage = MainWindow.GetWindowImage();
	//HDC dc = WindowImage->GetDC();  // HDC를 가져옴
	//std::wstring str = L"BallPos : (" + std::to_wstring(BallPos.X) + L", " + std::to_wstring(BallPos.Y) + L")";
	//TextOutW(dc, 100, 100, str.c_str(), static_cast<int>(str.size()));



}

