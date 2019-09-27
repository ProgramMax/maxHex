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

		File() = delete;
		File(const File& rhs) noexcept;
		File(File&& rhs) noexcept;
		#if defined(MAX_PLATFORM_WINDOWS)
		explicit File(LPCTSTR FilePath) noexcept;
		#endif
		~File() noexcept;

		File& operator =(const File& rhs) noexcept;
		File& operator =(File&& rhs) noexcept;


		#if defined(MAX_PLATFORM_WINDOWS)
		LPCTSTR FilePath;
		#endif

	};

} // namespace maxHex

#endif // #ifndef MAXHEX_FILE_HPP