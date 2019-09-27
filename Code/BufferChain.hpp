// Copyright 2019, The maxHex Contributors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MAXHEX_BUFFERCHAIN_HPP
#define MAXHEX_BUFFERCHAIN_HPP

#include "Buffer.hpp"
#include <vector>

namespace maxHex
{

	class BufferChain
	{
	public:

		BufferChain() noexcept;
		explicit BufferChain(Buffer InitialBuffer) noexcept;
		explicit BufferChain(std::vector<Buffer> BufferList) noexcept;
		BufferChain(const BufferChain& rhs);
		BufferChain(BufferChain&& rhs) noexcept;

		BufferChain& operator =(const BufferChain& rhs);
		BufferChain& operator =(BufferChain&& rhs) noexcept;

		std::vector<Buffer> BufferList;

	};

} // namespace maxHex

#endif // #ifndef MAXHEX_BUFFERCHAIN_HPP