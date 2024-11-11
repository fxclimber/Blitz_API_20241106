#pragma once
#include <vector>
#include <string>
#include "EngineMath.h"

// 다양한 데이타 타입을 바이너리로 저장,로드할수 있도록, 직렬화,역직렬화를 지원하는 클래스 
class UEngineSerializer
{
public:
	//초기화,정리
	UEngineSerializer();
	~UEngineSerializer();

	// 쓰기 -------------------------------------------------------
	// 주어진 데이타를 _Size만큼 저장, 근데 왜 unsigned int?
	// Data의 크기가 모자라면, 벡터크기 확장
	void Write(void* _Data, unsigned int _Size);

	// 연산자 오버로드들.. <<로 데이타 밀어넣기.
	// 필요한 자료형별로 다 만들어준다.템플릿 사용불가.
	// 연산오버로드에서 *아닌 &를 사용하는 이유?
	void operator<<(int& _Data)
	{
		Write(&_Data, sizeof(int));
	}

	void operator<<(bool& _Data)
	{
		Write(&_Data, sizeof(bool));
	}

	void operator<<(FVector2D& _Data)
	{
		Write(&_Data, sizeof(FVector2D));
	}

	void operator<<(FIntPoint& _Data)
	{
		Write(&_Data, sizeof(FIntPoint));
	}

	void operator<<(std::string& _Data)
	{
		// 스트링은 먼저 문자열길이를 int로 저장후,데이타 저장
		int Size = static_cast<int>(_Data.size());
		operator<<(Size);
		Write(&_Data[0], static_cast<int>(_Data.size()));
	}

	void operator<<(class ISerializObject& _Data);

	template<typename DataType>
	void operator<<(std::vector<DataType>& _vector)
	{
		int Size = static_cast<int>(_vector.size());
		operator<<(Size);

		for (size_t i = 0; i < _vector.size(); i++)
		{
			// 자료형 변환이 안된다는 것이다.
			operator<<(_vector[i]);
		}
	}

	// 읽기 -------------------------------------------------------
	void Read(void* _Data, unsigned int _Size);

	void operator>>(int& _Data)
	{
		Read(&_Data, sizeof(int));
	}

	void operator>>(bool& _Data)
	{
		Read(&_Data, sizeof(bool));
	}

	void operator>>(FVector2D& _Data)
	{
		Read(&_Data, sizeof(FVector2D));
	}

	void operator>>(FIntPoint& _Data)
	{
		Read(&_Data, sizeof(FIntPoint));
	}

	void operator>>(std::string& _Data)
	{
		int Size;
		operator>>(Size);
		_Data.resize(Size);
		Read(&_Data[0], static_cast<int>(_Data.size()));
	}

	void operator>>(class ISerializObject& _Data);

	template<typename DataType>
	void operator>>(std::vector<DataType>& _vector)
	{
		int Size = 0;
		operator>>(Size);
		_vector.resize(Size);

		for (size_t i = 0; i < _vector.size(); i++)
		{
			operator>>(_vector[i]);
		}
	}

	//--------------기타 함수들
	// 
	void* GetDataPtr()
	{
		return &Data[0];
	}
	// 데이타의 읽기위치 관리 
	int GetWriteOffset()
	{
		return WriteOffset;
	}
	// 크기 모자랄땐 확장 
	void DataResize(int _Value)
	{
		Data.resize(_Value);
	}

protected:

private:
	// 얼마나 데이터가 채워졌냐?
	int WriteOffset = 0;

	// 얼마나 데이터가 채워졌냐?
	int ReadOffset = 0;

	std::vector<char> Data;
};

// 인터페이스 클래스
class ISerializObject
{

public:
	// 데이터를 직렬화(압축)
	virtual void Serialize(UEngineSerializer& _Ser) = 0;
	// 데이터를 복구(할때)
	virtual void DeSerialize(UEngineSerializer& _Ser) = 0;
};

