#pragma once
#include <EngineCore/Actor.h>

class Map_Play : public AActor
{
public:
	Map_Play();
	~Map_Play(){}
	USpriteRenderer* GetRender()
	{
		return SpriteRender;
	}

protected:

	virtual void BeginPlay()override;
	virtual void Tick(float _DeltaTime)override;

private:
	USpriteRenderer* SpriteRender;
	USpriteRenderer* SpriteRenderScore;
	USpriteRenderer* SpriteRenderTime;
};

