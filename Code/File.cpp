// Copyright 2019, The maxHex Contributors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "File.hpp"

namespace maxHex
{

	#if defined(MAX_PLATFORM_WINDOWS)
	File::File(LPCTSTR FilePath)
		: FilePath(FilePath)
	{
	}
	#endif

} // namespace maxHex