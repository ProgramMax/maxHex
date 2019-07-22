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

		BufferChain();
		explicit BufferChain(Buffer&& InitialBuffer);
		explicit BufferChain(std::vector<Buffer>&& BufferList);

		std::vector<Buffer> BufferList;

	};

} // namespace maxHex

#endif // #ifndef MAXHEX_BUFFERCHAIN_HPP