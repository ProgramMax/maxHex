// Copyright 2019, The maxHex Contributors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Buffer.hpp"

namespace maxHex
{

	Buffer::Buffer(const BufferBacking Backing, const size_t Length, const size_t Capacity) noexcept
		: Backing(Backing)
		, Storage(std::make_unique<char[]>(Capacity))
		, Length(Length)
		, Capacity(Capacity)
	{
	}

	Buffer::Buffer(const Buffer& rhs) noexcept
		: Backing(rhs.Backing)
		, Storage(std::make_unique<char[]>(rhs.Capacity))
		, Length(rhs.Length)
		, Capacity(rhs.Capacity)
	{
		std::copy(rhs.Storage.get(), rhs.Storage.get() + rhs.Length, Storage.get());
	}

	Buffer::Buffer(Buffer&& rhs) noexcept = default;
	Buffer::~Buffer() noexcept = default;

	Buffer& Buffer::operator =(const Buffer& rhs) noexcept
	{
		Storage = std::make_unique<char[]>(rhs.Capacity);
		Length = rhs.Length;
		Capacity = rhs.Capacity;

		std::copy(rhs.Storage.get(), rhs.Storage.get() + rhs.Length, Storage.get());

		return *this;
	}

	Buffer& Buffer::operator =(Buffer&& rhs) noexcept = default;

} // namespace maxHex