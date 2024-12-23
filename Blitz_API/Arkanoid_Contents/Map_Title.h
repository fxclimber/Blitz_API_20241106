#pragma once
#include <EngineCore/Actor.h>
#include <EnginePlatform/EngineSound.h>


class USpriteRenderer;
class USoundPlayer;

class Map_Title : public AActor
{
public:
	Map_Title();
	~Map_Title(){}

	USpriteRenderer* GetRender()
	{
		return SpriteRender;
	}

protected:

	virtual void BeginPlay()override;
	virtual void Tick(float _DeltaTime)override;

private:
	class USpriteRenderer* SpriteRender;
	USoundPlayer BGM;

};

