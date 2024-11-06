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

	// 타일맵의 핵심 0, 0을 두가지를 정해야 한다.
	// 타일로서의 0,0이 어디야.
	// 타일의 위치는 월드로서의 타일맵 * 인덱스가 된다.
	float WinSize = UEngineAPICore::GetCore()->GetMainWindow().GetWindowSize().Half().X;
	//float WinSize = Globals::WinSize.X;//터지냐....왜........
	float AllSpriteWidth = (_BrickSize.X * static_cast<float>(_Count.X)) / 2;
	float gap = WinSize - AllSpriteWidth;
	SetActorLocation({ gap ,100.f });
}

FVector2D BrickEditor::IndexToBrickLocation(FIntPoint _Index)
{
	//인덱스를 기반으로 벽돌의 위치를 계산
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

// _SpriteScale : 벽돌 개별크기, 벽돌 개별위치 계산
void BrickEditor::SetBrickIndex(FIntPoint _Index, FVector2D _Pivot, FVector2D _SpriteScale, int _SpriteIndex)
{
	if (true == IsIndexOver(_Index))
	{
		return;
	}

	// 미리 다 만들지 않고
	// 지금 스프라이트 랜더러를 만든다.
	if (nullptr == AllBricks[_Index.Y][_Index.X].SpriteRenderer)
	{
		USpriteRenderer* NewSpriteRenderer = CreateDefaultSubObject<USpriteRenderer>();
		NewSpriteRenderer->SetComponentScale(BrickSize);
		AllBricks[_Index.Y][_Index.X].SpriteRenderer = NewSpriteRenderer;
	}

	// 스프라이트를 저장해 놨으므로.	// 그 이름으로 세팅해주면 된다.
	USpriteRenderer* FindSprite = AllBricks[_Index.Y][_Index.X].SpriteRenderer;
	FindSprite->SetSprite(SpriteName, _SpriteIndex);

	FVector2D TileLocation = IndexToBrickLocation(_Index);
	FindSprite->SetComponentScale(_SpriteScale);
	// 아래쪽에 있을수록 랜더링이 나중에 된다.
	FindSprite->SetOrder(_Index.Y);

	AllBricks[_Index.Y][_Index.X].SpriteRenderer->SetComponentLocation(TileLocation + BrickSize.Half() + _Pivot);
	AllBricks[_Index.Y][_Index.X].Pivot = _Pivot;
	AllBricks[_Index.Y][_Index.X].Scale = _SpriteScale;//개별 벽돌의 스케일을 저장
	AllBricks[_Index.Y][_Index.X].SpriteIndex = _SpriteIndex;
}

// 특정 위치에 해당하는 벽돌 참조를 얻기
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

	// 데이터만 읽어왔을뿐이지.
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
	// 벽돌의 위치와 크기를 가져오기 위해 brickIndex를 사용하여 AllBricks에서 접근
	if (true == IsIndexOver(brickIndex))
	{
		return FVector2D(0.0f, 0.0f); // 충돌 없음
	}

	auto& brick = AllBricks[brickIndex.Y][brickIndex.X];
	FVector2D brickPos = IndexToBrickLocation(brickIndex);
	FVector2D brickSize = brick.Scale; // 개별 벽돌의 크기

	UEngineDebug::CoreOutPutString("brickPos" + brickPos.ToString(), { 100, 600 });
	UEngineDebug::CoreOutPutString("brickSize" + brickSize.ToString(), { 100, 650 });


	// 충돌 감지 로직
	FVector2D HitResult = (playerPos - brickPos) / brickSize;
	if (HitResult.X > 0 && HitResult.Y > 0 && HitResult.X < 1 && HitResult.Y < 1) {
		if (HitResult.X < HitResult.Y) {
			if (HitResult.X > 1 - HitResult.Y) {
				UEngineDebug::CoreOutPutString("HitResult : BOTTOM" + HitResult.ToString(), { 100, 600 });
				return FVector2D(0.0f, -1.0f); // 아래쪽 반사 벡터
			}
			else {
				UEngineDebug::CoreOutPutString("HitResult : LEFT" + HitResult.ToString(), { 100, 600 });
				return FVector2D(-1.0f, 0.0f); // 왼쪽 반사 벡터
			}
		}
		else if (HitResult.X > HitResult.Y) {
			if (HitResult.Y > 1 - HitResult.X) {
				UEngineDebug::CoreOutPutString("HitResult : RIGHT" + HitResult.ToString(), { 100, 600 });
				return FVector2D(1.0f, 0.0f); // 오른쪽 반사 벡터
			}
			else {
				UEngineDebug::CoreOutPutString("HitResult : TOP" + HitResult.ToString(), { 100, 600 });
				return FVector2D(0.0f, 1.0f); // 위쪽 반사 벡터
			}
		}
	}

	return FVector2D(0.0f, 0.0f); // 충돌 없음
}
