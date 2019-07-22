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

		BufferChainTestSuite.AddTest(max::Testing::Test< max::Testing::CoutResultPolicy >{ "default constructor empties buffer list", [](max::Testing::Test< max::Testing::CoutResultPolicy >& CurrentTest, max::Testing::CoutResultPolicy const& ResultPolicy) {
			BufferChain TestObject;

			CurrentTest.MAX_TESTING_ASSERT(TestObject.BufferList.size() == 0);
			}
		});

		BufferChainTestSuite.AddTest(max::Testing::Test< max::Testing::CoutResultPolicy >{ "constructor consumes buffer", [](max::Testing::Test< max::Testing::CoutResultPolicy >& CurrentTest, max::Testing::CoutResultPolicy const& ResultPolicy) {
			#if defined(MAX_PLATFORM_WINDOWS)
			LPCTSTR FilePath = TEXT("Test\\Path");
			File TestFile(FilePath);
			#endif
			size_t SourceOffset = 0;
			const size_t BufferLength = 10;
			Buffer BufferToConsume(std::move(TestFile), std::move(SourceOffset), BufferLength);

			char const* MemoryLocation = BufferToConsume.ByteBuffer.get();

			BufferChain TestObject(std::move(BufferToConsume));

			CurrentTest.MAX_TESTING_ASSERT(TestObject.BufferList[0].ByteBuffer.get() == MemoryLocation);
			CurrentTest.MAX_TESTING_ASSERT(TestObject.BufferList[0].ByteBufferLength == BufferLength);
			CurrentTest.MAX_TESTING_ASSERT(BufferToConsume.ByteBuffer == nullptr);
			}
		});

		BufferChainTestSuite.AddTest(max::Testing::Test< max::Testing::CoutResultPolicy >{ "constructor consumes buffer list", [](max::Testing::Test< max::Testing::CoutResultPolicy >& CurrentTest, max::Testing::CoutResultPolicy const& ResultPolicy) {
			#if defined(MAX_PLATFORM_WINDOWS)
			LPCTSTR FirstFilePath = TEXT("Test\\Path");
			LPCTSTR SecondFilePath = TEXT("Test\\Path");
			File FirstTestFile(FirstFilePath);
			File SecondTestFile(SecondFilePath);
			#endif
			size_t FirstSourceOffset = 0;
			size_t SecondSourceOffset = 0;
			const size_t FirstBufferLength = 10;
			const size_t SecondBufferLength = 20;
			std::vector<Buffer> BufferListToConsume;
			BufferListToConsume.emplace_back(std::move(FirstTestFile), std::move(FirstSourceOffset), FirstBufferLength);
			BufferListToConsume.emplace_back(std::move(SecondTestFile), std::move(SecondSourceOffset), SecondBufferLength);

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