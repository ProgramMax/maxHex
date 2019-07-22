// Copyright 2019, The maxHex Contributors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Workspace.hpp"

#include <Windows.h>
#include <utility>
#include "File.hpp"

namespace maxHex
{

	Workspace::Workspace() = default;
	Workspace::Workspace(const Workspace& rhs) = default;
	Workspace::Workspace(Workspace&& rhs) = default;

	Workspace::Workspace(BufferChain&& Buffers)
		: Buffers(std::move(Buffers))
	{
	}

	Workspace::~Workspace() = default;
	Workspace& Workspace::operator =(const Workspace& rhs) = default;
	Workspace& Workspace::operator =(Workspace&& rhs) = default;

	#if defined(MAX_PLATFORM_WINDOWS)
	Workspace CreateWorkspaceFromFile(LPCTSTR FilePath)
	{
		HANDLE FileHandle = CreateFile(FilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		DWORD FileSize = GetFileSize(FileHandle, NULL);

		File SourceFile(FilePath);
		size_t SourceOffset = 0;
		Buffer FileBuffer(std::move(SourceFile), std::move(SourceOffset), static_cast<size_t>(FileSize));
		BufferChain FileBufferChain(std::move(FileBuffer));

		void* buff = FileBufferChain.BufferList[0].ByteBuffer.get();
		DWORD SizeRead;
		ReadFile(FileHandle, (void*)buff, FileSize, &SizeRead, NULL);
		CloseHandle(FileHandle);

		return Workspace(std::move(FileBufferChain));
	}
	#endif

} // namespace maxHex