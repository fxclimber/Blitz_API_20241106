#pragma once
#include "Letter.h"



class UI : public AActor
{
public:
	UI();
	~UI(){}

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

