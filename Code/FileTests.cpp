// Copyright 2019, The maxHex Contributors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "File.hpp"
#include <max/Testing/TestSuite.hpp>
#include <max/Testing/CoutResultPolicy.hpp>

namespace maxHex
{

	void RunFileTestSuite()
	{
		max::Testing::CoutResultPolicy ResultPolicy;
		auto FileTestSuite = max::Testing::TestSuite< max::Testing::CoutResultPolicy >{ "maxHex::File test suite", std::move(ResultPolicy) };

		FileTestSuite.AddTest(max::Testing::Test< max::Testing::CoutResultPolicy >{ "constructor", [](max::Testing::Test< max::Testing::CoutResultPolicy >& CurrentTest, max::Testing::CoutResultPolicy const& ResultPolicy) {
			#if defined(MAX_PLATFORM_WINDOWS)
			LPCTSTR FilePath = TEXT("Test\Path");
			File TestObject(FilePath);

			CurrentTest.MAX_TESTING_ASSERT(TestObject.FilePath == FilePath);
			#endif
		}
		});

		FileTestSuite.RunTests();
	}

} // namespace maxHex