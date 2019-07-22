// Copyright 2019, The maxHex Contributors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "BufferTests.hpp"
#include "BufferChainTests.hpp"
#include "FileTests.hpp"

int main()
{
	maxHex::RunBufferTestSuite();
	maxHex::RunBufferChainTestSuite();
	maxHex::RunFileTestSuite();

	return 0;
}
