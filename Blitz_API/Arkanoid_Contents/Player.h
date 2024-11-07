#pragma once
#include <EngineCore/Actor.h>

class APlayer : public AActor
{
public:
	// constrcuter destructer
	APlayer();
	~APlayer();

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void MoveFunction(FVector2D _Dir);

	void RunSoundPlay();

	bool IsMoving();

	USpriteRenderer* GetRender()
	{
		return SpriteRenderer;
	}

protected:

private:

	float Speed = 100.0f;
	int MySpriteIndex = 0;

	class USpriteRenderer* SpriteRenderer;


};

