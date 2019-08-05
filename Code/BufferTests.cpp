// Copyright 2019, The maxHex Contributors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Buffer.hpp"
#include <max/Testing/TestSuite.hpp>
#include <max/Testing/CoutResultPolicy.hpp>
#include <utility>

namespace maxHex
{

	void RunBufferTestSuite()
	{
		max::Testing::CoutResultPolicy ResultPolicy;
		auto BufferTestSuite = max::Testing::TestSuite< max::Testing::CoutResultPolicy >{ "maxHex::Buffer test suite", std::move(ResultPolicy) };

		BufferTestSuite.AddTest(max::Testing::Test< max::Testing::CoutResultPolicy >{ "constructor", [](max::Testing::Test< max::Testing::CoutResultPolicy >& CurrentTest, max::Testing::CoutResultPolicy const& ResultPolicy) {
			const size_t BufferLength = 10;
			const size_t BufferCapacity = 20;
			Buffer TestObject(BufferBacking::Memory, BufferLength, BufferCapacity);

			CurrentTest.MAX_TESTING_ASSERT(TestObject.Backing == BufferBacking::Memory);
			CurrentTest.MAX_TESTING_ASSERT(TestObject.Storage != nullptr);
			CurrentTest.MAX_TESTING_ASSERT(TestObject.Length == BufferLength);
			CurrentTest.MAX_TESTING_ASSERT(TestObject.Capacity == BufferCapacity);
		}
		});

		BufferTestSuite.AddTest(max::Testing::Test< max::Testing::CoutResultPolicy >{ "copy constructor", [](max::Testing::Test< max::Testing::CoutResultPolicy >& CurrentTest, max::Testing::CoutResultPolicy const& ResultPolicy) {
			const size_t BufferLength = 10;
			const size_t BufferCapacity = 20;
			Buffer OriginalObject(BufferBacking::Memory, BufferLength, BufferCapacity);

			for (size_t i = 0; i < BufferLength; i++)
			{
				OriginalObject.Storage[i] = static_cast<char>(i);
			}

			Buffer TestObject = OriginalObject;

			CurrentTest.MAX_TESTING_ASSERT(TestObject.Backing == OriginalObject.Backing);
			for (size_t i = 0; i < BufferLength; i++)
			{
				CurrentTest.MAX_TESTING_ASSERT(TestObject.Storage[i] == OriginalObject.Storage[i]);
			}
			CurrentTest.MAX_TESTING_ASSERT(TestObject.Length == OriginalObject.Length);
			CurrentTest.MAX_TESTING_ASSERT(TestObject.Capacity == OriginalObject.Capacity);
		}
		});

		BufferTestSuite.AddTest(max::Testing::Test< max::Testing::CoutResultPolicy >{ "move constructor", [](max::Testing::Test< max::Testing::CoutResultPolicy >& CurrentTest, max::Testing::CoutResultPolicy const& ResultPolicy) {
			const size_t BufferLength = 10;
			const size_t BufferCapacity = 20;
			Buffer OriginalObject(BufferBacking::Memory, BufferLength, BufferCapacity);

			const char* OriginalByteBufferAddress = OriginalObject.Storage.get();

			Buffer TestObject = std::move(OriginalObject);

			CurrentTest.MAX_TESTING_ASSERT(TestObject.Backing == BufferBacking::Memory);
			CurrentTest.MAX_TESTING_ASSERT(OriginalObject.Storage == nullptr);
			CurrentTest.MAX_TESTING_ASSERT(TestObject.Storage.get() == OriginalByteBufferAddress);
			CurrentTest.MAX_TESTING_ASSERT(TestObject.Length == BufferLength);
			CurrentTest.MAX_TESTING_ASSERT(TestObject.Capacity == BufferCapacity);
		}
		});

		BufferTestSuite.AddTest(max::Testing::Test< max::Testing::CoutResultPolicy >{ "copy assignment", [](max::Testing::Test< max::Testing::CoutResultPolicy >& CurrentTest, max::Testing::CoutResultPolicy const& ResultPolicy) {
			const size_t BufferLength = 10;
			const size_t BufferCapacity = 20;
			Buffer OriginalObject(BufferBacking::Memory, BufferLength, BufferCapacity);

			for (size_t i = 0; i < BufferLength; i++)
			{
				OriginalObject.Storage[i] = static_cast<char>(i);
			}

			const size_t SecondBufferLength = 40;
			const size_t SecondBufferCapacity = 50;
			Buffer TestObject(BufferBacking::Memory, SecondBufferLength, SecondBufferCapacity);

			TestObject = OriginalObject;

			CurrentTest.MAX_TESTING_ASSERT(TestObject.Backing == OriginalObject.Backing);
			for (size_t i = 0; i < BufferLength; i++)
			{
				CurrentTest.MAX_TESTING_ASSERT(TestObject.Storage[i] == OriginalObject.Storage[i]);
			}
			CurrentTest.MAX_TESTING_ASSERT(TestObject.Length == OriginalObject.Length);
			CurrentTest.MAX_TESTING_ASSERT(TestObject.Capacity == OriginalObject.Capacity);
		}
		});

		BufferTestSuite.AddTest(max::Testing::Test< max::Testing::CoutResultPolicy >{ "move assignment", [](max::Testing::Test< max::Testing::CoutResultPolicy >& CurrentTest, max::Testing::CoutResultPolicy const& ResultPolicy) {
			const size_t BufferLength = 10;
			const size_t BufferCapacity = 20;
			Buffer OriginalObject(BufferBacking::Memory, BufferLength, BufferCapacity);
			const char* OriginalByteBufferAddress = OriginalObject.Storage.get();
			const size_t SecondBufferLength = 40;
			const size_t SecondBufferCapacity = 50;
			Buffer TestObject(BufferBacking::Memory, SecondBufferLength, SecondBufferCapacity);

			TestObject = std::move(OriginalObject);

			CurrentTest.MAX_TESTING_ASSERT(TestObject.Backing == BufferBacking::Memory);
			CurrentTest.MAX_TESTING_ASSERT(OriginalObject.Storage == nullptr);
			CurrentTest.MAX_TESTING_ASSERT(TestObject.Storage.get() == OriginalByteBufferAddress);
			CurrentTest.MAX_TESTING_ASSERT(TestObject.Length == BufferLength);
			CurrentTest.MAX_TESTING_ASSERT(TestObject.Capacity == BufferCapacity);
		}
		});

		BufferTestSuite.RunTests();
	}

} // namespace maxHex