// Copyright 2019, The maxHex Contributors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Buffer.hpp"
#include "Font.hpp"
#include "Rasterizer.hpp"
#include "UserInteractionState.hpp"
#include "Window.hpp"
#include "Workspace.hpp"

#include <strsafe.h>
#include <algorithm>

namespace maxHex
{
namespace
{

	maxHex::Workspace TestWorkspace;
	maxHex::UserInteractionState TestUserInteractionState;
	std::unique_ptr<maxHex::Rasterizer> TestRasterizer; // TODO: The Form should own this as a non-unique_ptr
	std::unique_ptr<maxHex::Font> TestFont; // TODO: The Form should own this as a non-unique_ptr

	LRESULT CALLBACK WindowProcedure(HWND WindowHandle, UINT Message, WPARAM wParam, LPARAM lParam)
	{
		static int ClientHeight;
		static int ClientWidth;
		static int MaxWidth;
		SCROLLINFO ScrollInfo;
		static ULONG LinesToScrollPerThreshold = 0;
		static ULONG CharsToScrollPerThreshold = 0;
		static int VerticalScrollAccumulation = 0;
		static int HorizontalScrollAccumulation = 0;

		switch (Message)
		{
		case WM_CREATE:
		{
			HDC DeviceContext = GetDC(WindowHandle);

			TestRasterizer = std::make_unique<maxHex::Rasterizer>(maxHex::Rasterizer::Create(DeviceContext, *TestFont));
			ReleaseDC(WindowHandle, DeviceContext);

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
			SetScrollRange(WindowHandle, SB_VERT, 0, static_cast<int>(LineCount) - 1, FALSE);
			SetScrollPos(WindowHandle, SB_VERT, 0, TRUE);
			//return 0;
			// fall through
		}
		case WM_SETTINGCHANGE:
			SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &LinesToScrollPerThreshold, 0);
			SystemParametersInfo(SPI_GETWHEELSCROLLCHARS, 0, &CharsToScrollPerThreshold, 0);
			return 0;
		case WM_SIZE:
		{
			size_t TotalSize = 0;
			for (const std::unique_ptr<maxHex::Buffer>& CurrentBuffer : TestWorkspace.Buffers.BufferList)
			{
				TotalSize += CurrentBuffer->Length;
			}
			size_t LineCount = 0;
			if (TotalSize != 0) {
				LineCount = (TotalSize / 16) + 1;
			}
			ClientHeight = HIWORD(lParam);
			ClientWidth = LOWORD(lParam);

			ScrollInfo.cbSize = sizeof(ScrollInfo);
			ScrollInfo.fMask = SIF_RANGE | SIF_PAGE;
			ScrollInfo.nMin = 0;
			ScrollInfo.nMax = static_cast<int>(LineCount);
			ScrollInfo.nPage = ClientHeight / TestRasterizer->CharacterHeight;
			SetScrollInfo(WindowHandle, SB_VERT, &ScrollInfo, TRUE);

			ScrollInfo.cbSize = sizeof(ScrollInfo);
			ScrollInfo.fMask = SIF_RANGE | SIF_PAGE;
			ScrollInfo.nMin = 0;
			ScrollInfo.nMax = 78;
			ScrollInfo.nPage = ClientWidth / TestRasterizer->CharacterWidth;
			SetScrollInfo(WindowHandle, SB_HORZ, &ScrollInfo, TRUE);
			return 0;
		}
		case WM_VSCROLL:
		{
			ScrollInfo.cbSize = sizeof(ScrollInfo);
			ScrollInfo.fMask = SIF_ALL;
			GetScrollInfo(WindowHandle, SB_VERT, &ScrollInfo);

			switch (LOWORD(wParam))
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
			SetScrollInfo(WindowHandle, SB_VERT, &ScrollInfo, TRUE);
			GetScrollInfo(WindowHandle, SB_VERT, &ScrollInfo);

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
				GetClientRect(WindowHandle, &ScrollArea);
				// Leave the header
				ScrollArea.top += TestRasterizer->CharacterHeight;
				ScrollWindowEx(WindowHandle, 0, TestRasterizer->CharacterHeight * ScrollDifference, &ScrollArea, &ScrollArea, NULL, NULL, SW_INVALIDATE);
				UpdateWindow(WindowHandle);
			}
			return 0;
		}
		case WM_HSCROLL:
			ScrollInfo.cbSize = sizeof(ScrollInfo);
			ScrollInfo.fMask = SIF_ALL;
			GetScrollInfo(WindowHandle, SB_HORZ, &ScrollInfo);

			switch (LOWORD(wParam))
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
			SetScrollInfo(WindowHandle, SB_HORZ, &ScrollInfo, TRUE);
			GetScrollInfo(WindowHandle, SB_HORZ, &ScrollInfo);

