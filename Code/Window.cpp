// Copyright 2019, The maxHex Contributors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Window.hpp"

#include <maxGUI/Form.hpp>

#include "Buffer.hpp"
#include "UserInteractionState.hpp"
#include "Workspace.hpp"

#include <strsafe.h>
#include <algorithm>

#include <shellapi.h>

namespace maxHex
{

	std::unique_ptr<maxHex::Font> TestFont = nullptr;
	std::unique_ptr<maxHex::Rasterizer> TestRasterizer = nullptr;

namespace
{

	maxHex::Workspace TestWorkspace;
	maxHex::UserInteractionState TestUserInteractionState;

	int MaxWidth;
	ULONG LinesToScrollPerThreshold = 0;
	ULONG CharsToScrollPerThreshold = 0;
	int ClientHeight;
	int ClientWidth;
	SCROLLINFO ScrollInfo;


} // anonymous namespace

	void MainForm::OnCreated(maxGUI::FormConcept* form) noexcept {
		HDC DeviceContext = GetDC(form->window_handle_);

		TestRasterizer = std::make_unique<maxHex::Rasterizer>(maxHex::Rasterizer::Create(DeviceContext, *TestFont));
		ReleaseDC(form->window_handle_, DeviceContext);

		MaxWidth = 72 * TestRasterizer->CharacterWidth; // TODO: Find real value

		size_t TotalSize = 0;
		for (const std::unique_ptr<maxHex::Buffer>& CurrentBuffer : TestWorkspace.Buffers.BufferList)
		{
			TotalSize += CurrentBuffer->Length;
		}
		size_t LineCount = 0;
		if (TotalSize != 0) {
			LineCount = (TotalSize / 16) + 1;
		}
		// TODO: Figure out how to store larger numbers in the scroll range
		SetScrollRange(form->window_handle_, SB_VERT, 0, static_cast<int>(LineCount) - 1, FALSE);
		SetScrollPos(form->window_handle_, SB_VERT, 0, TRUE);

		SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &LinesToScrollPerThreshold, 0);
		SystemParametersInfo(SPI_GETWHEELSCROLLCHARS, 0, &CharsToScrollPerThreshold, 0);
	}

	void MainForm::OnClosed(maxGUI::FormConcept* /*form*/) noexcept {
		maxGUI::PostExitMessage(0);
	}

	void MainForm::OnResized(maxGUI::FormConcept* form, int new_height, int new_width) noexcept {
		size_t TotalSize = 0;
		for (const std::unique_ptr<maxHex::Buffer>& CurrentBuffer : TestWorkspace.Buffers.BufferList)
		{
			TotalSize += CurrentBuffer->Length;
		}
		size_t LineCount = 0;
		if (TotalSize != 0) {
			LineCount = (TotalSize / 16) + 1;
		}
		ClientHeight = new_height;
		ClientWidth = new_width;

		ScrollInfo.cbSize = sizeof(ScrollInfo);
		ScrollInfo.fMask = SIF_RANGE | SIF_PAGE;
		ScrollInfo.nMin = 0;
		ScrollInfo.nMax = static_cast<int>(LineCount);
		ScrollInfo.nPage = ClientHeight / TestRasterizer->CharacterHeight;
		SetScrollInfo(form->window_handle_, SB_VERT, &ScrollInfo, TRUE);

		ScrollInfo.cbSize = sizeof(ScrollInfo);
		ScrollInfo.fMask = SIF_RANGE | SIF_PAGE;
		ScrollInfo.nMin = 0;
		ScrollInfo.nMax = 78;
		ScrollInfo.nPage = ClientWidth / TestRasterizer->CharacterWidth;
		SetScrollInfo(form->window_handle_, SB_HORZ, &ScrollInfo, TRUE);
	}

	LRESULT MainForm::OnWindowMessage(maxGUI::FormConcept* form, UINT message, WPARAM wparam, LPARAM lparam) noexcept {
		static int VerticalScrollAccumulation = 0;
		static int HorizontalScrollAccumulation = 0;

		switch (message)
		{
		case WM_SETTINGCHANGE:
			SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &LinesToScrollPerThreshold, 0);
			SystemParametersInfo(SPI_GETWHEELSCROLLCHARS, 0, &CharsToScrollPerThreshold, 0);
			return 0;
		case WM_VSCROLL:
		{
			ScrollInfo.cbSize = sizeof(ScrollInfo);
			ScrollInfo.fMask = SIF_ALL;
			GetScrollInfo(form->window_handle_, SB_VERT, &ScrollInfo);

			switch (LOWORD(wparam))
			{
			case SB_TOP:
				ScrollInfo.nPos = ScrollInfo.nMin;
				break;
			case SB_BOTTOM:
				ScrollInfo.nPos = ScrollInfo.nMax;
				break;
			case SB_LINEUP:
				ScrollInfo.nPos -= 1;
				break;
			case SB_LINEDOWN:
				ScrollInfo.nPos += 1;
				break;
			case SB_PAGEUP:
				ScrollInfo.nPos -= ScrollInfo.nPage;
				break;
			case SB_PAGEDOWN:
				ScrollInfo.nPos += ScrollInfo.nPage;
				break;
			case SB_THUMBTRACK:
				ScrollInfo.nPos = ScrollInfo.nTrackPos;
				break;
				// Might not need this one now that we have THUMBTRACK
				//case SB_THUMBPOSITION:
					//ScrollInfo.nPos = HIWORD(wParam);
					//break;
			default:
				break;
			}
			ScrollInfo.fMask = SIF_POS;
			SetScrollInfo(form->window_handle_, SB_VERT, &ScrollInfo, TRUE);
			GetScrollInfo(form->window_handle_, SB_VERT, &ScrollInfo);

			size_t TotalSize = 0;
			for (const std::unique_ptr<maxHex::Buffer>& CurrentBuffer : TestWorkspace.Buffers.BufferList)
			{
				TotalSize += CurrentBuffer->Length;
			}
			size_t LineCount = 0;
			if (TotalSize != 0) {
				LineCount = TotalSize / 16;
			}

			if (TestUserInteractionState.VerticalScrollOffset != ScrollInfo.nPos)
			{
				int ScrollDifference = TestUserInteractionState.VerticalScrollOffset - ScrollInfo.nPos;

				TestUserInteractionState.VerticalScrollOffset = ScrollInfo.nPos;

				RECT ScrollArea;
				GetClientRect(form->window_handle_, &ScrollArea);
				// Leave the header
				ScrollArea.top += TestRasterizer->CharacterHeight;
				ScrollWindowEx(form->window_handle_, 0, TestRasterizer->CharacterHeight * ScrollDifference, &ScrollArea, &ScrollArea, NULL, NULL, SW_INVALIDATE);
				UpdateWindow(form->window_handle_);
			}
			return 0;
		}
		case WM_HSCROLL:
			ScrollInfo.cbSize = sizeof(ScrollInfo);
			ScrollInfo.fMask = SIF_ALL;
			GetScrollInfo(form->window_handle_, SB_HORZ, &ScrollInfo);

			switch (LOWORD(wparam))
			{
			case SB_LINELEFT:
				ScrollInfo.nPos -= 1;
				break;
			case SB_LINERIGHT:
				ScrollInfo.nPos += 1;
				break;
			case SB_PAGELEFT:
				ScrollInfo.nPos -= ScrollInfo.nPage;
				break;
			case SB_PAGERIGHT:
				ScrollInfo.nPos += ScrollInfo.nPage;
				break;
			case SB_THUMBTRACK:
				ScrollInfo.nPos = ScrollInfo.nTrackPos;
				break;
			default:
				break;
			}
			ScrollInfo.fMask = SIF_POS;
			SetScrollInfo(form->window_handle_, SB_HORZ, &ScrollInfo, TRUE);
			GetScrollInfo(form->window_handle_, SB_HORZ, &ScrollInfo);

			if (TestUserInteractionState.HorizontalScrollOffset != ScrollInfo.nPos)
			{
				int ScrollDifference = TestUserInteractionState.HorizontalScrollOffset - ScrollInfo.nPos;
				TestUserInteractionState.HorizontalScrollOffset = ScrollInfo.nPos;

				RECT ScrollArea;
				GetClientRect(form->window_handle_, &ScrollArea);
				ScrollWindowEx(form->window_handle_, TestRasterizer->CharacterWidth * ScrollDifference, 0, &ScrollArea, &ScrollArea, NULL, NULL, SW_INVALIDATE);
				UpdateWindow(form->window_handle_);
			}
			return 0;
		case WM_KEYDOWN:
			switch (wparam)
			{
			case VK_HOME:
			case VK_END:
				break;
			case VK_PRIOR:
				SendMessage(form->window_handle_, WM_VSCROLL, SB_PAGEUP, 0);
				break;
			case VK_NEXT:
				SendMessage(form->window_handle_, WM_VSCROLL, SB_PAGEDOWN, 0);
				break;
			case VK_UP:
				SendMessage(form->window_handle_, WM_VSCROLL, SB_LINEUP, 0);
				break;
			case VK_DOWN:
				SendMessage(form->window_handle_, WM_VSCROLL, SB_LINEDOWN, 0);
				break;
			case VK_LEFT:
				SendMessage(form->window_handle_, WM_HSCROLL, SB_LINELEFT, 0);
				break;
			case VK_RIGHT:
				SendMessage(form->window_handle_, WM_HSCROLL, SB_LINERIGHT, 0);
				break;
			}
			return 0;
		case WM_MOUSEWHEEL:
		{
			if (LinesToScrollPerThreshold == 0)
				break;

			ScrollInfo.cbSize = sizeof(ScrollInfo);
			ScrollInfo.fMask = SIF_ALL;
			GetScrollInfo(form->window_handle_, SB_VERT, &ScrollInfo);

			short AccumulatedDelta = (short)GET_WHEEL_DELTA_WPARAM(wparam);
			VerticalScrollAccumulation += AccumulatedDelta;
			int DeltaPerLine = WHEEL_DELTA;
			if (LinesToScrollPerThreshold != WHEEL_PAGESCROLL)
				DeltaPerLine = WHEEL_DELTA / LinesToScrollPerThreshold;
			int DeltasCompleted = VerticalScrollAccumulation / DeltaPerLine;
			VerticalScrollAccumulation -= DeltasCompleted * DeltaPerLine;
			int LinesToScroll = 0;
			if (LinesToScrollPerThreshold == WHEEL_PAGESCROLL)
			{
				if (AccumulatedDelta < 0)
				{
					LinesToScroll = -static_cast<int>(ScrollInfo.nPage) * DeltasCompleted;
				}
				else {
					LinesToScroll = ScrollInfo.nPage * DeltasCompleted;
				}
			}
			else {
				LinesToScroll = DeltasCompleted;
			}

			ScrollInfo.nPos -= LinesToScroll;
			ScrollInfo.fMask = SIF_POS;
			SetScrollInfo(form->window_handle_, SB_VERT, &ScrollInfo, TRUE);
			GetScrollInfo(form->window_handle_, SB_VERT, &ScrollInfo);

			size_t TotalSize = 0;
			for (const std::unique_ptr<maxHex::Buffer>& CurrentBuffer : TestWorkspace.Buffers.BufferList)
			{
				TotalSize += CurrentBuffer->Length;
			}
			size_t LineCount = 0;
			if (TotalSize != 0) {
				LineCount = TotalSize / 16;
			}
			if (TestUserInteractionState.VerticalScrollOffset != ScrollInfo.nPos)
			{
				int ScrollDifference = TestUserInteractionState.VerticalScrollOffset - ScrollInfo.nPos;

				TestUserInteractionState.VerticalScrollOffset = ScrollInfo.nPos;

				RECT ScrollArea;
				GetClientRect(form->window_handle_, &ScrollArea);
				// Leave the header
				ScrollArea.top += TestRasterizer->CharacterHeight;
				ScrollWindowEx(form->window_handle_, 0, TestRasterizer->CharacterHeight * ScrollDifference, &ScrollArea, &ScrollArea, NULL, NULL, SW_INVALIDATE);
				UpdateWindow(form->window_handle_);
			}
			return 0;
		}
		case WM_MOUSEHWHEEL: // Horizontal mouse wheel
		{
			ScrollInfo.cbSize = sizeof(ScrollInfo);
			ScrollInfo.fMask = SIF_ALL;
			GetScrollInfo(form->window_handle_, SB_HORZ, &ScrollInfo);

			short AccumulatedDelta = (short)GET_WHEEL_DELTA_WPARAM(wparam);
			HorizontalScrollAccumulation += AccumulatedDelta;
			int DeltaPerChar = WHEEL_DELTA / CharsToScrollPerThreshold;
			int DeltasCompleted = HorizontalScrollAccumulation / DeltaPerChar;
			HorizontalScrollAccumulation -= DeltasCompleted * DeltaPerChar;
			int CharactersToScroll = DeltasCompleted;

			ScrollInfo.nPos += CharactersToScroll;
			ScrollInfo.fMask = SIF_POS;
			SetScrollInfo(form->window_handle_, SB_HORZ, &ScrollInfo, TRUE);
			GetScrollInfo(form->window_handle_, SB_HORZ, &ScrollInfo);

			if (TestUserInteractionState.HorizontalScrollOffset != ScrollInfo.nPos)
			{
				int ScrollDifference = TestUserInteractionState.HorizontalScrollOffset - ScrollInfo.nPos;

				TestUserInteractionState.HorizontalScrollOffset = ScrollInfo.nPos;

				RECT ScrollArea;
				GetClientRect(form->window_handle_, &ScrollArea);
				ScrollWindowEx(form->window_handle_, TestRasterizer->CharacterWidth * ScrollDifference, 0, &ScrollArea, &ScrollArea, NULL, NULL, SW_INVALIDATE);
				UpdateWindow(form->window_handle_);
			}

			// Despite the docs saying you should return 0 if you handle the message, you actually need to return non-zero.
			// If you return zero it will convert the message to a WM_HSCROLL with SB_THUMBTRACK. It will also paint over
			// the scroll bar with an old Windows style until the message is done processing. This will cause a flicker.
			return 1;
		}
		case WM_PAINT:
		{
			PAINTSTRUCT PaintStruct;
			HDC DeviceContext = BeginPaint(form->window_handle_, &PaintStruct);

			TestRasterizer->Rasterize(form->window_handle_, DeviceContext, PaintStruct.rcPaint.top, PaintStruct.rcPaint.left, PaintStruct.rcPaint.bottom - PaintStruct.rcPaint.top, PaintStruct.rcPaint.right - PaintStruct.rcPaint.left, TestWorkspace.Buffers, TestUserInteractionState);

			EndPaint(form->window_handle_, &PaintStruct);
			return 0;
		}
		case WM_DROPFILES:
		{
			UINT FileCount = DragQueryFile((HDROP)wparam, 0xFFFFFFFF, NULL, 0);

			for (UINT i = 0; i < FileCount; i++)
			{
				UINT BufferCharacterCount = DragQueryFile((HDROP)wparam, i, NULL, 0) + 1;
				TCHAR* Buffer = new TCHAR[BufferCharacterCount];
				DragQueryFile((HDROP)wparam, i, Buffer, BufferCharacterCount);

				TestWorkspace = maxHex::CreateWorkspaceFromFile(maxHex::File(Buffer));
				size_t TotalSize = 0;
				for (const std::unique_ptr<maxHex::Buffer>& CurrentBuffer : TestWorkspace.Buffers.BufferList)
				{
					TotalSize += CurrentBuffer->Length;
				}
				size_t LineCount = (TotalSize / 16) + 1;
				InvalidateRect(form->window_handle_, NULL, FALSE);
				SetScrollRange(form->window_handle_, SB_VERT, 0, static_cast<int>(LineCount) - 1, FALSE);
				SetScrollPos(form->window_handle_, SB_VERT, 0, TRUE);
				UpdateWindow(form->window_handle_);
				delete[] Buffer;
			}
			DragFinish((HDROP)wparam);
			return 0;
		}
		}

		return DefWindowProc(form->window_handle_, message, wparam, lparam);
	}

} // namespace maxHex