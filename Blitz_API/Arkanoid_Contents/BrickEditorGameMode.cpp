#include "PreCompiledFile.h"
#include "BrickEditorGameMode.h"

#include <EngineCore/Level.h>
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineAPICore.h>
#include <EngineBase/EngineFile.h>
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineRandom.h>

#include "ContentsEnum.h"





// 타일찍고 저장할 수 있는 레벨
void BrickEditorGameMode::BeginPlay()
{
	Super::BeginPlay();

	//SetActorLocation(UEngineAPICore::GetCore()->GetMainWindow().GetWindowSize().Half());

	{
		//FVector2D Size = Globals::BrickSize; //{ 65,30 };
		FVector2D Size = { 57,26 };
		FIntPoint Num = { 10,3 };

		WallTileMap = GetWorld()->SpawnActor<BrickEditor>();
		WallTileMap->Create("Brick", Num, Size);

		//std::random
		std::srand(static_cast<unsigned int>(std::time(nullptr)));


		for (int y = 0; y < Num.X; y++)
		{
			for (int x = 0; x < Num.Y; x++)
			{
				//WallTileMap->SetBrickIndex({ y,x }, { 0, 0 }, Size, 6);
				int randomIndex = std::rand() % 5;//일반벽돌색 5개
				WallTileMap->SetBrickIndex({ y,x }, { 0, 0 }, Size, randomIndex);
			}
		}
	}




}

void BrickEditorGameMode::Tick(float _DeltaTime)
{
	// 맵이동 키입력
	if (true == UEngineInput::GetInst().IsDown('R'))
	{
		UEngineAPICore::GetCore()->OpenLevel("Ending");
	}

	if (true == UEngineInput::GetInst().IsPress(VK_LBUTTON))
	{
		FVector2D MousePos = UEngineAPICore::GetCore()->GetMainWindow().GetMousePos();
		WallTileMap->SetBrickLocation(MousePos, 5);
	}
	if (true == UEngineInput::GetInst().IsPress(VK_RBUTTON))
	{
		FVector2D MousePos = UEngineAPICore::GetCore()->GetMainWindow().GetMousePos();
		WallTileMap->SetBrickLocation(MousePos, 6);
	}

	//if (true == UEngineInput::GetInst().IsPress(VK_RBUTTON))
	//{
	//	FVector2D MousePos = UEngineAPICore::GetCore()->GetMainWindow().GetMousePos();
	//	ABrick* BaseTile = WallTileMap->GetBrickRef(MousePos);

	//	if (BaseTile != nullptr && BaseTile->SpriteRenderer != nullptr)
	//	{
	//		BaseTile->SpriteRenderer->Destroy(0.0f);
	//		BaseTile->SpriteRenderer = nullptr;
	//	}
	//}


	// 이동할 필요가 없다. 예로 보여주는것
	if (true == UEngineInput::GetInst().IsPress('A'))
	{
		WallTileMap->AddActorLocation(FVector2D::LEFT * _DeltaTime * 100.0f);
	}

	if (true == UEngineInput::GetInst().IsPress('D'))
	{
		WallTileMap->AddActorLocation(FVector2D::RIGHT * _DeltaTime * 100.0f);
	}

	if (true == UEngineInput::GetInst().IsPress('W'))
	{
		WallTileMap->AddActorLocation(FVector2D::UP * _DeltaTime * 100.0f);
	}

	if (true == UEngineInput::GetInst().IsPress('S'))
	{
		WallTileMap->AddActorLocation(FVector2D::DOWN * _DeltaTime * 100.0f);
	}


	// 직렬화,저장,로드
	if (true == UEngineInput::GetInst().IsPress('M'))
	{
		UEngineSerializer _Ser;
		WallTileMap->Serialize(_Ser);
		// 순수한 바이트 덩어리 그냥 저장하면 끝난다.
		UEngineDirectory Dir;

		if (false == Dir.MoveParentToDirectory("Resources"))
		{
			MSGASSERT("리소스 폴더를 찾지 못했습니다.");
			return;
		}

		Dir.Append("Data");

		std::string SaveFilePath = Dir.GetPathToString() + "\\MapData.Data";
		UEngineFile NewFile = SaveFilePath;
		NewFile.FileOpen("wb");
		NewFile.Write(_Ser);
	}

	if (true == UEngineInput::GetInst().IsPress('L'))
	{
		UEngineDirectory Dir;

		if (false == Dir.MoveParentToDirectory("Resources"))
		{
			MSGASSERT("리소스 폴더를 찾지 못했습니다.");
			return;
		}

		Dir.Append("Data");

		std::string SaveFilePath = Dir.GetPathToString() + "\\MapData.Data";
		UEngineFile NewFile = SaveFilePath;
		NewFile.FileOpen("rb");

		UEngineSerializer Ser;
		NewFile.Read(Ser);


		WallTileMap->DeSerialize(Ser);

	}

	// 랜덤
	if (true == UEngineInput::GetInst().IsPress('P'))
	{
		UEngineRandom Random;
		for (size_t i = 0; i < 10; i++)
		{
			int Value = Random.RandomInt(0, 100);
			UEngineDebug::OutPutString(std::to_string(Value));
		}
	}


}
