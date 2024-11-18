#pragma once


class DieGameMode : public AGameMode
{
public:

	DieGameMode();
	~DieGameMode(){}
	bool GetIsOpenLevel()
	{
		return IsOpenLevel;
	}

protected:
	void BeginPlay() override;

	void Tick(float _DeltaTime) override;
	bool IsOpenLevel=false;

};

