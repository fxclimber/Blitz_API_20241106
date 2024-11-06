#pragma once

namespace UEngineDebug
{
	// MainWindow가 있다.

	void SetIsDebug(bool _IsDebug);

	void SwitchIsDebug();

	void CoreOutPutString(std::string_view _Text);

	void CoreOutPutString(std::string_view _Text, FVector2D _Pos);

	// 분리해야 하겠네요

	void PrintEngineDebugText();
}