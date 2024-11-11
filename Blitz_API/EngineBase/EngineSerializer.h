#pragma once
#include <vector>
#include <string>
#include "EngineMath.h"

// �پ��� ����Ÿ Ÿ���� ���̳ʸ��� ����,�ε��Ҽ� �ֵ���, ����ȭ,������ȭ�� �����ϴ� Ŭ���� 
class UEngineSerializer
{
public:
	//�ʱ�ȭ,����
	UEngineSerializer();
	~UEngineSerializer();

	// ���� -------------------------------------------------------
	// �־��� ����Ÿ�� _Size��ŭ ����, �ٵ� �� unsigned int?
	// Data�� ũ�Ⱑ ���ڶ��, ����ũ�� Ȯ��
	void Write(void* _Data, unsigned int _Size);

	// ������ �����ε��.. <<�� ����Ÿ �о�ֱ�.
	// �ʿ��� �ڷ������� �� ������ش�.���ø� ���Ұ�.
	// ��������ε忡�� *�ƴ� &�� ����ϴ� ����?
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
		// ��Ʈ���� ���� ���ڿ����̸� int�� ������,����Ÿ ����
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
			// �ڷ��� ��ȯ�� �ȵȴٴ� ���̴�.
			operator<<(_vector[i]);
		}
	}

	// �б� -------------------------------------------------------
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

	//--------------��Ÿ �Լ���
	// 
	void* GetDataPtr()
	{
		return &Data[0];
	}
	// ����Ÿ�� �б���ġ ���� 
	int GetWriteOffset()
	{
		return WriteOffset;
	}
	// ũ�� ���ڶ��� Ȯ�� 
	void DataResize(int _Value)
	{
		Data.resize(_Value);
	}

protected:

private:
	// �󸶳� �����Ͱ� ä������?
	int WriteOffset = 0;

	// �󸶳� �����Ͱ� ä������?
	int ReadOffset = 0;

	std::vector<char> Data;
};

// �������̽� Ŭ����
class ISerializObject
{

public:
	// �����͸� ����ȭ(����)
	virtual void Serialize(UEngineSerializer& _Ser) = 0;
	// �����͸� ����(�Ҷ�)
	virtual void DeSerialize(UEngineSerializer& _Ser) = 0;
};

