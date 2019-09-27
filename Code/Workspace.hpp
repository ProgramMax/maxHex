// Copyright 2019, The maxHex Contributors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MAXHEX_WORKSPACE_HPP
#define MAXHEX_WORKSPACE_HPP

#include <BufferChain.hpp>
#include <File.hpp>

namespace maxHex
{

	class Workspace
	{
	public:

		Workspace() noexcept;
		explicit Workspace(BufferChain Buffers) noexcept;
		Workspace(const Workspace& rhs);
		Workspace(Workspace&& rhs) noexcept;
		~Workspace() noexcept;

		Workspace& operator =(const Workspace& rhs);
		Workspace& operator =(Workspace&& rhs) noexcept;

		BufferChain Buffers;

	};

	Workspace CreateWorkspaceFromFile(File FileToOpen) noexcept;

} // namespace maxHex

#endif // #ifndef MAXHEX_WORKSPACE_HPP