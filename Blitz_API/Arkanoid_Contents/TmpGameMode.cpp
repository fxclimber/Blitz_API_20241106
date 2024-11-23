#include "PreCompiledFile.h"
#include "TmpGameMode.h"
#include <EngineCore/Level.h>
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineAPICore.h>
#include <EngineBase/EngineFile.h>
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineRandom.h>
#include "ContentsEnum.h"

void TmpGameMode::BeginPlay()
{
	Super::BeginPlay();
	{
		FVector2D Size = { 57,26 };
		FIntPoint Num = { 11,23 };
		Editor = GetWorld()->SpawnActor<BrickEditor>();
		Editor->Create("Brick", Num, Size);

		for (int y = 0; y < Num.X; y++)
		{
			for (int x = 0; x < Num.Y; x++)
			{
				//Editor->SetBrickIndex({ y,x }, { 0, 0 }, Size, 4, 40001);
				//Editor->setBrickType({ y,x }, BrickType::Default);
			}
		}
		Editor->SetActorLocation({ 80.f,300.f });
	}
}

void TmpGameMode::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	if (UEngineInput::GetInst().IsDown('P'))
	{
		UEngineAPICore::GetCore()->OpenLevel("Play");
	}

	if (UEngineInput::GetInst().IsDown('Q'))
	{
		UEngineAPICore::GetCore()->OpenLevel("Editor");
	}

	static bool IsSpaceBarPressed = false;
	static BrickType BrickType = BrickType::Default;

	if (UEngineInput::GetInst().IsPress(VK_SPACE))
	{
		IsSpaceBarPressed = true;
	}
	if (IsSpaceBarPressed)
	{
		if (UEngineInput::GetInst().IsPress('0'))
		{
			BrickType = BrickType::Default;
			IsSpaceBarPressed = false;
		}
		else if (UEngineInput::GetInst().IsPress('6'))
		{
			BrickType = BrickType::HPBrick;
			IsSpaceBarPressed = false;
		}
		else if (UEngineInput::GetInst().IsPress('5'))
		{
			BrickType = BrickType::NotBreak;
			IsSpaceBarPressed = false;
		}
	}
	if (true == UEngineInput::GetInst().IsPress(VK_LBUTTON))
	{
		FVector2D MousePos = UEngineAPICore::GetCore()->GetMainWindow().GetMousePos();
		//ABrick* brick = GetWorld()->SpawnActor<ABrick>();		
		Editor->SetBrickLocation(MousePos, BrickType);
	}



	// ����
	if (true == UEngineInput::GetInst().IsPress(VK_RBUTTON))
	{
		FVector2D MousePos = UEngineAPICore::GetCore()->GetMainWindow().GetMousePos();
		ABrick* BaseTile = Editor->GetBrickRef(MousePos);
		if (BaseTile != nullptr && BaseTile->SpriteRenderer != nullptr)
		{
			BaseTile->BrickType = -1;
			BaseTile->SpriteRenderer->Destroy();
			BaseTile->SpriteRenderer = nullptr;
		}
	}


	// ����ȭ ����
	if (true == UEngineInput::GetInst().IsPress('M'))
	{
		UEngineSerializer _Ser;
		Editor->Serialize(_Ser);
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

	// ����ȭ �ε�
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
		Editor->DeSerialize(Ser);
	}
}