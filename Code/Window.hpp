// Copyright 2019, The maxHex Contributors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MAXHEX_WINDOW_HPP
#define MAXHEX_WINDOW_HPP

#define NOMINMAX
#include <Windows.h>

namespace maxHex
{

	int InitializeWindow(HINSTANCE Instance, int ShowCommand) noexcept;

} // namespace maxHex

#endif // #ifndef MAXHEX_WINDOW_HPP