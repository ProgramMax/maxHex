// Copyright 2019, The maxHex Contributors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Buffer.hpp"

#include <algorithm>
#include <utility>

namespace maxHex
{

	Buffer::Buffer(File SourceFile, size_t SourceOffset, const size_t BufferLength, const size_t BufferCapacity) noexcept
		: SourceFile(std::move(SourceFile))
		, SourceOffset(std::move(SourceOffset))
		, ByteBuffer(std::make_unique<char[]>(BufferCapacity))
		, ByteBufferLength(BufferLength)
		, ByteBufferCapacity(BufferCapacity)
	{
	}

	Buffer::Buffer(const Buffer& rhs) noexcept
		: SourceFile(rhs.SourceFile)
		, SourceOffset(rhs.SourceOffset)
		, ByteBuffer(std::make_unique<char[]>(rhs.ByteBufferCapacity))
		, ByteBufferLength(rhs.ByteBufferLength)
		, ByteBufferCapacity(rhs.ByteBufferCapacity)
	{
		std::copy(rhs.ByteBuffer.get(), rhs.ByteBuffer.get() + rhs.ByteBufferLength, ByteBuffer.get());
	}

	Buffer::Buffer(Buffer&& rhs) noexcept = default;
	Buffer::~Buffer() noexcept = default;

	Buffer& Buffer::operator =(const Buffer& rhs) noexcept
	{
		SourceFile = rhs.SourceFile;
		SourceOffset = rhs.SourceOffset;
		ByteBuffer = std::make_unique<char[]>(rhs.ByteBufferCapacity);
		ByteBufferLength = rhs.ByteBufferLength;
		ByteBufferCapacity = rhs.ByteBufferCapacity;

		std::copy(rhs.ByteBuffer.get(), rhs.ByteBuffer.get() + rhs.ByteBufferLength, ByteBuffer.get());

		return *this;
	}

	Buffer& Buffer::operator =(Buffer&& rhs) noexcept = default;

} // namespace maxHex