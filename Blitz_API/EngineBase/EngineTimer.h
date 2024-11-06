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
	LARGE_INTEGER Count = LARGE_INTEGER(); // �� 1�ʿ� xxx��ŭ ���� �־��.
	// QuadPart => 1 => ���� 1�ʿ� 1�� ���� �־��.
	// QuadPart => 100 => ���� 1�ʿ� 100�� ���� �־��.

	LARGE_INTEGER PrevTime = LARGE_INTEGER();
	// LONGLONG QuadPart => 10000 / 1 �������� ���� 10000�� �������

	LARGE_INTEGER CurTime = LARGE_INTEGER();
	// LONGLONG QuadPart => 10002 

	// �޸� �Ἥ
	double TimeCounter = 0.0f;

	double DeltaTime = 0.0;

	float fDeltaTime = 0.0f;
};

