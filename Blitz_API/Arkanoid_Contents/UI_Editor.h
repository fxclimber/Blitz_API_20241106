#pragma once
#include "Letter.h"




class UI_Editor : public AActor
{
public:
	UI_Editor();
	~UI_Editor() {}

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
	class BrickEditorGameMode* gamemode;

	ALetter* letter1 = nullptr;
	ALetter* letter2 = nullptr;
	ALetter* letter3 = nullptr;

	class USpriteRenderer* SpriteEdit =nullptr;

};
