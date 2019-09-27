// Copyright 2019, The maxHex Contributors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Window.hpp"
#include "Buffer.hpp"
#include "Workspace.hpp"

#include <strsafe.h>
#include <algorithm>

namespace maxHex
{
namespace
{

	maxHex::Workspace TestWorkspace;

	LRESULT CALLBACK WindowProcedure(HWND WindowHandle, UINT Message, WPARAM wParam, LPARAM lParam)
	{
		static int CharWidth;
		static int CharHeight;
		static int ClientHeight;
		static int ClientWidth;
		static int VerticalScrollPosition = 0;
		static int HorizontalScrollPosition = 0;
		static int MaxWidth;
		SCROLLINFO ScrollInfo;
		static ULONG LinesToScrollPerThreshold = 0;
		static ULONG CharsToScrollPerThreahold = 0;

		switch (Message)
		{
		case WM_CREATE:
		{
			HDC DeviceContext = GetDC(WindowHandle);
			TEXTMETRIC TextMetrics;
			GetTextMetrics(DeviceContext, &TextMetrics);
			CharWidth = TextMetrics.tmAveCharWidth;
			CharWidth++; // Ave seems not great
			CharHeight = TextMetrics.tmHeight + TextMetrics.tmExternalLeading;
			ReleaseDC(WindowHandle, DeviceContext);

			MaxWidth = 72 * CharWidth; // TODO: Find real value

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
			SystemParametersInfo(SPI_GETWHEELSCROLLCHARS, 0, &CharsToScrollPerThreahold, 0);
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
			ScrollInfo.nPage = ClientHeight / CharHeight;
			SetScrollInfo(WindowHandle, SB_VERT, &ScrollInfo, TRUE);

			ScrollInfo.cbSize = sizeof(ScrollInfo);
			ScrollInfo.fMask = SIF_RANGE | SIF_PAGE;
			ScrollInfo.nMin = 0;
			ScrollInfo.nMax = 78;
			ScrollInfo.nPage = ClientWidth / CharWidth;
			SetScrollInfo(WindowHandle, SB_HORZ, &ScrollInfo, TRUE);
			return 0;
		}
		case WM_VSCROLL:
		{
			ScrollInfo.cbSize = sizeof(ScrollInfo);
			ScrollInfo.fMask = SIF_ALL;
			GetScrollInfo(WindowHandle, SB_VERT, &ScrollInfo);
			VerticalScrollPosition = ScrollInfo.nPos;

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
			if (VerticalScrollPosition != ScrollInfo.nPos)
			{
				RECT ScrollArea;
				GetClientRect(WindowHandle, &ScrollArea);
				// Leave the header
				ScrollArea.top += CharHeight;
				ScrollWindowEx(WindowHandle, 0, CharHeight * (VerticalScrollPosition - ScrollInfo.nPos), &ScrollArea, &ScrollArea, NULL, NULL, SW_INVALIDATE);
				UpdateWindow(WindowHandle);
			}
			return 0;
		}
		case WM_HSCROLL:
			ScrollInfo.cbSize = sizeof(ScrollInfo);
			ScrollInfo.fMask = SIF_ALL;
			GetScrollInfo(WindowHandle, SB_HORZ, &ScrollInfo);
			HorizontalScrollPosition = ScrollInfo.nPos;

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

			if (HorizontalScrollPosition != ScrollInfo.nPos)
			{
				RECT ScrollArea;
				GetClientRect(WindowHandle, &ScrollArea);
				ScrollWindowEx(WindowHandle, CharWidth * (HorizontalScrollPosition - ScrollInfo.nPos), 0, &ScrollArea, &ScrollArea, NULL, NULL, SW_INVALIDATE);
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
			int LinesToScroll = 0;
			if (LinesToScrollPerThreshold == WHEEL_PAGESCROLL)
			{
				if (AccumulatedDelta < 0)
				{
					LinesToScroll = -static_cast<int>(ScrollInfo.nPage);
				}
				else {
					LinesToScroll = ScrollInfo.nPage;
				}
			}
			else {
				LinesToScroll = AccumulatedDelta / WHEEL_DELTA * LinesToScrollPerThreshold;
			}

			VerticalScrollPosition = ScrollInfo.nPos;


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
			if (VerticalScrollPosition != ScrollInfo.nPos)
			{
				RECT ScrollArea;
				GetClientRect(WindowHandle, &ScrollArea);
				// Leave the header
				ScrollArea.top += CharHeight;
				ScrollWindowEx(WindowHandle, 0, CharHeight * (VerticalScrollPosition - ScrollInfo.nPos), &ScrollArea, &ScrollArea, NULL, NULL, SW_INVALIDATE);
				UpdateWindow(WindowHandle);
			}
			return 0;
		}
		case WM_MOUSEHWHEEL: // Horizontal mouse wheel
		{
			ScrollInfo.cbSize = sizeof(ScrollInfo);
			ScrollInfo.fMask = SIF_ALL;
			GetScrollInfo(WindowHandle, SB_HORZ, &ScrollInfo);
			HorizontalScrollPosition = ScrollInfo.nPos;

			short AccumulatedDelta = (short)GET_WHEEL_DELTA_WPARAM(wParam);
			int CharactersToScroll = AccumulatedDelta / WHEEL_DELTA * CharsToScrollPerThreahold;
			ScrollInfo.nPos += CharactersToScroll;

			ScrollInfo.fMask = SIF_POS;
			SetScrollInfo(WindowHandle, SB_HORZ, &ScrollInfo, TRUE);
			GetScrollInfo(WindowHandle, SB_HORZ, &ScrollInfo);

			if (HorizontalScrollPosition != ScrollInfo.nPos)
			{
				RECT ScrollArea;
				GetClientRect(WindowHandle, &ScrollArea);
				ScrollWindowEx(WindowHandle, CharWidth * (HorizontalScrollPosition - ScrollInfo.nPos), 0, &ScrollArea, &ScrollArea, NULL, NULL, SW_INVALIDATE);
				HorizontalScrollPosition = ScrollInfo.nPos;
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

			FillRect(DeviceContext, &PaintStruct.rcPaint, (HBRUSH)GetStockObject(COLOR_APPWORKSPACE));

			HFONT NewFont = (HFONT)GetStockObject(SYSTEM_FIXED_FONT);
			HFONT OldFont = (HFONT)SelectObject(DeviceContext, NewFont);

			size_t TotalSize = 0;
			for (const std::unique_ptr<maxHex::Buffer>& CurrentBuffer : TestWorkspace.Buffers.BufferList)
			{
				TotalSize += CurrentBuffer->Length;
			}
			size_t LineCount = 0;
			if (TotalSize != 0) {
				LineCount = (TotalSize / 16) + 1;
			}
			ScrollInfo.cbSize = sizeof(ScrollInfo);
			ScrollInfo.fMask = SIF_POS;
			GetScrollInfo(WindowHandle, SB_VERT, &ScrollInfo);
			VerticalScrollPosition = ScrollInfo.nPos;
			GetScrollInfo(WindowHandle, SB_HORZ, &ScrollInfo);
			HorizontalScrollPosition = ScrollInfo.nPos;

			TextOut(DeviceContext, CharWidth * (12 + (3 * 0)) - (HorizontalScrollPosition * CharWidth), 0, TEXT("x0"), 2);
			TextOut(DeviceContext, CharWidth * (12 + (3 * 1)) - (HorizontalScrollPosition * CharWidth), 0, TEXT("x1"), 2);
			TextOut(DeviceContext, CharWidth * (12 + (3 * 2)) - (HorizontalScrollPosition * CharWidth), 0, TEXT("x2"), 2);
			TextOut(DeviceContext, CharWidth * (12 + (3 * 3)) - (HorizontalScrollPosition * CharWidth), 0, TEXT("x3"), 2);
			TextOut(DeviceContext, CharWidth * (12 + (3 * 4)) - (HorizontalScrollPosition * CharWidth), 0, TEXT("x4"), 2);
			TextOut(DeviceContext, CharWidth * (12 + (3 * 5)) - (HorizontalScrollPosition * CharWidth), 0, TEXT("x5"), 2);
			TextOut(DeviceContext, CharWidth * (12 + (3 * 6)) - (HorizontalScrollPosition * CharWidth), 0, TEXT("x6"), 2);
			TextOut(DeviceContext, CharWidth * (12 + (3 * 7)) - (HorizontalScrollPosition * CharWidth), 0, TEXT("x7"), 2);
			TextOut(DeviceContext, CharWidth * (12 + (3 * 8)) - (HorizontalScrollPosition * CharWidth), 0, TEXT("x8"), 2);
			TextOut(DeviceContext, CharWidth * (12 + (3 * 9)) - (HorizontalScrollPosition * CharWidth), 0, TEXT("x9"), 2);
			TextOut(DeviceContext, CharWidth * (12 + (3 * 10)) - (HorizontalScrollPosition * CharWidth), 0, TEXT("xA"), 2);
			TextOut(DeviceContext, CharWidth * (12 + (3 * 11)) - (HorizontalScrollPosition * CharWidth), 0, TEXT("xB"), 2);
			TextOut(DeviceContext, CharWidth * (12 + (3 * 12)) - (HorizontalScrollPosition * CharWidth), 0, TEXT("xC"), 2);
			TextOut(DeviceContext, CharWidth * (12 + (3 * 13)) - (HorizontalScrollPosition * CharWidth), 0, TEXT("xD"), 2);
			TextOut(DeviceContext, CharWidth * (12 + (3 * 14)) - (HorizontalScrollPosition * CharWidth), 0, TEXT("xE"), 2);
			TextOut(DeviceContext, CharWidth * (12 + (3 * 15)) - (HorizontalScrollPosition * CharWidth), 0, TEXT("xF"), 2);

			TextOut(DeviceContext, CharWidth * (47 + 12 + 3 + 0) - (HorizontalScrollPosition * CharWidth), 0, TEXT("0"), 1);
			TextOut(DeviceContext, CharWidth * (47 + 12 + 3 + 1) - (HorizontalScrollPosition * CharWidth), 0, TEXT("1"), 1);
			TextOut(DeviceContext, CharWidth * (47 + 12 + 3 + 2) - (HorizontalScrollPosition * CharWidth), 0, TEXT("2"), 1);
			TextOut(DeviceContext, CharWidth * (47 + 12 + 3 + 3) - (HorizontalScrollPosition * CharWidth), 0, TEXT("3"), 1);
			TextOut(DeviceContext, CharWidth * (47 + 12 + 3 + 4) - (HorizontalScrollPosition * CharWidth), 0, TEXT("4"), 1);
			TextOut(DeviceContext, CharWidth * (47 + 12 + 3 + 5) - (HorizontalScrollPosition * CharWidth), 0, TEXT("5"), 1);
			TextOut(DeviceContext, CharWidth * (47 + 12 + 3 + 6) - (HorizontalScrollPosition * CharWidth), 0, TEXT("6"), 1);
			TextOut(DeviceContext, CharWidth * (47 + 12 + 3 + 7) - (HorizontalScrollPosition * CharWidth), 0, TEXT("7"), 1);
			TextOut(DeviceContext, CharWidth * (47 + 12 + 3 + 8) - (HorizontalScrollPosition * CharWidth), 0, TEXT("8"), 1);
			TextOut(DeviceContext, CharWidth * (47 + 12 + 3 + 9) - (HorizontalScrollPosition * CharWidth), 0, TEXT("9"), 1);
			TextOut(DeviceContext, CharWidth * (47 + 12 + 3 + 10) - (HorizontalScrollPosition * CharWidth), 0, TEXT("A"), 1);
			TextOut(DeviceContext, CharWidth * (47 + 12 + 3 + 11) - (HorizontalScrollPosition * CharWidth), 0, TEXT("B"), 1);
			TextOut(DeviceContext, CharWidth * (47 + 12 + 3 + 12) - (HorizontalScrollPosition * CharWidth), 0, TEXT("C"), 1);
			TextOut(DeviceContext, CharWidth * (47 + 12 + 3 + 13) - (HorizontalScrollPosition * CharWidth), 0, TEXT("D"), 1);
			TextOut(DeviceContext, CharWidth * (47 + 12 + 3 + 14) - (HorizontalScrollPosition * CharWidth), 0, TEXT("E"), 1);
			TextOut(DeviceContext, CharWidth * (47 + 12 + 3 + 15) - (HorizontalScrollPosition * CharWidth), 0, TEXT("F"), 1);

			size_t AccumulatedBufferSize = 0;
			size_t CurrentBuffer = 0;
			for (int i = VerticalScrollPosition; i < LineCount; i++)
			{
				int Height = CharHeight * (i - VerticalScrollPosition + 1);
				SetTextAlign(DeviceContext, TA_LEFT | TA_TOP);

				// Display the address
				const size_t BufferSizeInCharacters = 10;
				const size_t BufferSizeInBytes = BufferSizeInCharacters * sizeof(TCHAR);
				TCHAR Buffer[BufferSizeInCharacters];
				StringCbPrintf(Buffer, BufferSizeInBytes, TEXT("%08dx"), i);
				TextOut(DeviceContext, -(HorizontalScrollPosition * CharWidth), Height, Buffer, BufferSizeInCharacters - 1);

				int BytesOnThisLine = 16;
				if (i == LineCount - 1)
				{
					BytesOnThisLine = TotalSize % 16;
				}

				// Display the hex & ASCII
				const char* HexString = "0123456789ABCDEF";
				for (int j = 0; j < BytesOnThisLine; j++)
				{
					size_t BufferIndex = static_cast<size_t>(i) * 16 + j;
					while (AccumulatedBufferSize + TestWorkspace.Buffers.BufferList[CurrentBuffer]->Length <= BufferIndex)
					{
						AccumulatedBufferSize += TestWorkspace.Buffers.BufferList[CurrentBuffer]->Length;
						CurrentBuffer++;
					}
					BufferIndex -= AccumulatedBufferSize;

					const unsigned char CurrentChar = TestWorkspace.Buffers.BufferList[CurrentBuffer]->Storage[BufferIndex];
					size_t HighNibble = CurrentChar >> 4;
					size_t  LowNibble = CurrentChar & 0x0f;
					TextOutA(DeviceContext, (CharWidth * (12 + (3 * j) + 0)) - (HorizontalScrollPosition * CharWidth), Height, &HexString[HighNibble], 1);
					TextOutA(DeviceContext, (CharWidth * (12 + (3 * j) + 1)) - (HorizontalScrollPosition * CharWidth), Height, &HexString[LowNibble], 1);
					TextOutA(DeviceContext, (CharWidth * (47 + 12 + 3 + j)) - (HorizontalScrollPosition * CharWidth), Height, reinterpret_cast<const char*>(&CurrentChar), 1);
				}
			}

			SelectObject(DeviceContext, OldFont);

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
		WindowClass.style = CS_HREDRAW | CS_VREDRAW;
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
		HFONT NewFont = (HFONT)GetStockObject(SYSTEM_FIXED_FONT);
		HFONT OldFont = (HFONT)SelectObject(DeviceContext, NewFont);

		TEXTMETRIC TextMetrics;
		GetTextMetrics(DeviceContext, &TextMetrics);
		int CharWidth = TextMetrics.tmAveCharWidth;
		CharWidth++; // Ave seems not great
		int CharHeight = TextMetrics.tmHeight + TextMetrics.tmExternalLeading;

		int ClientWidth = CharWidth * 72;
		int ClientHeight = CharHeight * 40;
		SelectObject(DeviceContext, OldFont);
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