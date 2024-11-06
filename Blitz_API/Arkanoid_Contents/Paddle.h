#pragma once
#include <EngineCore/Actor.h>

class Paddle : public AActor
{
public:
	Paddle();
	~Paddle();

	void BeginPlay()override;
	void Tick(float _DeltaTime) override;

	void MoveFunction(FVector2D _Dir);

	void RunSoundPlay();

	void LevelChangeStart();
	void LevelChangeEnd();

protected:

private:
	float Speed = 100.0f;
	int MySpriteIndex = 0;

	class USpriteRenderer* SpriteRenderer;

};
