#pragma once
#include <Windows.h>
#include <assert.h>

#define MSGASSERT(VALUE) std::string ErrorText = VALUE; MessageBoxA(nullptr, ErrorText.c_str(), "치명적 에러", MB_OK); assert(false);

namespace UEngineDebug
{
	// 분리해야 하겠네요
	void OutPutString(const std::string& Text);


}

