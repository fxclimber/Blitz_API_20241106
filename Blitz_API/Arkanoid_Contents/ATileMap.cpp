#include "PreCompiledFile.h"
#include "ATileMap.h"

#include <EnginePlatform/EngineInput.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/EngineAPICore.h>
#include <EngineCore/EngineCoreDebug.h>
#include "ContentsEnum.h"



ATileMap::ATileMap()
{

}

ATileMap::~ATileMap()
{
}

// 타일의 
void ATileMap::Create(std::string_view _Sprite, FIntPoint _Count, FVector2D _TileSize)
{
	SpriteName = _Sprite;
	TileSize = _TileSize;
	TileCount = _Count;

	AllTiles.resize(_Count.Y);

	for (size_t y = 0; y < AllTiles.size(); y++)
	{
		AllTiles[y].resize(_Count.X);;
	}

	// 타일맵의 핵심 0, 0을 두가지를 정해야 한다.
	// 타일로서의 0,0이 어디야.
	// 타일의 위치는 월드로서의 타일맵 * 인덱스가 된다.
	float WinSize = UEngineAPICore::GetCore()->GetMainWindow().GetWindowSize().Half().X;
	float AllSpriteWidth = ( _TileSize.X * static_cast<float>(_Count.X))/2;
	float gap = WinSize - AllSpriteWidth;
	SetActorLocation({ gap ,100.f });

}

FVector2D ATileMap::IndexToTileLocation(FIntPoint _Index)
{
	return FVector2D(_Index.X * TileSize.X, _Index.Y * TileSize.Y);
}

FIntPoint ATileMap::LocationToIndex(FVector2D _Location)
{
	FVector2D Location = _Location / TileSize;

	return FIntPoint(Location.iX(), Location.iY());
}

// 마우스 위치나 플레이어 위치가 들어왔을때.
void ATileMap::SetTileLocation(FVector2D _Location, int _SpriteIndex)
{
	FVector2D TilePos = _Location - GetActorLocation();

	FIntPoint Point = LocationToIndex(TilePos);

	if (true == IsIndexOver(Point))
	{
		return;
	}

	SetTileIndex(Point, _SpriteIndex);
}

bool ATileMap::IsIndexOver(FIntPoint _Index)
{
	if (0 > _Index.X)
	{
		return true;
	}

	if (0 > _Index.Y)
	{
		return true;
	}

	if (TileCount.X - 1 < _Index.X)
	{
		return true;
	}

	if (TileCount.Y - 1 < _Index.Y)
	{
		return true;
	}

	return false;
}

void ATileMap::SetTileIndex(FIntPoint _Index, int _SpriteIndex)
{
	SetTileIndex(_Index, { 0,0 }, TileSize, _SpriteIndex);
}

void ATileMap::SetTileIndex(FIntPoint _Index, FVector2D _Pivot, FVector2D _SpriteScale, int _SpriteIndex)
{
	if (true == IsIndexOver(_Index))
	{
		return;
	}

	// 미리 다 만들지 않고
	// 지금 스프라이트 랜더러를 만든다.
	if (nullptr == AllTiles[_Index.Y][_Index.X].SpriteRenderer)
	{
		USpriteRenderer* NewSpriteRenderer = CreateDefaultSubObject<USpriteRenderer>();
		NewSpriteRenderer->SetComponentScale(TileSize);
		AllTiles[_Index.Y][_Index.X].SpriteRenderer = NewSpriteRenderer;
	}

	// 스프라이트를 저장해 놨으므로.	// 그 이름으로 세팅해주면 된다.
	USpriteRenderer* FindSprite = AllTiles[_Index.Y][_Index.X].SpriteRenderer;
	FindSprite->SetSprite(SpriteName, _SpriteIndex);

	FVector2D TileLocation = IndexToTileLocation(_Index);
	FindSprite->SetComponentScale(_SpriteScale);
	// 아래쪽에 있을수록 랜더링이 나중에 된다.
	FindSprite->SetOrder(_Index.Y);

	AllTiles[_Index.Y][_Index.X].SpriteRenderer->SetComponentLocation(TileLocation + TileSize.Half() + _Pivot);
	AllTiles[_Index.Y][_Index.X].Pivot = _Pivot;
	AllTiles[_Index.Y][_Index.X].Scale = _SpriteScale;
	AllTiles[_Index.Y][_Index.X].SpriteIndex = _SpriteIndex;
}



Tile* ATileMap::GetTileRef(FVector2D _Location)
{
	FIntPoint Point = LocationToIndex(_Location);

	return GetTileRef(Point);
}

// 타일을 내가 직접 이걸로 얻어와서 변형시키면 된다.
Tile* ATileMap::GetTileRef(FIntPoint _Index)
{
	if (true == IsIndexOver(_Index))
	{
		return nullptr;
	}

	return &AllTiles[_Index.Y][_Index.X];
}

void ATileMap::Serialize(UEngineSerializer& _Ser)
{
	// AllTiles

	// 저장하고 싶다.
	// : public ISerializObject 상속
	// void Serialize(UEngineSerializer& _Ser)
	// void DeSerialize(UEngineSerializer & _Ser)
	// 구현
	// 중요데이터 저장하는 코드 직접 짜기

	_Ser << TileCount;
	_Ser << TileSize;
	_Ser << SpriteName;
	_Ser << AllTiles;
}

void ATileMap::DeSerialize(UEngineSerializer& _Ser)
{
	// 기존에 있던것 다 날려버리고 로드해야 한다.
	// 삭제

	// 디시리얼 라이즈는 무조건 역순이어야 합니다.
	// 순서 바꾸면 절대로 안되거나
	_Ser >> TileCount;
	_Ser >> TileSize;
	_Ser >> SpriteName;

	std::vector<std::vector<Tile>> LoadTiles;
	_Ser >> LoadTiles;

	// 데이터만 읽어왔을뿐이지.
	Create(SpriteName, TileCount, TileSize);

	for (int y = 0; y < LoadTiles.size(); y++)
	{
		for (int x = 0; x < LoadTiles[y].size(); x++)
		{
			SetTileIndex({ x, y }, LoadTiles[y][x].Pivot, LoadTiles[y][x].Scale, LoadTiles[y][x].SpriteIndex);
		}
	}

}

