#pragma once
#include <EngineBase/EngineResources.h>
#include <map>
#include "ThirdParty/fmod/Inc/fmod.hpp"

// 사운드 플레이어, 재생하는 사운드에 대한 볼륨등
class USoundPlayer
{
public:
	friend class UEngineSound;

	void On()
	{
		Control->setPaused(false);
	}
	void Off()
	{
		Control->setPaused(true);
	}
	void OnOffSwitch()
	{
		bool Check = false;
		Control->getPaused(&Check);
		if (true == Check)
		{
			Control->setPaused(false);
		}
		else
		{
			Control->setPaused(true);
		}
	}

	void Loop(int Count = -1)
	{
		Control->setLoopCount(Count);
	}


private:
	FMOD::Channel* Control = nullptr;
};

class UEngineSound : public UEngineResources
{
public:
	UEngineSound(){}
	~UEngineSound();

	static void Load(std::string_view _Path);
	static void Load(std::string_view _Name, std::string_view _Path);
	static USoundPlayer Play(std::string_view _Name);
	static UEngineSound* Find(std::string_view _Name);
	static void Release();
	static void Update();// 동시재생기능은 업데이트로.

protected:

private:
	static std::map<std::string, UEngineSound*> Sounds;
	FMOD::Sound* SoundHandle;
	bool ResLoad(std::string_view _Path);
};