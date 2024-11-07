#include "PreCompiledFile.h"
#include "BrickEditor.h"

#include <EnginePlatform/EngineInput.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/EngineAPICore.h>
#include <EngineCore/EngineCoreDebug.h>
#include "ContentsEnum.h"



void BrickEditor::Create(std::string_view _Sprite, FIntPoint _Count, FVector2D _BrickSize)
{
	SpriteName = _Sprite;
	BrickSize = _BrickSize;
	BrickCount = _Count;

	AllBricks.resize(_Count.Y);

	for (size_t y = 0; y < AllBricks.size(); y++)
	{
		AllBricks[y].resize(_Count.X);;
	}

	// Ÿ�ϸ��� �ٽ� 0, 0�� �ΰ����� ���ؾ� �Ѵ�.
	// Ÿ�Ϸμ��� 0,0�� ����.
	// Ÿ���� ��ġ�� ����μ��� Ÿ�ϸ� * �ε����� �ȴ�.
	float WinSize = UEngineAPICore::GetCore()->GetMainWindow().GetWindowSize().Half().X;
	//float WinSize = Globals::WinSize.X;//������....��........
	float AllSpriteWidth = (_BrickSize.X * static_cast<float>(_Count.X)) / 2;
	float gap = WinSize - AllSpriteWidth;
	SetActorLocation({ gap ,100.f });
}

FVector2D BrickEditor::IndexToBrickLocation(FIntPoint _Index)
{
	//�ε����� ������� ������ ��ġ�� ���
	return FVector2D(_Index.X * BrickSize.X, _Index.Y * BrickSize.Y);
}

FIntPoint BrickEditor::LocationToIndex(FVector2D _Location)
{
	FVector2D Location = _Location / BrickSize;

	return FIntPoint(Location.iX(), Location.iY());
}

void BrickEditor::SetBrickLocation(FVector2D _Location, int _SpriteIndex)
{
	FVector2D TilePos = _Location - GetActorLocation();

	FIntPoint Point = LocationToIndex(TilePos);

	if (true == IsIndexOver(Point))
	{
		return;
	}

	SetBrickIndex(Point, _SpriteIndex);
}

bool BrickEditor::IsIndexOver(FIntPoint _Index)
{
	if (0 > _Index.X)
	{
		return true;
	}

	if (0 > _Index.Y)
	{
		return true;
	}

	if (BrickCount.X - 1 < _Index.X)
	{
		return true;
	}

	if (BrickCount.Y - 1 < _Index.Y)
	{
		return true;
	}

	return false;
}

void BrickEditor::SetBrickIndex(FIntPoint _Index, int _SpriteIndex)
{
	SetBrickIndex(_Index, { 0,0 }, BrickSize, _SpriteIndex);
}

// _SpriteScale : ���� ����ũ��, ���� ������ġ ���
void BrickEditor::SetBrickIndex(FIntPoint _Index, FVector2D _Pivot, FVector2D _SpriteScale, int _SpriteIndex)
{
	if (true == IsIndexOver(_Index))
	{
		return;
	}

	// �̸� �� ������ �ʰ�
	// ���� ��������Ʈ �������� �����.
	if (nullptr == AllBricks[_Index.Y][_Index.X].SpriteRenderer)
	{
		USpriteRenderer* NewSpriteRenderer = CreateDefaultSubObject<USpriteRenderer>();
		NewSpriteRenderer->SetComponentScale(BrickSize);
		AllBricks[_Index.Y][_Index.X].SpriteRenderer = NewSpriteRenderer;
	}

	// ��������Ʈ�� ������ �����Ƿ�.	// �� �̸����� �������ָ� �ȴ�.
	USpriteRenderer* FindSprite = AllBricks[_Index.Y][_Index.X].SpriteRenderer;
	FindSprite->SetSprite(SpriteName, _SpriteIndex);

	FVector2D TileLocation = IndexToBrickLocation(_Index);
	FindSprite->SetComponentScale(_SpriteScale);
	// �Ʒ��ʿ� �������� �������� ���߿� �ȴ�.
	FindSprite->SetOrder(_Index.Y);

	AllBricks[_Index.Y][_Index.X].SpriteRenderer->SetComponentLocation(TileLocation + BrickSize.Half() + _Pivot);
	AllBricks[_Index.Y][_Index.X].Pivot = _Pivot;
	AllBricks[_Index.Y][_Index.X].Scale = _SpriteScale;//���� ������ �������� ����
	AllBricks[_Index.Y][_Index.X].SpriteIndex = _SpriteIndex;
}

// Ư�� ��ġ�� �ش��ϴ� ���� ������ ���
ABrick* BrickEditor::GetBrickRef(FVector2D _Location)
{
	FIntPoint Point = LocationToIndex(_Location);

	return GetBrickRef(Point);
}

