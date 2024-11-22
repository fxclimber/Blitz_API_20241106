#pragma once
#include <EngineCore/Actor.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineBase/EngineSerializer.h>
#include "Brick.h"
#include <chrono>
#include <EnginePlatform/EngineSound.h>


// Hp�� sprite�� �����ϴ� enum
enum BrickType
{
	Default,
	HPBrick,
	NotBreak,
};

// Brick�� ���� ��ġ�� ũ��� BrickSize, Scale, IndexToBrickLocation,
// SetComponentLocation �Լ����� Ȯ��
class ABrick : public ISerializObject
{
public:
	USpriteRenderer* SpriteRenderer;

	int BrickType = -1;
	FVector2D Scale;
	FVector2D Pivot;
	int HP = 400001;

	// �����͸� ����ȭ(����)
	void Serialize(UEngineSerializer& _Ser)
	{
		std::string SpriteName;
		if (nullptr != SpriteRenderer)
		{
			SpriteName = SpriteRenderer->GetCurSpriteName();
		}
		_Ser << SpriteName;
		_Ser << BrickType;
		_Ser << Scale;
		_Ser << Pivot;
	}
	// ����ȭ ����Ÿ �ε�
	void DeSerialize(UEngineSerializer& _Ser)
	{
		std::string SpriteName;
		_Ser >> SpriteName;
		_Ser >> BrickType;
		_Ser >> Scale;
		_Ser >> Pivot;
	}
};

class BrickEditor : public AActor, public ISerializObject
{
public:
	BrickEditor();
	~BrickEditor()
	{
	}

	void Tick(float _DeltaTime) override;

	//hp set
	void SetBrickHp(FIntPoint _Index, int _Hp);
	// sprite set
	void SetBrickSprite(FIntPoint _Index, std::string_view _Sprite, int _SpriteIndex);
	// brick type set (hp,sprite)
	void setBrickType(FIntPoint _Index, BrickType _Type);
	// �ڷᱸ���� �ֱ�,�ڷᱸ�� ũ������,����(��������)��ġ��
	BrickEditor* Create(std::string_view _Sprite, FIntPoint _Count, FVector2D _BrickSize);
	// ��ġ,�ε��� ���ؼ�, SetBrickIndexȣ�� 
	void SetBrickLocation(FVector2D _Location, BrickType _Type);
	// �����ε� SetBrickIndexȣ��-����
	void SetBrickIndex(FIntPoint _Index, int _SpriteIndex, int _HP);
	// ���� ��������Ʈ ���õ�.
	void SetBrickIndex(FIntPoint _Index, FVector2D _Pivot, FVector2D _SpriteScale, int _SpriteIndex, int _HP);
	// ������ ��ġ�� ����Ͽ� BrickPositions�� ����
	FVector2D IndexToBrickLocation(FIntPoint _Index);
	// ������ġ/����ũ�� �� �ε����� ��ġ
	FIntPoint LocationToIndex(FVector2D _Location);
	// �ε����� ����x,y�������� �۰�,0���� Ŀ���Ѵ�
	bool IsIndexOver(FIntPoint _Index);

	int GetBrickType(const ABrick& brick)
	{
		return brick.BrickType; // ���޵� ������ Ÿ�� ��ȯ
	}

	ABrick* GetBrickRef(FIntPoint _Index);
	ABrick* GetBrickRef(FVector2D _Location);

	// �����͸� ����ȭ(����)
	void Serialize(UEngineSerializer& _Ser);
	// �����͸� ����(�Ҷ�)
	void DeSerialize(UEngineSerializer& _Ser);
	// ���Ӹ�忡�� ȣ���ϴ� �浹üũ�Լ�
	FVector2D CheckCollision(class ABall* Ball, FIntPoint brickIndex);
	// �ε����� ������ Ȯ�ο����θ�.
	FVector2D CheckCollision(const FVector2D& playerPos, const FVector2D& playerSize, FIntPoint brickIndex);

	FVector2D GetBrickSize()
	{
		return BrickSize;
	}
	FIntPoint GetBrickCount()
	{
		return BrickCount;
	}
	// �ε���,������Ʈ(������)üũ�ϰ�, �ǹ����ȿ��,hp==0�϶� destroy
	void RemoveBlock(FIntPoint brickIndex);
	// �������� ������ġx�� �ڵ����� �߽ɿ� ���� �������Ƿ�, y�� �ǵ������� �����ϱ�����
	void SetBricksHeight(float _height)
	{
		Height = _height;
	}
	// ������ ���Ӹ��-����÷��̿��� �����ϴ����� ���Ǻ��� ���������ϰ� �Լ��� ����
	void SpawnFX(FVector2D _brickPos);
	// �÷��̽ð� ���� �ð��Լ� 
	float GetElapsedTime() const;
	// ���Ӹ�忡�� ui�� �ѱ� ����
	int GetScore()const
	{
		return Score;
	}
	// ������� ����Ʈ������ �������� �ٸ��� �ֱ�����.
	int RandomBrick = 1;

	bool GetGameClear()const
	{
		return GameClear;
	}

	FIntPoint BrickCount;
	int GetDeathCount()
	{
		return DeathCount;
	}

	std::vector<std::vector<ABrick>>& GetAllBricks()
	{
		return AllBricks;
	}

protected:

private:
	std::string SpriteName;
	FVector2D BrickSize;//���� ����ũ��

	float Height = 0;// ���� ������ġy ������
	FVector2D PlusPos;//���� ��ġ ������

	std::vector<std::vector<ABrick>> AllBricks;
	std::vector<std::vector<FVector2D>> BrickPositions;// �̳� (��ġ) �����
	// b������ ���ʽ��� ����
	class Brick* BonusA;
	FVector2D brickPos;// ���������� ��ġ

	WhereIsBall ballPosition = WhereIsBall::BOTTOM;
	std::chrono::time_point<std::chrono::steady_clock> stageStartTime;
	int Score = 0;
	bool GameClear =false;
	USoundPlayer BallSoundPlayer;
	int DeathCount = 0;

	
};


