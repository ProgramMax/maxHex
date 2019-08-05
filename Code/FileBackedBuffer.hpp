// Copyright 2019, The maxHex Contributors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Buffer.hpp"
#include "File.hpp"

namespace maxHex
{

	class FileBackedBuffer : public Buffer
	{
	public:

		FileBackedBuffer(File SourceFile, size_t SourceOffset, const size_t BufferLength, const size_t BufferCapacity) noexcept;
		FileBackedBuffer(const FileBackedBuffer& rhs) noexcept;
		FileBackedBuffer(FileBackedBuffer&& rhs) noexcept;
		~FileBackedBuffer() noexcept;

		FileBackedBuffer& operator =(const FileBackedBuffer& rhs) noexcept;
		FileBackedBuffer& operator =(FileBackedBuffer&& rhs) noexcept;

		File SourceFile;
		size_t SourceOffset;

	};

} // namespace maxHex