			if (TestUserInteractionState.HorizontalScrollOffset != ScrollInfo.nPos)
			{
				int ScrollDifference = TestUserInteractionState.HorizontalScrollOffset - ScrollInfo.nPos;
				TestUserInteractionState.HorizontalScrollOffset = ScrollInfo.nPos;

				RECT ScrollArea;
				GetClientRect(WindowHandle, &ScrollArea);
				ScrollWindowEx(WindowHandle, TestRasterizer->CharacterWidth * ScrollDifference, 0, &ScrollArea, &ScrollArea, NULL, NULL, SW_INVALIDATE);
				UpdateWindow(WindowHandle);
			}
			return 0;
		case WM_KEYDOWN:
			switch (wParam)
			{
			case VK_HOME:
			case VK_END:
				break;
			case VK_PRIOR:
				SendMessage(WindowHandle, WM_VSCROLL, SB_PAGEUP, 0);
				break;
			case VK_NEXT:
				SendMessage(WindowHandle, WM_VSCROLL, SB_PAGEDOWN, 0);
				break;
			case VK_UP:
				SendMessage(WindowHandle, WM_VSCROLL, SB_LINEUP, 0);
				break;
			case VK_DOWN:
				SendMessage(WindowHandle, WM_VSCROLL, SB_LINEDOWN, 0);
				break;
			case VK_LEFT:
				SendMessage(WindowHandle, WM_HSCROLL, SB_LINELEFT, 0);
				break;
			case VK_RIGHT:
				SendMessage(WindowHandle, WM_HSCROLL, SB_LINERIGHT, 0);
				break;
			}
			return 0;
		case WM_MOUSEWHEEL:
		{
			if (LinesToScrollPerThreshold == 0)
				break;

			ScrollInfo.cbSize = sizeof(ScrollInfo);
			ScrollInfo.fMask = SIF_ALL;
			GetScrollInfo(WindowHandle, SB_VERT, &ScrollInfo);

			short AccumulatedDelta = (short)GET_WHEEL_DELTA_WPARAM(wParam);
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
			SetScrollInfo(WindowHandle, SB_VERT, &ScrollInfo, TRUE);
			GetScrollInfo(WindowHandle, SB_VERT, &ScrollInfo);

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
				GetClientRect(WindowHandle, &ScrollArea);
				// Leave the header
				ScrollArea.top += TestRasterizer->CharacterHeight;
				ScrollWindowEx(WindowHandle, 0, TestRasterizer->CharacterHeight * ScrollDifference, &ScrollArea, &ScrollArea, NULL, NULL, SW_INVALIDATE);
				UpdateWindow(WindowHandle);
			}
			return 0;
		}
		case WM_MOUSEHWHEEL: // Horizontal mouse wheel
		{
			ScrollInfo.cbSize = sizeof(ScrollInfo);
			ScrollInfo.fMask = SIF_ALL;
			GetScrollInfo(WindowHandle, SB_HORZ, &ScrollInfo);

			short AccumulatedDelta = (short)GET_WHEEL_DELTA_WPARAM(wParam);
			HorizontalScrollAccumulation += AccumulatedDelta;
			int DeltaPerChar = WHEEL_DELTA / CharsToScrollPerThreshold;
			int DeltasCompleted = HorizontalScrollAccumulation / DeltaPerChar;
			HorizontalScrollAccumulation -= DeltasCompleted * DeltaPerChar;
			int CharactersToScroll = DeltasCompleted;

			ScrollInfo.nPos += CharactersToScroll;
			ScrollInfo.fMask = SIF_POS;
			SetScrollInfo(WindowHandle, SB_HORZ, &ScrollInfo, TRUE);
			GetScrollInfo(WindowHandle, SB_HORZ, &ScrollInfo);

			if (TestUserInteractionState.HorizontalScrollOffset != ScrollInfo.nPos)
			{
				int ScrollDifference = TestUserInteractionState.HorizontalScrollOffset - ScrollInfo.nPos;

				TestUserInteractionState.HorizontalScrollOffset = ScrollInfo.nPos;

				RECT ScrollArea;
				GetClientRect(WindowHandle, &ScrollArea);
				ScrollWindowEx(WindowHandle, TestRasterizer->CharacterWidth * ScrollDifference, 0, &ScrollArea, &ScrollArea, NULL, NULL, SW_INVALIDATE);
				UpdateWindow(WindowHandle);
			}

			// Despite the docs saying you should return 0 if you handle the message, you actually need to return non-zero.
			// If you return zero it will convert the message to a WM_HSCROLL with SB_THUMBTRACK. It will also paint over
			// the scroll bar with an old Windows style until the message is done processing. This will cause a flicker.
			return 1;
		}
		case WM_PAINT:
		{
			PAINTSTRUCT PaintStruct;
			HDC DeviceContext = BeginPaint(WindowHandle, &PaintStruct);

			TestRasterizer->Rasterize(WindowHandle, DeviceContext, PaintStruct.rcPaint.top, PaintStruct.rcPaint.left, PaintStruct.rcPaint.bottom - PaintStruct.rcPaint.top, PaintStruct.rcPaint.right - PaintStruct.rcPaint.left, TestWorkspace.Buffers, TestUserInteractionState);

			EndPaint(WindowHandle, &PaintStruct);
			return 0;
		}
		case WM_DROPFILES:
		{
			UINT FileCount = DragQueryFile((HDROP)wParam, 0xFFFFFFFF, NULL, 0);

			for (UINT i = 0; i < FileCount; i++)
			{
				UINT BufferCharacterCount = DragQueryFile((HDROP)wParam, i, NULL, 0) + 1;
				TCHAR* Buffer = new TCHAR[BufferCharacterCount];
				DragQueryFile((HDROP)wParam, i, Buffer, BufferCharacterCount);

				TestWorkspace = maxHex::CreateWorkspaceFromFile(maxHex::File(Buffer));
				size_t TotalSize = 0;
				for (const std::unique_ptr<maxHex::Buffer>& CurrentBuffer : TestWorkspace.Buffers.BufferList)
				{
					TotalSize += CurrentBuffer->Length;
				}
				size_t LineCount = (TotalSize / 16) + 1;
				InvalidateRect(WindowHandle, NULL, FALSE);
				SetScrollRange(WindowHandle, SB_VERT, 0, static_cast<int>(LineCount) - 1, FALSE);
				SetScrollPos(WindowHandle, SB_VERT, 0, TRUE);
				UpdateWindow(WindowHandle);
				delete[] Buffer;
			}
			DragFinish((HDROP)wParam);
			return 0;
		}
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		}

		return DefWindowProc(WindowHandle, Message, wParam, lParam);
	}

} // anonymous namespace

	int InitializeWindow(HINSTANCE Instance, int ShowCommand) noexcept
	{
		TCHAR AppName[] = TEXT("maxHex");

		WNDCLASSEX WindowClass;
		WindowClass.cbSize = sizeof(WindowClass);
		WindowClass.style = CS_HREDRAW | CS_VREDRAW | CS_CLASSDC;
		WindowClass.lpfnWndProc = WindowProcedure;
		WindowClass.cbClsExtra = 0;
		WindowClass.cbWndExtra = 0;
		WindowClass.hInstance = Instance;
		WindowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		WindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		WindowClass.hbrBackground = (HBRUSH)GetStockObject(COLOR_APPWORKSPACE);
		WindowClass.lpszMenuName = NULL;
		WindowClass.lpszClassName = AppName;
		WindowClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
		if (!RegisterClassEx(&WindowClass))
		{
			return -1;
		}


		HDC DeviceContext = CreateCompatibleDC(NULL);

		// Windows Vista+ comes with Consolas
		TestFont = std::make_unique<maxHex::Font>(maxHex::Font::Create("Consolas", 14));
		if (TestFont->font == NULL)
		{
			TestFont->font = (HFONT)GetStockObject(SYSTEM_FIXED_FONT);
		}
		SelectObject(DeviceContext, TestFont->font);

		TEXTMETRIC TextMetrics;
		GetTextMetrics(DeviceContext, &TextMetrics);
		int CharWidth = TextMetrics.tmAveCharWidth;
		int CharHeight = TextMetrics.tmHeight + TextMetrics.tmExternalLeading;

		int VerticalScrollBarWidth = GetSystemMetrics(SM_CXVSCROLL);
		//int HorizontalScrollBarHeight = GetSystemMetrics(SM_CXVSCROLL);

		int ClientWidth = CharWidth * 78 + VerticalScrollBarWidth;
		int ClientHeight = CharHeight * 40;
		DeleteObject(DeviceContext);


		RECT rcRect;
		rcRect.top = 0;
		rcRect.left = 0;
		rcRect.right = ClientWidth;
		rcRect.bottom = ClientHeight;

		DWORD WindowStyle = WS_OVERLAPPEDWINDOW | WS_VSCROLL;
		DWORD ExtraWindowStyle = WS_EX_ACCEPTFILES;

		AdjustWindowRectEx(&rcRect, WindowStyle, FALSE, ExtraWindowStyle);
		DWORD TotalHeight = 0;
		DWORD TotalWidth = 0;

		rcRect.bottom -= rcRect.top;
		TotalHeight = rcRect.bottom - rcRect.top;

		rcRect.right -= rcRect.left;
		TotalWidth = rcRect.right - rcRect.left;




		HWND WindowHandle = CreateWindowEx(ExtraWindowStyle, AppName, TEXT("maxHex"), WindowStyle, CW_USEDEFAULT, CW_USEDEFAULT, TotalWidth, TotalHeight, NULL, NULL, Instance, NULL);
		ShowWindow(WindowHandle, ShowCommand);
		UpdateWindow(WindowHandle);

		return 0;
	}

} // namespace maxHex