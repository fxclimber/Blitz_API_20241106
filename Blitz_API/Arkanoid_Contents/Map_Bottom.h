#pragma once
#include <EngineCore/Actor.h>


class Map_Bottom : public AActor
{
public:
	Map_Bottom();
	~Map_Bottom(){}

	//void BeginPlay() override;
	//void Tick(float _DeltaTime) override;
	USpriteRenderer* GetRender()
	{
		return SpriteRender;
	}

protected:

private:
	USpriteRenderer* SpriteRender;



};

