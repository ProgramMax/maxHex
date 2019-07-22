// Copyright 2019, The maxHex Contributors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Buffer.hpp"
#include "BufferChain.hpp"
#include <max/Testing/TestSuite.hpp>
#include <max/Testing/CoutResultPolicy.hpp>
#include <utility>

namespace maxHex
{

	void RunBufferChainTestSuite()
	{
		max::Testing::CoutResultPolicy ResultPolicy;
		auto BufferChainTestSuite = max::Testing::TestSuite< max::Testing::CoutResultPolicy >{ "maxhex::BufferChain test suite", std::move(ResultPolicy) };

		BufferChainTestSuite.AddTest(max::Testing::Test< max::Testing::CoutResultPolicy >{ "constructor consumes buffer", [](max::Testing::Test< max::Testing::CoutResultPolicy >& CurrentTest, max::Testing::CoutResultPolicy const& ResultPolicy) {
			const size_t BufferLength = 10;
			Buffer BufferToConsume(BufferLength);
			char const* MemoryLocation = BufferToConsume.ByteBuffer.get();

			BufferChain TestObject(std::move(BufferToConsume));

			CurrentTest.MAX_TESTING_ASSERT(TestObject.BufferList[0].ByteBuffer.get() == MemoryLocation);
			CurrentTest.MAX_TESTING_ASSERT(TestObject.BufferList[0].ByteBufferLength == BufferLength);
			CurrentTest.MAX_TESTING_ASSERT(BufferToConsume.ByteBuffer == nullptr);
			}
		});

		BufferChainTestSuite.AddTest(max::Testing::Test< max::Testing::CoutResultPolicy >{ "constructor consumes buffer list", [](max::Testing::Test< max::Testing::CoutResultPolicy >& CurrentTest, max::Testing::CoutResultPolicy const& ResultPolicy) {
			const size_t FirstBufferLength = 10;
			const size_t SecondBufferLength = 20;
			std::vector<Buffer> BufferListToConsume;
			BufferListToConsume.emplace_back(FirstBufferLength);
			BufferListToConsume.emplace_back(SecondBufferLength);

			char const* FirstMemoryLocation = BufferListToConsume[0].ByteBuffer.get();
			char const* SecondMemoryLocation = BufferListToConsume[1].ByteBuffer.get();

			BufferChain TestObject(std::move(BufferListToConsume));

			CurrentTest.MAX_TESTING_ASSERT(TestObject.BufferList[0].ByteBuffer.get() == FirstMemoryLocation);
			CurrentTest.MAX_TESTING_ASSERT(TestObject.BufferList[0].ByteBufferLength == FirstBufferLength);
			CurrentTest.MAX_TESTING_ASSERT(TestObject.BufferList[1].ByteBuffer.get() == SecondMemoryLocation);
			CurrentTest.MAX_TESTING_ASSERT(TestObject.BufferList[1].ByteBufferLength == SecondBufferLength);
			CurrentTest.MAX_TESTING_ASSERT(BufferListToConsume.size() == 0);
			}
		});

		BufferChainTestSuite.RunTests();
	}

} // namespace maxHex