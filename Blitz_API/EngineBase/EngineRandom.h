#pragma once
#include <random>

class UEngineRandom
{
public:
	UEngineRandom();
	~UEngineRandom();


	void SetSeed(__int64 _Seed)
	{
		MtGen = std::mt19937_64(_Seed);
	}

	int RandomInt(int _Min, int _Max)
	{
		if (_Max < _Min)
		{
			float SwapValue = _Min;
			_Min = _Max;
			_Max = SwapValue;
		}

		// _Min ~ _Max ·£´ı°ª »Ì¾ÆÁà
		std::uniform_int_distribution<int> RandomCreate(_Min, _Max);

		// MtGen Á¦³×·¹ÀÌÅÍ ½á¼­
		// std::mt19937_64 ¸Ş¸£Çî Æ®À§½ºÅÍ ¾Ë°í¸®Áò ½á¼­ ¸¸µé¾îÁà.
		return RandomCreate.operator()(MtGen);
	}

	float Randomfloat(float _Min, float _Max)
	{
		if (_Max < _Min)
		{
			float SwapValue = _Min;
			_Min = _Max;
			_Max = SwapValue;
		}

		// _Min ~ _Max ·£´ı°ª »Ì¾ÆÁà
		std::uniform_real_distribution<float> RandomCreate(_Min, _Max);
		return RandomCreate.operator()(MtGen);
	}


protected:

private:

	std::mt19937_64 MtGen = std::mt19937_64(time(nullptr));

};
