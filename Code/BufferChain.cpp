// Copyright 2019, The maxHex Contributors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "BufferChain.hpp"
#include <utility>

namespace maxHex
{

	BufferChain::BufferChain() noexcept = default;

	BufferChain::BufferChain(Buffer InitialBuffer) noexcept
		: BufferList()
	{
		BufferList.push_back(std::move(InitialBuffer));
	}

	BufferChain::BufferChain(std::vector<Buffer> BufferList) noexcept
		: BufferList(std::move(BufferList))
	{
	}

	BufferChain::BufferChain(const BufferChain& rhs) = default;
	BufferChain::BufferChain(BufferChain&& rhs) noexcept = default;

	BufferChain& BufferChain::operator =(const BufferChain& rhs) = default;
	BufferChain& BufferChain::operator =(BufferChain&& rhs) noexcept = default;

} // namespace maxHex