ABrick* BrickEditor::GetBrickRef(FIntPoint _Index)
{
	if (true == IsIndexOver(_Index))
	{
		return nullptr;
	}

	return &AllBricks[_Index.Y][_Index.X];
}





void BrickEditor::Serialize(UEngineSerializer& _Ser)
{
	_Ser << BrickCount;
	_Ser << BrickSize;
	_Ser << SpriteName;
	_Ser << AllBricks;

}

void BrickEditor::DeSerialize(UEngineSerializer& _Ser)
{
	_Ser >> BrickCount;
	_Ser >> BrickSize;
	_Ser >> SpriteName;

	std::vector<std::vector<ABrick>> LoadTiles;
	_Ser >> LoadTiles;

	// �����͸� �о����������.
	Create(SpriteName, BrickCount, BrickSize);

	for (int y = 0; y < LoadTiles.size(); y++)
	{
		for (int x = 0; x < LoadTiles[y].size(); x++)
		{
			SetBrickIndex({ x, y }, LoadTiles[y][x].Pivot, LoadTiles[y][x].Scale, LoadTiles[y][x].SpriteIndex);
		}
	}

}


FVector2D BrickEditor::CheckCollision(const FVector2D& playerPos, const FVector2D& playerSize, FIntPoint brickIndex)
{
	// ������ ��ġ�� ũ�⸦ �������� ���� brickIndex�� ����Ͽ� AllBricks���� ����
	if (true == IsIndexOver(brickIndex))
	{
		return FVector2D(0.0f, 0.0f); // �浹 ����
	}
	//FVector2D thisPos = GetTransform().Location;
	auto& brick = AllBricks[brickIndex.Y][brickIndex.X];
	FVector2D brickPos = IndexToBrickLocation(brickIndex);// -thisPos;
	FVector2D brickSize = brick.Scale; // ���� ������ ũ��

	for (int y = 0; y < AllBricks.size(); ++y) {
		for (int x = 0; x < AllBricks[y].size(); ++x) {
			FVector2D brickPos = IndexToBrickLocation({ y, x });
			FVector2D brickSize = AllBricks[y][x].Scale;

			// �α� ���
			UEngineDebug::CoreOutPutString("brickPos[" + std::to_string(y) + "][" + std::to_string(x) + "] : " + brickPos.ToString(), { 30, 300 + y * 100 + x * 20 });
		}
	}

	// �浹 ���� ����
	FVector2D HitResult = (playerPos - brickPos) / brickSize;

	for (int y = 0; y < AllBricks.size(); ++y) {
		for (int x = 0; x < AllBricks[y].size(); ++x) {
			auto& brick = AllBricks[y][x];
			FVector2D brickPos = IndexToBrickLocation({ y, x });
			FVector2D brickSize = brick.Scale;

			// �浹 �˻�
			if (playerPos.X < brickPos.X + brickSize.X &&
				playerPos.X + playerSize.X > brickPos.X &&
				playerPos.Y < brickPos.Y + brickSize.Y &&
				playerPos.Y + playerSize.Y > brickPos.Y) {
				// �浹�� ���� �߰�
				UEngineDebug::CoreOutPutString("Collision with brick at index [" + std::to_string(y) + "][" + std::to_string(x) + "]", { 800, 800 });

				// �ʿ��� ������ ���⿡ �߰� (��: �浹 ó��, ���� �ı� ��)
				//brick.HandleCollision(); // ���÷� ������ �浹 ó�� �޼��带 ȣ��
			}
		}
	}



	if (HitResult.X > 0 && HitResult.Y > 0 && HitResult.X < 1 && HitResult.Y < 1) {
		if (HitResult.X < HitResult.Y) {
			if (HitResult.X > 1 - HitResult.Y) {
				UEngineDebug::CoreOutPutString("HitResult : BOTTOM" + HitResult.ToString(), { 100, 600 });
				return FVector2D(0.0f, -1.0f); // �Ʒ��� �ݻ� ����
			}
			else {
				UEngineDebug::CoreOutPutString("HitResult : LEFT" + HitResult.ToString(), { 100, 600 });
				return FVector2D(-1.0f, 0.0f); // ���� �ݻ� ����
			}
		}
		else if (HitResult.X > HitResult.Y) {
			if (HitResult.Y > 1 - HitResult.X) {
				UEngineDebug::CoreOutPutString("HitResult : RIGHT" + HitResult.ToString(), { 100, 600 });
				return FVector2D(1.0f, 0.0f); // ������ �ݻ� ����
			}
			else {
				UEngineDebug::CoreOutPutString("HitResult : TOP" + HitResult.ToString(), { 100, 600 });
				return FVector2D(0.0f, 1.0f); // ���� �ݻ� ����
			}
		}
	}

	return FVector2D(0.0f, 0.0f); // �浹 ����
}
