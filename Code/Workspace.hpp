// Copyright 2019, The maxHex Contributors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MAXHEX_WORKSPACE_HPP
#define MAXHEX_WORKSPACE_HPP

#include <max/Compiling/Configuration.hpp>
#if defined(MAX_PLATFORM_WINDOWS)
#include <Windows.h>
#endif

#include "BufferChain.hpp"

namespace maxHex
{

	class Workspace
	{
	public:

		Workspace();
		Workspace(const Workspace& rhs);
		Workspace(Workspace&& rhs);
		explicit Workspace(BufferChain&& Buffers);
		~Workspace();

		Workspace& operator =(const Workspace& rhs);
		Workspace& operator =(Workspace&& rhs);

		BufferChain Buffers;

	};

	#if defined(MAX_PLATFORM_WINDOWS)
	Workspace CreateWorkspaceFromFile(LPCTSTR FilePath);
	#endif

} // namespace maxHex

#endif // #ifndef MAXHEX_WORKSPACE_HPP