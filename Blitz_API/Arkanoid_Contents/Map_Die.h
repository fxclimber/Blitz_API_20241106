#pragma once
#include <EngineCore/Actor.h>

class Map_Die : public AActor
{
public:
	Map_Die();
	~Map_Die(){}

	void Tick(float _DeltaTime) override;


};

