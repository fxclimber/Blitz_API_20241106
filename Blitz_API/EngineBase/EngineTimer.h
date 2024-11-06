#pragma once
#include <Windows.h>

class UEngineTimer
{
public:
	// constrcuter destructer
	UEngineTimer();
	~UEngineTimer();

	void TimeCheck();

	float GetDeltaTime()
	{
		// 0.0000001
		// 0.000000000000001
		return fDeltaTime;
	}

	double GetDoubleDeltaTime()
	{
		// 0.0000001
		// 0.000000000000001
		return DeltaTime;
	}

	void TimeStart();

	float End();

	double DEnd();

protected:

private:

	// LONGLONG QuadPart => __int64
	LARGE_INTEGER Count = LARGE_INTEGER(); // 나 1초에 xxx만큼 샐수 있어요.
	// QuadPart => 1 => 나는 1초에 1을 샐수 있어요.
	// QuadPart => 100 => 나는 1초에 100을 샐수 있어요.

	LARGE_INTEGER PrevTime = LARGE_INTEGER();
	// LONGLONG QuadPart => 10000 / 1 기준으로 부터 10000초 지났어요

	LARGE_INTEGER CurTime = LARGE_INTEGER();
	// LONGLONG QuadPart => 10002 

	// 메모리 써서
	double TimeCounter = 0.0f;

	double DeltaTime = 0.0;

	float fDeltaTime = 0.0f;
};

