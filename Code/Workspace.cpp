// Copyright 2019, The maxHex Contributors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Workspace.hpp"

#include <Windows.h>
#include <vector>
#include <memory>
#include <utility>
#include "File.hpp"
#include "FileBackedBuffer.hpp"

#include <max/Compiling/Configuration.hpp>
#if defined(MAX_PLATFORM_WINDOWS)
#include <Windows.h>
#endif

namespace maxHex
{

	Workspace::Workspace() noexcept = default;
	
	Workspace::Workspace(BufferChain Buffers) noexcept
		: Buffers(std::move(Buffers))
	{
	}

	// TODO: Make BufferChain's copy ctor work
	//Workspace::Workspace(const Workspace& rhs) noexcept = default;
	Workspace::Workspace(Workspace&& rhs) noexcept = default;

	Workspace::~Workspace() noexcept = default;
	// TODO: Make BufferChain's copy ctor work
	//Workspace& Workspace::operator =(const Workspace& rhs) noexcept = default;
	Workspace& Workspace::operator =(Workspace&& rhs) noexcept = default;

	#if defined(MAX_PLATFORM_WINDOWS)
	Workspace CreateWorkspaceFromFile(File FileToOpen) noexcept
	{
		DWORD TotalSizeRead = 0;
		size_t SourceOffset = 0;
		std::vector<std::unique_ptr<Buffer>> FileBuffers;
		const size_t ReadChunkSizeInBytes = 4 * 1024;

		HANDLE FileHandle = CreateFile(FileToOpen.FilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		DWORD FileSize = GetFileSize(FileHandle, NULL);
		while (TotalSizeRead < FileSize) {
			SourceOffset = TotalSizeRead;

			auto CurrentBuffer = std::make_unique<FileBackedBuffer>( FileToOpen, SourceOffset, 0, ReadChunkSizeInBytes );
			void* RawBuffer = CurrentBuffer->Storage.get();
			DWORD SizeRead = 0;
			if (!ReadFile(FileHandle, RawBuffer, ReadChunkSizeInBytes, &SizeRead, NULL))
			{
				auto ErrorNumber = GetLastError();
				switch (ErrorNumber)
				{
				case ERROR_IO_PENDING:
					// not an error
				default:
					break;
				}
			}
			CurrentBuffer->Length = SizeRead;
			FileBuffers.push_back(std::move(CurrentBuffer));
			if (ReadChunkSizeInBytes != SizeRead) {
				// TODO: Allow buffers to contain more data than they use
			}
			TotalSizeRead += SizeRead;
		}
		CloseHandle(FileHandle);

		BufferChain FileBufferChain{ std::move(FileBuffers) };

		return Workspace{ std::move(FileBufferChain) };
	}
	#endif

} // namespace maxHex