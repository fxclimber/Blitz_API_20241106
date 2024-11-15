#include "PreCompiledFile.h"
#include "EngineSound.h"
#include <EngineBase/EngineString.h>
#include <EngineBase/EngineDebug.h>

std::map<std::string, UEngineSound*> UEngineSound::Sounds;

// 사운드 제어 핸들
FMOD::System* SoundSystem = nullptr;

#ifdef _DEBUG
#pragma comment(lib, "fmod_vc.lib")
#else
#pragma comment(lib, "fmod_vc.lib")
#endif

class SoundInit
{
public:
	SoundInit()
	{
		if (FMOD_RESULT::FMOD_OK != FMOD::System_Create(&SoundSystem))
		{
			MSGASSERT("FMOD 시스템 이닛에 실패");
			return;
		}
		// 사운드 채널 설정, 
		// int maxchannels 동시재생
		// FMOD_INITFLAGS flags 지정사항
		// void* extradriverdata 지정사항에 넣을 데이타
		if (FMOD_RESULT::FMOD_OK != SoundSystem->init(32, FMOD_DEFAULT, nullptr))
		{
			MSGASSERT("FMOD 시스템 이닛에 실패");
			return;
		}

	}
};

SoundInit SoundInitObject;

void UEngineSound::Update()
{
	if (nullptr == SoundSystem)
	{
		return;
	}
	if (FMOD_RESULT::FMOD_OK != SoundSystem->update())
	{
		MSGASSERT("FMOD시스템 업데이트에 이상감지")
	}
}
// 엔진 끝날때 직접 호출
void UEngineSound::Release()
{
	std::map<std::string, UEngineSound*>::iterator StartIter = Sounds.begin();
	std::map<std::string, UEngineSound*>::iterator EndIter = Sounds.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		if (nullptr != StartIter->second)
		{
			delete StartIter->second;
			StartIter->second = nullptr;
		}
	}
	Sounds.clear();
	if (nullptr != SoundSystem)
	{
		SoundSystem->release();
		SoundSystem = nullptr;
	}
}


// 경로 저장
void UEngineSound::Load(std::string_view _Path)
{
	UEnginePath EnginePath = UEnginePath(_Path);
	std::string FileName = EnginePath.GetFileName();
	UEngineSound::Load(FileName.data(), _Path);
}
// 대문자로 변경, 사운드객체 생성
void UEngineSound::Load(std::string_view _Name, std::string_view _Path)
{
	std::string UpperString = UEngineString::ToUpper(_Name);
	UEngineSound* NewSound = new UEngineSound();

	if (false == NewSound->ResLoad(_Path))
	{
		delete NewSound;
		MSGASSERT("사운드로드에 실패"+UpperString);
		return;
	}
	UEngineSound::Sounds.insert({ UpperString, NewSound });
}

UEngineSound* UEngineSound::Find(std::string_view _Name)
{
	std::string UpperString = UEngineString::ToUpper(_Name);
	if (false == Sounds.contains(UpperString))
	{
		return nullptr;
	}
	return Sounds[UpperString];
}

// 사운드 플레이어객체 생성
USoundPlayer UEngineSound::Play(std::string_view _Name)
{
	std::string UpperString = UEngineString::ToUpper(_Name);
	UEngineSound* FindSound = Find(_Name);
	if (nullptr == FindSound)
	{
		MSGASSERT("로드하지 않은 사운드" + UpperString);
	}

	FMOD::Channel* Ch = nullptr;
	SoundSystem->playSound(FindSound->SoundHandle, nullptr, false, &Ch);

	//1번 재생
	Ch->setLoopCount(0);
	Ch->setVolume(1.0f);

	USoundPlayer NewPlayer;
	NewPlayer.Control = Ch;
	return NewPlayer;
}



bool UEngineSound::ResLoad(std::string_view _Path)
{
	SoundSystem->createSound(_Path.data(), FMOD_LOOP_NORMAL, nullptr, &SoundHandle);

	if (nullptr == SoundHandle)
	{
		MSGASSERT("사운드로드실패" + std::string(_Path));
		return false;
	}
	return true;
}
