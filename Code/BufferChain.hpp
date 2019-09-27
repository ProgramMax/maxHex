// Copyright 2019, The maxHex Contributors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MAXHEX_BUFFERCHAIN_HPP
#define MAXHEX_BUFFERCHAIN_HPP

#include "Buffer.hpp"
#include <vector>
#include <memory>

namespace maxHex
{

	class BufferChain
	{
	public:

		BufferChain() noexcept;
		explicit BufferChain(std::vector<std::unique_ptr<Buffer>> BufferList) noexcept;
		BufferChain(const BufferChain& rhs) noexcept = delete;
		BufferChain(BufferChain&& rhs) noexcept;
		
		~BufferChain() noexcept;

		BufferChain& operator =(const BufferChain& rhs) noexcept = delete;
		BufferChain& operator =(BufferChain&& rhs) noexcept;

		std::vector<std::unique_ptr<Buffer>> BufferList;

	};

} // namespace maxHex

#endif // #ifndef MAXHEX_BUFFERCHAIN_HPP