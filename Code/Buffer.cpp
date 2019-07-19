// Copyright 2019, The maxHex Contributors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Buffer.hpp"

#include <algorithm>
#include <utility>

namespace maxHex
{

	Buffer::Buffer(const size_t BufferLength) noexcept
		: ByteBuffer(std::make_unique<char[]>(BufferLength))
		, ByteBufferLength(BufferLength)
	{
	}

	Buffer::Buffer(const Buffer& rhs) noexcept
	{
		ByteBuffer = std::make_unique<char[]>(rhs.ByteBufferLength);
		ByteBufferLength = rhs.ByteBufferLength;
		std::copy(rhs.ByteBuffer.get(), rhs.ByteBuffer.get() + rhs.ByteBufferLength, ByteBuffer.get());
	}

	Buffer::Buffer(Buffer&& rhs) noexcept
	{
		ByteBuffer = std::move(rhs.ByteBuffer);
		rhs.ByteBuffer = nullptr;
		ByteBufferLength = std::move(rhs.ByteBufferLength);
	}

	Buffer& Buffer::operator =(const Buffer& rhs) noexcept
	{
		ByteBuffer = std::make_unique<char[]>(rhs.ByteBufferLength);
		ByteBufferLength = rhs.ByteBufferLength;
		std::copy(rhs.ByteBuffer.get(), rhs.ByteBuffer.get() + rhs.ByteBufferLength, ByteBuffer.get());

		return *this;
	}

	Buffer& Buffer::operator =(Buffer&& rhs) noexcept
	{
		ByteBuffer = std::move(rhs.ByteBuffer);
		rhs.ByteBuffer = nullptr;
		ByteBufferLength = std::move(rhs.ByteBufferLength);

		return *this;
	}

} // namespace maxHex