#include "PreCompiledFile.h"
#include "BrickEditorGameMode.h"

#include <EngineCore/Level.h>
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineAPICore.h>
#include <EngineBase/EngineFile.h>
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineRandom.h>

#include "ContentsEnum.h"





// Ÿ����� ������ �� �ִ� ����
void BrickEditorGameMode::BeginPlay()
{
	Super::BeginPlay();

	//SetActorLocation(UEngineAPICore::GetCore()->GetMainWindow().GetWindowSize().Half());

	{
		//FVector2D Size = Globals::BrickSize; //{ 65,30 };
		FVector2D Size = { 57,26 };
		FIntPoint Num = { 10,8 };

		WallTileMap = GetWorld()->SpawnActor<BrickEditor>();
		WallTileMap->Create("Brick", Num, Size);

		for (int y = 0; y < Num.X; y++)
		{
			for (int x = 0; x < Num.Y; x++)
			{
				WallTileMap->SetBrickIndex({ y,x }, { 0, 0 }, Size, 2);
			}
		}
	}

	//{
	//	UEngineDirectory Dir;

	//	if (false == Dir.MoveParentToDirectory("Resources"))
	//	{
	//		MSGASSERT("���ҽ� ������ ã�� ���߽��ϴ�.");
	//		return;
	//	}

	//	Dir.Append("Data");

	//	std::string SaveFilePath = Dir.GetPathToString() + "\\MapData.Data";
	//	UEngineFile NewFile = SaveFilePath;
	//	NewFile.FileOpen("rb");

	//	UEngineSerializer Ser;
	//	NewFile.Read(Ser);


	//	WallTileMap->DeSerialize(Ser);
	//}

}

void BrickEditorGameMode::Tick(float _DeltaTime)
{
	// ���̵� Ű�Է�
	if (true == UEngineInput::GetInst().IsDown('R'))
	{
		UEngineAPICore::GetCore()->OpenLevel("Ending");
	}

	if (true == UEngineInput::GetInst().IsPress(VK_LBUTTON))
	{
		FVector2D MousePos = UEngineAPICore::GetCore()->GetMainWindow().GetMousePos();
		WallTileMap->SetBrickLocation(MousePos, 1);
	}

	if (true == UEngineInput::GetInst().IsPress(VK_RBUTTON))
	{
		FVector2D MousePos = UEngineAPICore::GetCore()->GetMainWindow().GetMousePos();
		ABrick* BaseTile = WallTileMap->GetBrickRef(MousePos);

		if (BaseTile != nullptr && BaseTile->SpriteRenderer != nullptr)
		{
			BaseTile->SpriteRenderer->Destroy(0.0f);
			BaseTile->SpriteRenderer = nullptr;
		}
	}


	// �̵��� �ʿ䰡 ����. ���� �����ִ°�
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


	// ����ȭ,����,�ε�
	if (true == UEngineInput::GetInst().IsPress('M'))
	{
		UEngineSerializer _Ser;
		WallTileMap->Serialize(_Ser);
		// ������ ����Ʈ ��� �׳� �����ϸ� ������.
		UEngineDirectory Dir;

		if (false == Dir.MoveParentToDirectory("Resources"))
		{
			MSGASSERT("���ҽ� ������ ã�� ���߽��ϴ�.");
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
			MSGASSERT("���ҽ� ������ ã�� ���߽��ϴ�.");
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

	// ����
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
