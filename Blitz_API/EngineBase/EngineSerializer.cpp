#include "PreCompiledFile.h"
#include "EngineSerializer.h"

UEngineSerializer::UEngineSerializer()
{
}

UEngineSerializer::~UEngineSerializer()
{
}

void UEngineSerializer::Write(void* _Data, unsigned int _Size)
{
	// 좀더 구체적인 계산과정 설명필요 
	if (WriteOffset + _Size >= Data.size())
	{
		Data.resize(Data.size() * 2 + _Size);
	}
	memcpy_s(&Data[WriteOffset], _Size, _Data, _Size);
	WriteOffset += _Size;
}

void UEngineSerializer::operator<<(ISerializObject& _Data)
{
	_Data.Serialize(*this);
}

void UEngineSerializer::Read(void* _Data, unsigned int _Size)
{
	memcpy_s(_Data, _Size, &Data[ReadOffset], _Size);
	ReadOffset += _Size;
}

void UEngineSerializer::operator>>(ISerializObject& _Data)
{
	_Data.DeSerialize(*this);
}
