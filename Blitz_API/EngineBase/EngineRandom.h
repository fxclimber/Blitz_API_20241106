#pragma once
#include <random>

class UEngineRandom
{
public:
	// constrcuter destructer
	UEngineRandom();
	~UEngineRandom();

	void SetSeed(__int64 _Seed)
	{
		std::mt19937_64 MtGen = std::mt19937_64(_Seed);
	}

	int RandomInt(int _Min, int _Max)
	{
		// _Min ~ _Max 랜덤값 뽑아줘
		std::uniform_int_distribution<int> RandomCreate(_Min, _Max);

		// MtGen 제네레이터 써서
		// std::mt19937_64 메르헨 트위스터 알고리즘 써서 만들어줘.
		return RandomCreate.operator()(MtGen);
	}

	float Randomfloat(float _Min, float _Max)
	{
		// _Min ~ _Max 랜덤값 뽑아줘
		std::uniform_real_distribution<float> RandomCreate(_Min, _Max);
		return RandomCreate.operator()(MtGen);
	}


protected:

private:
	// std 랜덤은
	// 제네레이터를 시작으로 합니다.

	// 기본적으로 타임에 의한 시드로 세팅
	// 만들면서 new 시드 넣어줍니다.
	std::mt19937_64 MtGen = std::mt19937_64(time(nullptr));

};
