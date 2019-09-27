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

		BufferChainTestSuite.AddTest(max::Testing::Test< max::Testing::CoutResultPolicy >{ "default constructor", [](max::Testing::Test< max::Testing::CoutResultPolicy >& CurrentTest, max::Testing::CoutResultPolicy const& ResultPolicy) {
			BufferChain TestObject;

			CurrentTest.MAX_TESTING_ASSERT(TestObject.BufferList.size() == 0);
			}
		});

		// TODO: Make BufferChain's copy ctor work
		/*
		BufferChainTestSuite.AddTest(max::Testing::Test< max::Testing::CoutResultPolicy >{ "copy constructor", [](max::Testing::Test< max::Testing::CoutResultPolicy >& CurrentTest, max::Testing::CoutResultPolicy const& ResultPolicy) {
			const size_t BufferLength = 10;
			const size_t BufferCapacity = 20;
			auto TestBuffer = std::make_unique<Buffer>(BufferBacking::Memory, BufferLength, BufferCapacity);
			std::vector<std::unique_ptr<Buffer>> Buffers;
			Buffers.push_back(std::move(TestBuffer));
			BufferChain OriginalObject(std::move(Buffers));

			BufferChain TestObject(OriginalObject);

			CurrentTest.MAX_TESTING_ASSERT(TestObject.BufferList.size() == OriginalObject.BufferList.size());
			CurrentTest.MAX_TESTING_ASSERT(TestObject.BufferList[0]->Length == OriginalObject.BufferList[0]->Length);
			CurrentTest.MAX_TESTING_ASSERT(TestObject.BufferList[0]->Capacity == OriginalObject.BufferList[0]->Capacity);
			}
		});
		*/

		BufferChainTestSuite.AddTest(max::Testing::Test< max::Testing::CoutResultPolicy >{ "move constructor", [](max::Testing::Test< max::Testing::CoutResultPolicy >& CurrentTest, max::Testing::CoutResultPolicy const& ResultPolicy) {
			const size_t BufferLength = 10;
			const size_t BufferCapacity = 20;
			auto TestBuffer = std::make_unique<Buffer>(BufferBacking::Memory, BufferLength, BufferCapacity);
			char const* MemoryLocation = TestBuffer->Storage.get();
			std::vector<std::unique_ptr<Buffer>> Buffers;
			Buffers.push_back(std::move(TestBuffer));
			BufferChain OriginalObject(std::move(Buffers));

			BufferChain TestObject(std::move(OriginalObject));

			CurrentTest.MAX_TESTING_ASSERT(TestObject.BufferList[0]->Storage.get() == MemoryLocation);
			CurrentTest.MAX_TESTING_ASSERT(OriginalObject.BufferList.size() == 0);
			CurrentTest.MAX_TESTING_ASSERT(TestObject.BufferList.size() == 1);
			CurrentTest.MAX_TESTING_ASSERT(TestObject.BufferList[0]->Length == BufferLength);
			CurrentTest.MAX_TESTING_ASSERT(TestObject.BufferList[0]->Capacity == BufferCapacity);
			}
		});

		BufferChainTestSuite.AddTest(max::Testing::Test< max::Testing::CoutResultPolicy >{ "constructor", [](max::Testing::Test< max::Testing::CoutResultPolicy >& CurrentTest, max::Testing::CoutResultPolicy const& ResultPolicy) {
			const size_t FirstBufferLength = 10;
			const size_t SecondBufferLength = 20;
			const size_t FirstBufferCapacity = 30;
			const size_t SecondBufferCapacity = 40;
			auto FirstBuffer = std::make_unique<Buffer>(BufferBacking::Memory, FirstBufferLength, FirstBufferCapacity);
			auto SecondBuffer = std::make_unique<Buffer>(BufferBacking::Memory, SecondBufferLength, SecondBufferCapacity);
			char const* FirstMemoryLocation = FirstBuffer->Storage.get();
			char const* SecondMemoryLocation = SecondBuffer->Storage.get();
			std::vector<std::unique_ptr<Buffer>> Buffers;
			Buffers.push_back(std::move(FirstBuffer));
			Buffers.push_back(std::move(SecondBuffer));

			BufferChain TestObject(std::move(Buffers));

			CurrentTest.MAX_TESTING_ASSERT(TestObject.BufferList[0]->Storage.get() == FirstMemoryLocation);
			CurrentTest.MAX_TESTING_ASSERT(TestObject.BufferList[0]->Length == FirstBufferLength);
			CurrentTest.MAX_TESTING_ASSERT(TestObject.BufferList[0]->Capacity == FirstBufferCapacity);
			CurrentTest.MAX_TESTING_ASSERT(TestObject.BufferList[1]->Storage.get() == SecondMemoryLocation);
			CurrentTest.MAX_TESTING_ASSERT(TestObject.BufferList[1]->Length == SecondBufferLength);
			CurrentTest.MAX_TESTING_ASSERT(TestObject.BufferList[1]->Capacity == SecondBufferCapacity);
			CurrentTest.MAX_TESTING_ASSERT(Buffers.size() == 0);
			}
		});

		// TODO: Make BufferChain's copy assignment work
		/*
		BufferChainTestSuite.AddTest(max::Testing::Test< max::Testing::CoutResultPolicy >{ "copy assignment operator", [](max::Testing::Test< max::Testing::CoutResultPolicy >& CurrentTest, max::Testing::CoutResultPolicy const& ResultPolicy) {
			const size_t OriginalBufferLength = 10;
			const size_t OriginalBufferCapacity = 20;
			auto OriginalBuffer = std::make_unique<Buffer>(BufferBacking::Memory, OriginalBufferLength, OriginalBufferCapacity);
			std::vector<std::unique_ptr<Buffer>> OriginalBufferList;
			OriginalBufferList.push_back(std::move(OriginalBuffer));
			BufferChain OriginalObject(std::move(OriginalBufferList));

			const size_t TestBufferLength = 30;
			const size_t TestBufferCapacity = 40;
			auto TestBuffer = std::make_unique<Buffer>(BufferBacking::Memory, TestBufferLength, TestBufferCapacity);
			std::vector<std::unique_ptr<Buffer>> TestBufferList;
			TestBufferList.push_back(std::move(TestBuffer));
			BufferChain TestObject(std::move(TestBufferList));

			TestObject = OriginalObject;

			CurrentTest.MAX_TESTING_ASSERT(TestObject.BufferList.size() == OriginalObject.BufferList.size());
			CurrentTest.MAX_TESTING_ASSERT(TestObject.BufferList[0]->Length == OriginalObject.BufferList[0]->Length);
			CurrentTest.MAX_TESTING_ASSERT(TestObject.BufferList[0]->Capacity == OriginalObject.BufferList[0]->Capacity);
			}
		});
		*/

		BufferChainTestSuite.AddTest(max::Testing::Test< max::Testing::CoutResultPolicy >{ "move constructor", [](max::Testing::Test< max::Testing::CoutResultPolicy >& CurrentTest, max::Testing::CoutResultPolicy const& ResultPolicy) {
			const size_t OriginalBufferLength = 10;
			const size_t OriginalBufferCapacity = 20;
			auto OriginalBuffer = std::make_unique<Buffer>(BufferBacking::Memory, OriginalBufferLength, OriginalBufferCapacity);
			std::vector<std::unique_ptr<Buffer>> OriginalBufferList;
			OriginalBufferList.push_back(std::move(OriginalBuffer));
			BufferChain OriginalObject(std::move(OriginalBufferList));

			const size_t TestBufferLength = 30;
			const size_t TestBufferCapacity = 40;
			auto TestBuffer = std::make_unique<Buffer>(BufferBacking::Memory, TestBufferLength, TestBufferCapacity);
			std::vector<std::unique_ptr<Buffer>> TestBufferList;
			TestBufferList.push_back(std::move(TestBuffer));
			BufferChain TestObject(std::move(TestBufferList));

			TestObject = std::move(OriginalObject);

			CurrentTest.MAX_TESTING_ASSERT(TestObject.BufferList.size() == 1);
			CurrentTest.MAX_TESTING_ASSERT(OriginalObject.BufferList.size() == 0);
			CurrentTest.MAX_TESTING_ASSERT(TestObject.BufferList[0]->Length == OriginalBufferLength);
			CurrentTest.MAX_TESTING_ASSERT(TestObject.BufferList[0]->Capacity == OriginalBufferCapacity);
			}
		});

		BufferChainTestSuite.RunTests();
	}

} // namespace maxHex