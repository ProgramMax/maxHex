// Copyright 2019, The maxHex Contributors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MAXHEX_BUFFER_HPP
#define MAXHEX_BUFFER_HPP

#include <memory>

namespace maxHex
{

	class Buffer
	{
	public:

		explicit Buffer(const size_t BufferLength) noexcept;
		Buffer(const Buffer& rhs) noexcept;
		Buffer(Buffer&& rhs) noexcept;
		~Buffer() noexcept = default;

		Buffer& operator =(const Buffer& rhs) noexcept;
		Buffer& operator =(Buffer&& rhs) noexcept;

		std::unique_ptr<char[]> ByteBuffer;
		size_t ByteBufferLength;

	};

} // namespace maxHex

#endif // #ifndef MAXHEX_BUFFER_HPP