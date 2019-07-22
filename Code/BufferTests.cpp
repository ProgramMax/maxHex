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
		auto BufferTestSuite = max::Testing::TestSuite< max::Testing::CoutResultPolicy >{ "maxhex::Buffer test suite", std::move(ResultPolicy) };

		BufferTestSuite.AddTest(max::Testing::Test< max::Testing::CoutResultPolicy >{ "constructor allocates memory", [](max::Testing::Test< max::Testing::CoutResultPolicy >& CurrentTest, max::Testing::CoutResultPolicy const& ResultPolicy) {
			const size_t BufferLength = 10;
			Buffer TestObject(BufferLength);

			CurrentTest.MAX_TESTING_ASSERT(TestObject.ByteBuffer != nullptr);
		}
		});

		BufferTestSuite.AddTest(max::Testing::Test< max::Testing::CoutResultPolicy >{ "constructor assigns buffer length", [](max::Testing::Test< max::Testing::CoutResultPolicy >& CurrentTest, max::Testing::CoutResultPolicy const& ResultPolicy) {
			const size_t BufferLength = 10;
			Buffer TestObject(BufferLength);

			CurrentTest.MAX_TESTING_ASSERT(TestObject.ByteBufferLength == BufferLength);
		}
		});

		BufferTestSuite.AddTest(max::Testing::Test< max::Testing::CoutResultPolicy >{ "copy constructor copies elements", [](max::Testing::Test< max::Testing::CoutResultPolicy >& CurrentTest, max::Testing::CoutResultPolicy const& ResultPolicy) {
			const size_t BufferLength = 10;
			Buffer OriginalObject(BufferLength);
			for (size_t i = 0; i < BufferLength; i++)
			{
				OriginalObject.ByteBuffer[i] = static_cast<char>(i);
			}

			Buffer TestObject = OriginalObject;
			for (size_t i = 0; i < BufferLength; i++)
			{
				CurrentTest.MAX_TESTING_ASSERT(TestObject.ByteBuffer[i] == OriginalObject.ByteBuffer[i]);
			}
		}
		});

		BufferTestSuite.AddTest(max::Testing::Test< max::Testing::CoutResultPolicy >{ "copy constructor matches length", [](max::Testing::Test< max::Testing::CoutResultPolicy >& CurrentTest, max::Testing::CoutResultPolicy const& ResultPolicy) {
			const size_t BufferLength = 10;
			Buffer OriginalObject(BufferLength);

			Buffer TestObject = OriginalObject;
			CurrentTest.MAX_TESTING_ASSERT(TestObject.ByteBufferLength == OriginalObject.ByteBufferLength);
		}
		});

		BufferTestSuite.AddTest(max::Testing::Test< max::Testing::CoutResultPolicy >{ "move constructor moves byte buffer", [](max::Testing::Test< max::Testing::CoutResultPolicy >& CurrentTest, max::Testing::CoutResultPolicy const& ResultPolicy) {
			const size_t BufferLength = 10;
			Buffer OriginalObject(BufferLength);
			const char* OriginalByteBufferAddress = OriginalObject.ByteBuffer.get();

			Buffer TestObject = std::move(OriginalObject);

			CurrentTest.MAX_TESTING_ASSERT(TestObject.ByteBuffer.get() == OriginalByteBufferAddress);
		}
		});

		BufferTestSuite.AddTest(max::Testing::Test< max::Testing::CoutResultPolicy >{ "move constructor nulls old byte buffer", [](max::Testing::Test< max::Testing::CoutResultPolicy >& CurrentTest, max::Testing::CoutResultPolicy const& ResultPolicy) {
			const size_t BufferLength = 10;
			Buffer OriginalObject(BufferLength);

			Buffer TestObject = std::move(OriginalObject);
			CurrentTest.MAX_TESTING_ASSERT(OriginalObject.ByteBuffer == nullptr);
		}
		});

		BufferTestSuite.AddTest(max::Testing::Test< max::Testing::CoutResultPolicy >{ "move constructor moves byte buffer length", [](max::Testing::Test< max::Testing::CoutResultPolicy >& CurrentTest, max::Testing::CoutResultPolicy const& ResultPolicy) {
			const size_t BufferLength = 10;
			Buffer OriginalObject(BufferLength);

			Buffer TestObject = std::move(OriginalObject);

			CurrentTest.MAX_TESTING_ASSERT(TestObject.ByteBufferLength == BufferLength);
		}
		});

		BufferTestSuite.AddTest(max::Testing::Test< max::Testing::CoutResultPolicy >{ "copy assignment operator copies elements", [](max::Testing::Test< max::Testing::CoutResultPolicy >& CurrentTest, max::Testing::CoutResultPolicy const& ResultPolicy) {
			const size_t BufferLength = 10;
			Buffer OriginalObject(BufferLength);
			for (size_t i = 0; i < BufferLength; i++)
			{
				OriginalObject.ByteBuffer[i] = static_cast<char>(i);
			}

			Buffer TestObject(BufferLength);
			TestObject = OriginalObject;

			for (size_t i = 0; i < BufferLength; i++)
			{
				CurrentTest.MAX_TESTING_ASSERT(TestObject.ByteBuffer[i] == OriginalObject.ByteBuffer[i]);
			}
		}
		});

		BufferTestSuite.AddTest(max::Testing::Test< max::Testing::CoutResultPolicy >{ "copy assignment operator matches length", [](max::Testing::Test< max::Testing::CoutResultPolicy >& CurrentTest, max::Testing::CoutResultPolicy const& ResultPolicy) {
			const size_t BufferLength = 10;
			Buffer OriginalObject(BufferLength);

			Buffer TestObject(BufferLength);
			TestObject = OriginalObject;
			CurrentTest.MAX_TESTING_ASSERT(TestObject.ByteBufferLength == OriginalObject.ByteBufferLength);
		}
		});

		BufferTestSuite.AddTest(max::Testing::Test< max::Testing::CoutResultPolicy >{ "move assignment operator nulls old byte buffer", [](max::Testing::Test< max::Testing::CoutResultPolicy >& CurrentTest, max::Testing::CoutResultPolicy const& ResultPolicy) {
			const size_t BufferLength = 10;
			Buffer OriginalObject(BufferLength);

			Buffer TestObject(BufferLength);
			TestObject = std::move(OriginalObject);

			CurrentTest.MAX_TESTING_ASSERT(OriginalObject.ByteBuffer == nullptr);
		}
		});

		BufferTestSuite.AddTest(max::Testing::Test< max::Testing::CoutResultPolicy >{ "move constructor moves byte buffer length", [](max::Testing::Test< max::Testing::CoutResultPolicy >& CurrentTest, max::Testing::CoutResultPolicy const& ResultPolicy) {
			const size_t BufferLength = 10;
			Buffer OriginalObject(BufferLength);

			Buffer TestObject(BufferLength);
			TestObject = std::move(OriginalObject);

			CurrentTest.MAX_TESTING_ASSERT(TestObject.ByteBufferLength == BufferLength);
		}
		});

		BufferTestSuite.RunTests();
	}

} // namespace maxHex