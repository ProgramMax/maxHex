// Copyright 2019, The maxHex Contributors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MAXHEX_BUFFER_HPP
#define MAXHEX_BUFFER_HPP

#include <File.hpp>
#include <memory>

namespace maxHex
{

	class Buffer
	{
	public:

		Buffer() = delete;
		Buffer(File SourceFile, size_t SourceOffset, const size_t BufferLength, const size_t BufferCapacity) noexcept;
		Buffer(const Buffer& rhs) noexcept;
		Buffer(Buffer&& rhs) noexcept;
		~Buffer() noexcept;

		Buffer& operator =(const Buffer& rhs) noexcept;
		Buffer& operator =(Buffer&& rhs) noexcept;

		File SourceFile;
		size_t SourceOffset;
		std::unique_ptr<char[]> ByteBuffer;
		size_t ByteBufferLength;
		size_t ByteBufferCapacity;

	};

} // namespace maxHex

#endif // #ifndef MAXHEX_BUFFER_HPP