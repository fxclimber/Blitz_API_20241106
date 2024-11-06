#pragma once
#include <EngineCore/Actor.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineBase/EngineSerializer.h>
#include "Brick.h"


// �׷����� �� �� �������� �Ұų�.
// �ټ��� �̹����� ���� �԰����� 
// �ƿ� UTileMapRenderer��� �� �����.
// Y������ �ʿ���� ����� �ϼ���. 
// UTileMapRenderer ����� �� ������ �ִ�.
// �ǰ��� ���ۺ��� �ϴ°� �ƴϰ� �ǰ��� �ٲٴ°ſ���.
// ���� ���� ������ �ϴ� ������������ �ƾ�.
// �׷� �ð� ������ �Ͻø� �˴ϴ�.
// 
// [][][][][][][][][][][]
// [][][][][][][][][][][]
// [][][][][][][][][][][]
// [][][][][][][][][][][]
// [][][][][][][][][][][]
// [][][][][][][][][][][]
// [][][][][][][][][][][]
// [][][][][][][][][][][]

// ������ �׳� �����ص� �ȴ�.
class Tile : public ISerializObject
{
public:
	// �̷� Ŭ������ �����Ҷ�
	// ���� ���δ� �� �����ؾ� �Ѵ�.
	// ���߿��� ������°� ��������?
	// �����Լ� ���̺�
	USpriteRenderer* SpriteRenderer;

	bool IsMove = true;
	int TileType = -1;
	// Ÿ���ϳ��ϳ��� �������� ũ�⸦ ������ �ִٸ�
	// �̰ɷ� ���� �Է����ּž� �մϴ�.
	FVector2D Scale;
	FVector2D Pivot;
	int SpriteIndex;

	// �����͸� ����ȭ(����)
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

// ������� ���� ���� ���ϴ� ���.
// �÷��̾ ��� Ÿ�� ���� �����ϴ��� �˰� �;��.

// ���� :
class ATileMap : public AActor, public ISerializObject
{
public:
	// constrcuter destructer
	ATileMap();
	~ATileMap();

	//             100, 100                  64 64 
	// Ÿ�� �̹����� sprite 1������ 
	void Create(std::string_view _Sprite, FIntPoint _Count, FVector2D _TileSize);

	// ���⿡��    �� �ε������ٰ�    �̽�������Ʈ 

	void SetTileLocation(FVector2D _Location, int _SpriteIndex);

	void SetTileIndex(FIntPoint _Index, int _SpriteIndex);
	void SetTileIndex(FIntPoint _Index, FVector2D _Pivot, FVector2D _SpriteScale, int _SpriteIndex);

	Tile* GetTileRef(FIntPoint _Index);
	Tile* GetTileRef(FVector2D _Location);

	FVector2D IndexToTileLocation(FIntPoint _Index);

	FIntPoint LocationToIndex(FVector2D _Location);

	bool IsIndexOver(FIntPoint _Index);

	// �����͸� ����ȭ(����)
	void Serialize(UEngineSerializer& _Ser);
	// �����͸� ����(�Ҷ�)
	void DeSerialize(UEngineSerializer& _Ser);

protected:

private:
	// �޸𸮸� �Ƴ����� Map
	// Ÿ���� �������� �ʴٰ� Ÿ�� ���� �Ǿ���.

	// �ȾƳ����� Vector Vector
	// ������, ũ��, ��������, �ٹ�

	// �����ϴ� ����鸶�� ��������

	// �ΰ����� ȥ���ϰڽ��ϴ�.
	// ���ʹ� Ȯ���Ű������ SPriteRenderer�� ������ ���� �̴ϴ�.
	// ���� ������ �����.

	// ����ȭ ��� ȭ�鿡 ���̴� ũ�⸸ŭ�� �������� ���.
	// ������ �ȸ���� ������� ���ڽ��ϴ�.

	// �����̴ϱ� �׳� ���ܹ����°�.
	FIntPoint TileCount;
	std::string SpriteName;
	FVector2D TileSize;
	std::vector<std::vector<Tile>> AllTiles;
};


