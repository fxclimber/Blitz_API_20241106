#include "PreCompiledFile.h"
#include "EngineDebug.h"


namespace UEngineDebug
{
	// 분리해야 하겠네요
	void OutPutString(const std::string& Text)
	{
		std::string ResultText = Text + "\n";
		OutputDebugStringA(ResultText.c_str());
	}


}