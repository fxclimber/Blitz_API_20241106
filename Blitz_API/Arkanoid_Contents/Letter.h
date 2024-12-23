#pragma once
#include <vector>

#include <EngineCore/Actor.h>
#include <EngineCore/ImageManager.h>
#include <EngineCore/SpriteRenderer.h>

class ALetter : public AActor
{
public:
	ALetter();
	~ALetter();

	void SetTextSpriteName(const std::string _Text);

	void SetTextScale(FVector2D _TextScale)
	{
		TextScale = _TextScale;
	}

	template<typename EnumType>
	void SetOrder(EnumType _Order)
	{
		SetOrder(static_cast<int>(_Order));
	}

	void SetOrder(int _Order);

	void SetValue(int _Score);
	void SetFinalValue(int _FinalScore)
	{
		FinalScoreUI = _FinalScore;
	}
	void SetText(const std::string& InputText);

	std::vector<class USpriteRenderer*>& GetRenders()
	{
		return Renders;
	}

	static int ScoreUI;
	static int FinalScoreUI;

protected:
	void BeginPlay() override {}
	void Tick(float _DeltaTime) override {}

private:

	std::string TextSpriteName;
	FVector2D TextScale;
	std::vector<class USpriteRenderer*> Renders;


};

