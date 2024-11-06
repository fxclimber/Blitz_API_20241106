#pragma once
#include <EngineCore/Actor.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineBase/EngineSerializer.h>
#include "Brick.h"


// 그려지는 걸 뭘 기준으로 할거냐.
// 다수의 이미지가 일정 규격으로 
// 아예 UTileMapRenderer라는 걸 만든다.
// Y소팅이 필요없는 사람만 하세요. 
// UTileMapRenderer 방식이 더 빠를수 있다.
// 되고나서 시작부터 하는게 아니고 되고나서 바꾸는거에요.
// 맘에 들지 않지만 일단 컨텐츠적으로 됐어.
// 그럼 시간 남으면 하시면 됩니다.
// 
// [][][][][][][][][][][]
// [][][][][][][][][][][]
// [][][][][][][][][][][]
// [][][][][][][][][][][]
// [][][][][][][][][][][]
// [][][][][][][][][][][]
// [][][][][][][][][][][]
// [][][][][][][][][][][]

// 값형은 그냥 저장해도 된다.
class Tile : public ISerializObject
{
public:
	// 이런 클래스를 저장할때
	// 내부 정부는 다 저장해야 한다.
	// 그중에서 쓸모없는게 누군가요?
	// 가상함수 테이블
	USpriteRenderer* SpriteRenderer;

	bool IsMove = true;
	int TileType = -1;
	// 타일하나하나가 개별적인 크기를 가지고 있다면
	// 이걸로 직접 입력해주셔야 합니다.
	FVector2D Scale;
	FVector2D Pivot;
	int SpriteIndex;

	// 데이터를 직렬화(압축)
	void Serialize(UEngineSerializer& _Ser)
	{
		std::string SpriteName;
		if (nullptr != SpriteRenderer)
		{
			SpriteName = SpriteRenderer->GetCurSpriteName();
		}
		_Ser << SpriteName;
		_Ser << IsMove;
		_Ser << TileType;
		_Ser << Scale;
		_Ser << Pivot;
		_Ser << SpriteIndex;
	}

	void DeSerialize(UEngineSerializer& _Ser)
	{
		//std::string SpriteName;
		// _Ser >> SpriteName;

		// SpriteRenderer->SetSprite(SpriteName);

		std::string SpriteName;
		_Ser >> SpriteName;
		_Ser >> IsMove;
		_Ser >> TileType;
		_Ser >> Scale;
		_Ser >> Pivot;
		_Ser >> SpriteIndex;

	}

};

// 사람들이 가장 먼저 원하는 기능.
// 플레이어가 어느 타일 위에 존재하는지 알고 싶어요.

// 설명 :
class ATileMap : public AActor, public ISerializObject
{
public:
	// constrcuter destructer
	ATileMap();
	~ATileMap();

	//             100, 100                  64 64 
	// 타일 이미지는 sprite 1개에서 
	void Create(std::string_view _Sprite, FIntPoint _Count, FVector2D _TileSize);

	// 여기에서    이 인덱스에다가    이스프라이트 

	void SetTileLocation(FVector2D _Location, int _SpriteIndex);

	void SetTileIndex(FIntPoint _Index, int _SpriteIndex);
	void SetTileIndex(FIntPoint _Index, FVector2D _Pivot, FVector2D _SpriteScale, int _SpriteIndex);

	Tile* GetTileRef(FIntPoint _Index);
	Tile* GetTileRef(FVector2D _Location);

	FVector2D IndexToTileLocation(FIntPoint _Index);

	FIntPoint LocationToIndex(FVector2D _Location);

	bool IsIndexOver(FIntPoint _Index);

	// 데이터를 직렬화(압축)
	void Serialize(UEngineSerializer& _Ser);
	// 데이터를 복구(할때)
	void DeSerialize(UEngineSerializer& _Ser);

protected:

private:
	// 메모리를 아끼려면 Map
	// 타일이 존재하지 않다가 타일 생성 되야해.

	// 안아낄려면 Vector Vector
	// 봄버맨, 크아, 벽돌깨기, 바바

	// 구현하는 사람들마다 가지각색

	// 두가지를 혼합하겠습니다.
	// 벡터는 확장시키겠지만 SPriteRenderer는 만들지 않을 겁니다.
	// 저는 빠르고 쉬운거.

	// 최적화 방식 화면에 보이는 크기만큼의 랜더러만 들고.
	// 랜더만 안만드는 방식으로 가겠습니다.

	// 값형이니까 그냥 생겨버리는것.
	FIntPoint TileCount;
	std::string SpriteName;
	FVector2D TileSize;
	std::vector<std::vector<Tile>> AllTiles;
};


