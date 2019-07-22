// Copyright 2019, The maxHex Contributors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MAXHEX_FILE_HPP
#define MAXHEX_FILE_HPP

#include <max/Compiling/Configuration.hpp>

#if defined(MAX_PLATFORM_WINDOWS)
#include <Windows.h>
#endif

namespace maxHex
{

	class File
	{
	public:

		#if defined(MAX_PLATFORM_WINDOWS)
		explicit File(LPCTSTR FilePath);
		#endif

		#if defined(MAX_PLATFORM_WINDOWS)
		LPCTSTR FilePath;
		#endif

	};

} // namespace maxHex

#endif // #ifndef MAXHEX_FILE_HPP