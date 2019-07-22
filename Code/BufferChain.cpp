// Copyright 2019, The maxHex Contributors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "BufferChain.hpp"
#include <utility>

namespace maxHex
{

	BufferChain::BufferChain()
		: BufferList()
	{
	}

	BufferChain::BufferChain(Buffer&& InitialBuffer)
		: BufferList()
	{
		BufferList.push_back(std::move(InitialBuffer));
	}

	BufferChain::BufferChain(std::vector<Buffer>&& BufferList)
		: BufferList(std::move(BufferList))
	{
	}

} // namespace maxHex