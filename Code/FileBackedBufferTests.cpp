// Copyright 2019, The maxHex Contributors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "FileBackedBuffer.hpp"
#include "File.hpp"
#include <max/Testing/TestSuite.hpp>
#include <max/Testing/CoutResultPolicy.hpp>
#include <utility>
#if defined(MAX_PLATFORM_WINDOWS)
#include <Windows.h>
#endif

namespace maxHex
{

	void RunFileBackedBufferTestSuite()
	{
		max::Testing::CoutResultPolicy ResultPolicy;
		auto BufferTestSuite = max::Testing::TestSuite< max::Testing::CoutResultPolicy >{ "maxHex::FileBackedBuffer test suite", std::move(ResultPolicy) };

		BufferTestSuite.AddTest(max::Testing::Test< max::Testing::CoutResultPolicy >{ "constructor", [](max::Testing::Test< max::Testing::CoutResultPolicy >& CurrentTest, max::Testing::CoutResultPolicy const& ResultPolicy) {
#if defined(MAX_PLATFORM_WINDOWS)
			LPCTSTR FilePath = TEXT("Test\\Path");
			File TestFile(FilePath);
#endif
			size_t SourceOffset = 0;
			const size_t BufferLength = 10;
			const size_t BufferCapacity = 20;
			FileBackedBuffer TestObject{ std::move(TestFile), std::move(SourceOffset), BufferLength, BufferCapacity };

			CurrentTest.MAX_TESTING_ASSERT(TestObject.SourceFile.FilePath == FilePath);
			CurrentTest.MAX_TESTING_ASSERT(TestObject.SourceOffset == SourceOffset);
			CurrentTest.MAX_TESTING_ASSERT(TestObject.Backing == BufferBacking::File);
			CurrentTest.MAX_TESTING_ASSERT(TestObject.Storage != nullptr);
			CurrentTest.MAX_TESTING_ASSERT(TestObject.Length == BufferLength);
			CurrentTest.MAX_TESTING_ASSERT(TestObject.Capacity == BufferCapacity);
			}
		});

		BufferTestSuite.AddTest(max::Testing::Test< max::Testing::CoutResultPolicy >{ "copy constructor", [](max::Testing::Test< max::Testing::CoutResultPolicy >& CurrentTest, max::Testing::CoutResultPolicy const& ResultPolicy) {
#if defined(MAX_PLATFORM_WINDOWS)
			LPCTSTR FilePath = TEXT("Test\\Path");
			File TestFile(FilePath);
#endif
			size_t SourceOffset = 0;
			const size_t BufferLength = 10;
			const size_t BufferCapacity = 20;
			FileBackedBuffer OriginalObject(std::move(TestFile), std::move(SourceOffset), BufferLength, BufferCapacity);

			for (size_t i = 0; i < BufferLength; i++)
			{
				OriginalObject.Storage[i] = static_cast<char>(i);
			}

			FileBackedBuffer TestObject = OriginalObject;

			CurrentTest.MAX_TESTING_ASSERT(TestObject.SourceFile.FilePath == OriginalObject.SourceFile.FilePath);
			CurrentTest.MAX_TESTING_ASSERT(TestObject.SourceOffset == OriginalObject.SourceOffset);
			CurrentTest.MAX_TESTING_ASSERT(TestObject.Backing == OriginalObject.Backing);
			CurrentTest.MAX_TESTING_ASSERT(TestObject.Length == OriginalObject.Length);
			CurrentTest.MAX_TESTING_ASSERT(TestObject.Capacity == OriginalObject.Capacity);
			for (size_t i = 0; i < BufferLength; i++)
			{
				CurrentTest.MAX_TESTING_ASSERT(TestObject.Storage[i] == OriginalObject.Storage[i]);
			}
			}
		});

		BufferTestSuite.AddTest(max::Testing::Test< max::Testing::CoutResultPolicy >{ "move constructor", [](max::Testing::Test< max::Testing::CoutResultPolicy >& CurrentTest, max::Testing::CoutResultPolicy const& ResultPolicy) {
#if defined(MAX_PLATFORM_WINDOWS)
			LPCTSTR FilePath = TEXT("Test\\Path");
			File TestFile(FilePath);
#endif
			size_t SourceOffset = 0;
			const size_t BufferLength = 10;
			const size_t BufferCapacity = 20;
			FileBackedBuffer OriginalObject(std::move(TestFile), std::move(SourceOffset), BufferLength, BufferCapacity);

			const char* OriginalStorageAddress = OriginalObject.Storage.get();

			FileBackedBuffer TestObject = std::move(OriginalObject);

			CurrentTest.MAX_TESTING_ASSERT(OriginalObject.Storage == nullptr);
			CurrentTest.MAX_TESTING_ASSERT(TestObject.SourceFile.FilePath == FilePath);
			CurrentTest.MAX_TESTING_ASSERT(TestObject.SourceOffset == SourceOffset);
			CurrentTest.MAX_TESTING_ASSERT(TestObject.Backing == BufferBacking::File);
			CurrentTest.MAX_TESTING_ASSERT(TestObject.Storage.get() == OriginalStorageAddress);
			CurrentTest.MAX_TESTING_ASSERT(TestObject.Length == BufferLength);
			CurrentTest.MAX_TESTING_ASSERT(TestObject.Capacity == BufferCapacity);
			}
		});

		BufferTestSuite.AddTest(max::Testing::Test< max::Testing::CoutResultPolicy >{ "copy assignment operator", [](max::Testing::Test< max::Testing::CoutResultPolicy >& CurrentTest, max::Testing::CoutResultPolicy const& ResultPolicy) {
#if defined(MAX_PLATFORM_WINDOWS)
			LPCTSTR FilePath = TEXT("Test\\Path");
			File TestFile(FilePath);
#endif
			size_t SourceOffset = 0;
			const size_t BufferLength = 10;
			const size_t BufferCapacity = 20;
			FileBackedBuffer OriginalObject(std::move(TestFile), std::move(SourceOffset), BufferLength, BufferCapacity);

			for (size_t i = 0; i < BufferLength; i++)
			{
				OriginalObject.Storage[i] = static_cast<char>(i);
			}

#if defined(MAX_PLATFORM_WINDOWS)
			LPCTSTR SecondFilePath = TEXT("Test\\Path");
			File SecondTestFile(SecondFilePath);
#endif
			size_t SecondSourceOffset = 30;
			const size_t SecondBufferLength = 40;
			const size_t SecondBufferCapacity = 50;
			FileBackedBuffer TestObject(std::move(SecondTestFile), std::move(SecondSourceOffset), SecondBufferLength, SecondBufferCapacity);
			TestObject = OriginalObject;


			CurrentTest.MAX_TESTING_ASSERT(TestObject.SourceFile.FilePath == OriginalObject.SourceFile.FilePath);
			CurrentTest.MAX_TESTING_ASSERT(TestObject.SourceOffset == OriginalObject.SourceOffset);
			CurrentTest.MAX_TESTING_ASSERT(TestObject.Backing == OriginalObject.Backing);
			CurrentTest.MAX_TESTING_ASSERT(TestObject.Length == OriginalObject.Length);
			CurrentTest.MAX_TESTING_ASSERT(TestObject.Capacity == OriginalObject.Capacity);
			for (size_t i = 0; i < BufferLength; i++)
			{
				CurrentTest.MAX_TESTING_ASSERT(TestObject.Storage[i] == OriginalObject.Storage[i]);
			}
			}
		});

		BufferTestSuite.AddTest(max::Testing::Test< max::Testing::CoutResultPolicy >{ "move assignment operator", [](max::Testing::Test< max::Testing::CoutResultPolicy >& CurrentTest, max::Testing::CoutResultPolicy const& ResultPolicy) {
#if defined(MAX_PLATFORM_WINDOWS)
			LPCTSTR FilePath = TEXT("Test\\Path");
			File TestFile(FilePath);
#endif
			size_t SourceOffset = 0;
			const size_t BufferLength = 10;
			const size_t BufferCapacity = 20;
			FileBackedBuffer OriginalObject(std::move(TestFile), std::move(SourceOffset), BufferLength, BufferCapacity);
			const char* OriginalStorageAddress = OriginalObject.Storage.get();

#if defined(MAX_PLATFORM_WINDOWS)
			LPCTSTR SecondFilePath = TEXT("Test\\Path");
			File SecondTestFile(SecondFilePath);
#endif
			size_t SecondSourceOffset = 30;
			const size_t SecondBufferLength = 40;
			const size_t SecondBufferCapacity = 50;

			FileBackedBuffer TestObject(std::move(SecondTestFile), std::move(SecondSourceOffset), SecondBufferLength, SecondBufferCapacity);

			TestObject = std::move(OriginalObject);

			CurrentTest.MAX_TESTING_ASSERT(OriginalObject.Storage == nullptr);
			CurrentTest.MAX_TESTING_ASSERT(TestObject.Storage.get() == OriginalStorageAddress);
			CurrentTest.MAX_TESTING_ASSERT(TestObject.SourceFile.FilePath == FilePath);
			CurrentTest.MAX_TESTING_ASSERT(TestObject.SourceOffset == SourceOffset);
			CurrentTest.MAX_TESTING_ASSERT(TestObject.Backing == BufferBacking::File);
			CurrentTest.MAX_TESTING_ASSERT(TestObject.Length == BufferLength);
			CurrentTest.MAX_TESTING_ASSERT(TestObject.Capacity == BufferCapacity);
			}
		});

		BufferTestSuite.RunTests();
	}

} // namespace maxHex