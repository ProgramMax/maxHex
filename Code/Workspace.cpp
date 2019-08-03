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

	Workspace::Workspace(BufferChain Buffers)
		: Buffers(std::move(Buffers))
	{
	}

	Workspace::~Workspace() = default;
	Workspace& Workspace::operator =(const Workspace& rhs) = default;
	Workspace& Workspace::operator =(Workspace&& rhs) = default;

	#if defined(MAX_PLATFORM_WINDOWS)
	Workspace CreateWorkspaceFromFile(LPCTSTR FilePath)
	{
		File SourceFile(FilePath);
		DWORD TotalSizeRead = 0;
		size_t SourceOffset = 0;
		std::vector<Buffer> FileBuffers;
		const size_t ReadChunkSizeInBytes = 100;

		HANDLE FileHandle = CreateFile(FilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		DWORD FileSize = GetFileSize(FileHandle, NULL);
		while (TotalSizeRead < FileSize) {
			SourceOffset = TotalSizeRead;
			FileBuffers.emplace_back(std::move(SourceFile), SourceOffset, 0, ReadChunkSizeInBytes);

			void* buff = FileBuffers.back().ByteBuffer.get();
			DWORD SizeRead = 0;
			ReadFile(FileHandle, (void*)buff, ReadChunkSizeInBytes, &SizeRead, NULL);
			FileBuffers.back().ByteBufferLength = SizeRead;
			if (ReadChunkSizeInBytes != SizeRead) {
				// TODO: Allow buffers to contain more data than they use
			}
			TotalSizeRead += SizeRead;
		}
		CloseHandle(FileHandle);

		BufferChain FileBufferChain(std::move(FileBuffers));

		return Workspace(std::move(FileBufferChain));
	}
	#endif

} // namespace maxHex