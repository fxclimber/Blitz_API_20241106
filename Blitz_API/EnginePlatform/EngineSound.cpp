#include "PreCompiledFile.h"
#include "EngineSound.h"
#include <EngineBase/EngineString.h>
#include <EngineBase/EngineDebug.h>

std::map<std::string, UEngineSound*> UEngineSound::Sounds;

// ���� ���� �ڵ�
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
			MSGASSERT("FMOD �ý��� �̴ֿ� ����");
			return;
		}
		// ���� ä�� ����, 
		// int maxchannels �������
		// FMOD_INITFLAGS flags ��������
		// void* extradriverdata �������׿� ���� ����Ÿ
		if (FMOD_RESULT::FMOD_OK != SoundSystem->init(32, FMOD_DEFAULT, nullptr))
		{
			MSGASSERT("FMOD �ý��� �̴ֿ� ����");
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
		MSGASSERT("FMOD�ý��� ������Ʈ�� �̻���")
	}
}
// ���� ������ ���� ȣ��
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


// ��� ����
void UEngineSound::Load(std::string_view _Path)
{
	UEnginePath EnginePath = UEnginePath(_Path);
	std::string FileName = EnginePath.GetFileName();
	UEngineSound::Load(FileName.data(), _Path);
}
// �빮�ڷ� ����, ���尴ü ����
void UEngineSound::Load(std::string_view _Name, std::string_view _Path)
{
	std::string UpperString = UEngineString::ToUpper(_Name);
	UEngineSound* NewSound = new UEngineSound();

	if (false == NewSound->ResLoad(_Path))
	{
		delete NewSound;
		MSGASSERT("����ε忡 ����"+UpperString);
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

// ���� �÷��̾ü ����
USoundPlayer UEngineSound::Play(std::string_view _Name)
{
	std::string UpperString = UEngineString::ToUpper(_Name);
	UEngineSound* FindSound = Find(_Name);
	if (nullptr == FindSound)
	{
		MSGASSERT("�ε����� ���� ����" + UpperString);
	}

	FMOD::Channel* Ch = nullptr;
	SoundSystem->playSound(FindSound->SoundHandle, nullptr, false, &Ch);

	//1�� ���
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
		MSGASSERT("����ε����" + std::string(_Path));
		return false;
	}
	return true;
}
