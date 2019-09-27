// Copyright 2019, The maxHex Contributors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma region Includes & Forward Declarations

#include <Window.hpp>

#define NOMINMAX
#include <Windows.h>

#pragma endregion

int WINAPI WinMain(_In_ HINSTANCE Instance, _In_opt_ HINSTANCE /*PreviousInstance*/, _In_ LPSTR /*CommandLine*/, _In_ int ShowCommand)
{
	auto Result = maxHex::InitializeWindow(Instance, ShowCommand);
	if (Result != 0)
	{
		return Result;
	}

	MSG Message;
	while (GetMessage(&Message, NULL, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return static_cast<int>(Message.wParam);
}