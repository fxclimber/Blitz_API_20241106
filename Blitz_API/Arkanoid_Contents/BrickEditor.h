#pragma once
#include <EngineCore/Actor.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineBase/EngineSerializer.h>
#include "Brick.h"

enum BlockType
{
	HPBlock,
	NotBreak,
};

// Brick�� ���� ��ġ�� ũ��� BrickSize, Scale, IndexToBrickLocation,
// SetComponentLocation �Լ����� Ȯ��
class ABrick : public ISerializObject
{
public:
	// �̷� Ŭ������ �����Ҷ�
	// ���� ���δ� �� �����ؾ� �Ѵ�.
	// ���߿��� ������°� ��������?
	// �����Լ� ���̺�
	USpriteRenderer* SpriteRenderer;

	bool IsMove = true;
	int BrickType = -1;
	FVector2D Scale;
	FVector2D Pivot;
	int SpriteIndex;
	unsigned int HP = -1;

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
		_Ser << BrickType;
		_Ser << Scale;
		_Ser << Pivot;
		_Ser << SpriteIndex;
	}

	void DeSerialize(UEngineSerializer& _Ser)
	{
		std::string SpriteName;
		_Ser >> SpriteName;
		_Ser >> IsMove;
		_Ser >> BrickType;
		_Ser >> Scale;
		_Ser >> Pivot;
		_Ser >> SpriteIndex;
	}

};

// ������� ���� ���� ���ϴ� ���.
// �÷��̾ ��� Ÿ�� ���� �����ϴ��� �˰� �;��.
class BrickEditor : public AActor, public ISerializObject
{
public:
	// constrcuter destructer
	BrickEditor(){}
	~BrickEditor(){}


	//             100, 100                  64 64 
	// Ÿ�� �̹����� sprite 1������ 
	void Create(std::string_view _Sprite, FIntPoint _Count, FVector2D _BrickSize);

	// ���⿡��    �� �ε������ٰ�    �̽�������Ʈ 
	void SetBrickLocation(FVector2D _Location, int _SpriteIndex);

	void SetBrickIndex(FIntPoint _Index, int _SpriteIndex);
	// _SpriteScale : ���� ����ũ��, ���� ������ġ ���
	void SetBrickIndex(FIntPoint _Index, FVector2D _Pivot, FVector2D _SpriteScale, int _SpriteIndex);

	ABrick* GetBrickRef(FIntPoint _Index);
	ABrick* GetBrickRef(FVector2D _Location);

	FVector2D IndexToBrickLocation(FIntPoint _Index);

	FIntPoint LocationToIndex(FVector2D _Location);

	bool IsIndexOver(FIntPoint _Index);

	// �����͸� ����ȭ(����)
	void Serialize(UEngineSerializer& _Ser);
	// �����͸� ����(�Ҷ�)
	void DeSerialize(UEngineSerializer& _Ser);

	FVector2D CheckCollision(class ABall* Ball, FIntPoint brickIndex);

	FVector2D CheckCollision(const FVector2D& playerPos, const FVector2D& playerSize, FIntPoint brickIndex);

	FVector2D GetBrickSize()
	{
		return BrickSize;
	}
	FIntPoint GetBrickCount()
	{
		return BrickCount;
	}

	void RemoveBlock(FIntPoint brickIndex);

protected:

private:
	// �����̴ϱ� �׳� ���ܹ����°�.
	FIntPoint BrickCount;
	std::string SpriteName;
	FVector2D BrickSize;//���� ����ũ��
	FVector2D PlusPos;
	std::vector<std::vector<ABrick>> AllBricks;
};


