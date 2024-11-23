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

private:
	USpriteRenderer* SpriteRender;
};

