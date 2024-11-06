#pragma once
#include <EngineCore/Actor.h>


class APlayMap : public AActor
{
public:
	APlayMap();
	~APlayMap();
	void Tick(float _DeltaTime) override;

public:

protected:

private:
	int MySpriteIndex = 0;
	class USpriteRenderer* SpriteRenderer;


};

