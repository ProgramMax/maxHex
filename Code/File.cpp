// Copyright 2019, The maxHex Contributors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "File.hpp"

namespace maxHex
{


	File::File(const File& rhs) noexcept = default;
	File::File(File&& rhs) noexcept = default;

	#if defined(MAX_PLATFORM_WINDOWS)
	File::File(LPCTSTR FilePath) noexcept
		: FilePath(FilePath)
	{
	}
	#endif

	File::~File() noexcept = default;

	File& File::operator =(const File& rhs) noexcept = default;
	File& File::operator =(File&& rhs) noexcept = default;

} // namespace maxHex