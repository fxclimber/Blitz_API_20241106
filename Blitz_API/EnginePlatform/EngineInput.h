#pragma once
#include <EngineBase/EngineDelegate.h>
#include <EngineBase/EngineDebug.h>
#include <vector>
#include <functional>

enum class KeyEvent
{
	Down,
	Press,
	Free,
	Up,
};


class UEngineInput
{
public:
	~UEngineInput();

#pragma region 포인터형

	//static UEngineInput* GetInst()
	//{
	//	if (nullptr == Inst)
	//	{
	//		Inst = new UEngineInput();
	//	}
	//	return Inst;
	//}

	//static void DestroyInst()
	//{
	//	if (nullptr == Inst)
	//	{
	//		delete Inst;
	//		Inst = nullptr;
	//	}
	//}

#pragma endregion

	static UEngineInput& GetInst()
	{
		static UEngineInput Inst = UEngineInput();
		return Inst;
	}





private:
	class UEngineKey
	{
	public:
		int Key = -1;
		//안눌리다가 처음 눌렸을때
		bool IsDown = false;
		//눌린 이후로 계속 누를때
		bool IsPress = false;
		// 눌리다가 뗏을때
		bool IsUp = false;
		//안누르고 있을때
		bool IsFree = true;

		float PressTime = 0.0f;

		std::vector<std::function<void(float)>> PressEvents;
		std::vector<std::function<void(float)>> DownEvents;
		std::vector<std::function<void(float)>> UpEvents;
		std::vector<std::function<void(float)>> FreeEvents;

		UEngineKey()
		{

		}

		UEngineKey(int _Key)
			: Key(_Key)
		{

		}

		void EventCheck(float _DeltaTime);

		void KeyCheck(float _DeltaTime);

	};


public:
	void KeyCheck(float _DeltaTime);
	void EventCheck(float _DeltaTiem);

	bool IsDown(int _KeyIndex)
	{
		if (false == Keys.contains(_KeyIndex))
		{
			MSGASSERT("아직도 등록되지 않은키가 존재합니다");
			return false;
		}
		return Keys[_KeyIndex].IsDown;
	}

	bool IsUp(int _KeyIndex)
	{
		if (false == Keys.contains(_KeyIndex))
		{
			MSGASSERT("아직도 등록되지 않은키가 존재합니다");
			return false;
		}
		return Keys[_KeyIndex].IsUp;
	}

	bool IsPress(int _KeyIndex)
	{
		if (false == Keys.contains(_KeyIndex))
		{
			MSGASSERT("아직도 등록되지 않은키가 존재합니다");
			return false;
		}
		return Keys[_KeyIndex].IsPress;
	}

	float IsPressTime(int _KeyIndex)
	{
		if (false == Keys.contains(_KeyIndex))
		{
			MSGASSERT("아직도 등록되지 않은키가 존재합니다");
			return false;
		}
		return Keys[_KeyIndex].PressTime;
	}

	bool IsFree(int _KeyIndex)
	{
		if (false == Keys.contains(_KeyIndex))
		{
			MSGASSERT("아직도 등록되지 않은키가 존재합니다");
			return false;
		}
		return Keys[_KeyIndex].IsFree;
	}

	void BindAction(int _KeyIndex, KeyEvent _EventType, std::function<void(float)> _Function);
#pragma region 포인터형
	//static UEngineInput* Inst;
#pragma endregion

protected:

private:
	//값형
	std::map<int, UEngineKey> Keys;
	UEngineInput();

};

