// Copyright 2019, The maxHex Contributors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "BufferChain.hpp"
#include <utility>
#include <algorithm>

namespace maxHex
{

	BufferChain::BufferChain() noexcept = default;

	BufferChain::BufferChain(std::vector<std::unique_ptr<Buffer>> BufferList) noexcept
		: BufferList(std::move(BufferList))
	{
	}

	/*
	BufferChain::BufferChain(const BufferChain & rhs) noexcept
		: BufferList()
	{
		BufferList.reserve(rhs.BufferList.size());
		for (const std::unique_ptr<Buffer>& CurrentBuffer : rhs.BufferList)
		{
			// TODO: This should really call ->Clone() since Buffer is a base class.
			auto CopiedBuffer = std::make_unique<Buffer>(*CurrentBuffer);
			BufferList.push_back(std::move(CopiedBuffer));
		}
	}
	*/

	BufferChain::BufferChain(BufferChain&& rhs) noexcept = default;
	BufferChain::~BufferChain() noexcept = default;

	/*
	BufferChain& BufferChain::operator =(const BufferChain& rhs) noexcept
	{
		BufferList = std::vector<std::unique_ptr<Buffer>>{};
		BufferList.reserve(rhs.BufferList.size());
		for (const std::unique_ptr<Buffer>& CurrentBuffer : rhs.BufferList)
		{
			// TODO: This should really call ->Clone() since Buffer is a base class.
			auto CopiedBuffer = std::make_unique<Buffer>(*CurrentBuffer);
			BufferList.push_back(std::move(CopiedBuffer));
		}

		return *this;
	}
	*/

	BufferChain& BufferChain::operator =(BufferChain&& rhs) noexcept = default;

} // namespace maxHex