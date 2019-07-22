// Copyright 2019, The maxHex Contributors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Workspace.hpp"

#include <Windows.h>
#include <utility>

namespace maxHex
{

	/*
	Workspace::Workspace()
		: Buffer("Hello world!Testing1234567890")
		, BufferLength(29)
	{
	}
	*/

	Workspace::Workspace()
	{
		Buffer = nullptr;
		BufferLength = 0;
	}

	Workspace::Workspace(Workspace&& rhs)
		: Buffer(std::move(rhs.Buffer))
		, BufferLength(std::move(rhs.BufferLength))
	{
		rhs.Buffer = nullptr;
	}

	Workspace::~Workspace()
	{
		delete[] Buffer;
	}

	Workspace& Workspace::operator =(Workspace&& rhs)
	{
		Buffer = std::move(rhs.Buffer);
		BufferLength = std::move(rhs.BufferLength);

		rhs.Buffer = nullptr;

		return *this;
	}

	Workspace::Workspace(LPTSTR FilePath)
	{
		HANDLE FileHandle = CreateFile(FilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		DWORD FileSize = GetFileSize(FileHandle, NULL);
		Buffer = new char[FileSize];
		BufferLength = FileSize;

		DWORD SizeRead;
		ReadFile(FileHandle, (void*)Buffer, FileSize, &SizeRead, NULL);
		CloseHandle(FileHandle);
	}

	Workspace CreateWorkspaceFromFile(LPTSTR FilePath)
	{
		return Workspace(FilePath);
	}

} // namespace maxHex