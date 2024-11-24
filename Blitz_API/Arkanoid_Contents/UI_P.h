#pragma once
#include "Letter.h"



class UI_P : public AActor
{
public:
	UI_P();
	~UI_P() {}

	std::vector<ALetter*> GetLetter()
	{
		return Letters;
	}

protected:
	virtual void BeginPlay()override;
	virtual void Tick(float _DeltaTime)override;
private:
	class USpriteRenderer* SpriteRenderer = nullptr;

	std::vector<ALetter*> Letters;

	class AFade* Fade = nullptr;
	class ATestGameMode* gamemode;

	ALetter* letter1 = nullptr;
	ALetter* letter2 = nullptr;
	ALetter* letter3 = nullptr;
};

