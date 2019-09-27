// Copyright 2019, The maxHex Contributors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MAXHEX_BUFFER_HPP
#define MAXHEX_BUFFER_HPP


#include "BufferBacking.hpp"

#include <memory>

namespace maxHex
{

	class Buffer
	{
	public:

		Buffer() = delete;
		Buffer(const BufferBacking Backing, const size_t Length, const size_t Capacity) noexcept;
		Buffer(const Buffer& rhs) noexcept;
		Buffer(Buffer&& rhs) noexcept;
		virtual ~Buffer() noexcept;

		Buffer& operator =(const Buffer& rhs) noexcept;
		Buffer& operator =(Buffer&& rhs) noexcept;

		// TODO: Being a base class, should this have virtual std::unique_ptr<Base> Clone(); ?
		// It would be needed for BufferChain to have a copy ctor/assign.

		BufferBacking Backing;
		std::unique_ptr<char[]> Storage;
		size_t Length;
		size_t Capacity;

	};

} // namespace maxHex

#endif // #ifndef MAXHEX_BUFFER_HPP