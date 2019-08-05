// Copyright 2019, The maxHex Contributors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "FileBackedBuffer.hpp"

#include <algorithm>
#include <utility>

namespace maxHex
{

	FileBackedBuffer::FileBackedBuffer(File SourceFile, size_t SourceOffset, const size_t BufferLength, const size_t BufferCapacity) noexcept
		: Buffer(BufferBacking::File, BufferLength, BufferCapacity)
		, SourceFile(std::move(SourceFile))
		, SourceOffset(std::move(SourceOffset))
	{
	}

	FileBackedBuffer::FileBackedBuffer(const FileBackedBuffer& rhs) noexcept = default;
	FileBackedBuffer::FileBackedBuffer(FileBackedBuffer&& rhs) noexcept = default;
	FileBackedBuffer::~FileBackedBuffer() noexcept = default;
	FileBackedBuffer& FileBackedBuffer::operator =(const FileBackedBuffer& rhs) noexcept = default;
	FileBackedBuffer& FileBackedBuffer::operator =(FileBackedBuffer&& rhs) noexcept = default;

} // namespace maxHex