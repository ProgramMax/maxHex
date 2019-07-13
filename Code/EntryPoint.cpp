#include "EntryPoint.hpp"

#pragma region Includes & Forward Declarations

#include "Workspace.hpp"

#define NOMINMAX
#include <Windows.h>
#include <strsafe.h>
#include <algorithm>

LRESULT CALLBACK WindowProcedure(HWND WindowHandle, UINT Message, WPARAM wParam, LPARAM lParam);

#pragma endregion

maxHex::Workspace TestWorkspace;

int WINAPI WinMain(_In_ HINSTANCE Instance, _In_opt_ HINSTANCE /*PreviousInstance*/, _In_ LPSTR /*CommandLine*/, _In_ int ShowCommand)
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

	MSG Message;
	while (GetMessage(&Message, NULL, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return static_cast<int>(Message.wParam);
}

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
	static int DeltaPerLine = 0, AccumulatedDelta = 0;
	ULONG ScrollLines;

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

		int LineCount = (TestWorkspace.BufferLength / 16) + 1;
		SetScrollRange(WindowHandle, SB_VERT, 0, LineCount - 1, FALSE);
		SetScrollPos(WindowHandle, SB_VERT, 0, TRUE);
		//return 0;
		// fall through
	}
	case WM_SETTINGCHANGE:
		SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &ScrollLines, 0);
		if (ScrollLines)
		{
			DeltaPerLine = WHEEL_DELTA / ScrollLines;
		}
		else {
			DeltaPerLine = 0;
		}
		return 0;
	case WM_SIZE:
	{
		int LineCount = (TestWorkspace.BufferLength / 16) + 1;

		ClientHeight = HIWORD(lParam);
		ClientWidth = LOWORD(lParam);

		ScrollInfo.cbSize = sizeof(ScrollInfo);
		ScrollInfo.fMask = SIF_RANGE | SIF_PAGE;
		ScrollInfo.nMin = 0;
		ScrollInfo.nMax = LineCount - 1;
		ScrollInfo.nPage = ClientHeight / CharHeight;
		SetScrollInfo(WindowHandle, SB_VERT, &ScrollInfo, TRUE);

		ScrollInfo.cbSize = sizeof(ScrollInfo);
		ScrollInfo.fMask = SIF_RANGE | SIF_PAGE;
		ScrollInfo.nMin = 0;
		ScrollInfo.nMax = 2 + ClientWidth / CharWidth;
		ScrollInfo.nPage = ClientWidth / CharWidth;
		SetScrollInfo(WindowHandle, SB_HORZ, &ScrollInfo, TRUE);
		return 0;
	}
	case WM_VSCROLL:
	{
		ScrollInfo.cbSize = sizeof(ScrollInfo);
		ScrollInfo.fMask = SIF_ALL;
		GetScrollInfo(WindowHandle, SB_VERT, &ScrollInfo);

		int OldVerticalScrollPosition = VerticalScrollPosition;
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


		int LineCount = (TestWorkspace.BufferLength / 16);
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
		case SB_THUMBPOSITION:
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
			ScrollWindowEx(WindowHandle, CharWidth* (HorizontalScrollPosition - ScrollInfo.nPos), 0, NULL, NULL, NULL, NULL, SW_INVALIDATE);
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
		if (DeltaPerLine == 0)
			break;
		AccumulatedDelta += (short)HIWORD(wParam);
		while (AccumulatedDelta >= DeltaPerLine)
		{
			SendMessage(WindowHandle, WM_VSCROLL, SB_LINEUP, 0);
			AccumulatedDelta -= DeltaPerLine;
		}
		while (AccumulatedDelta <= -DeltaPerLine)
		{
			SendMessage(WindowHandle, WM_VSCROLL, SB_LINEDOWN, 0);
			AccumulatedDelta += DeltaPerLine;
		}
		return 0;
	case WM_PAINT:
	{
		PAINTSTRUCT PaintStruct;
		HDC DeviceContext = BeginPaint(WindowHandle, &PaintStruct);

		HFONT NewFont = (HFONT)GetStockObject(SYSTEM_FIXED_FONT);
		HFONT OldFont = (HFONT)SelectObject(DeviceContext, NewFont);

		int LineCount = (TestWorkspace.BufferLength / 16) + 1;
		
		ScrollInfo.cbSize = sizeof(ScrollInfo);
		ScrollInfo.fMask = SIF_POS;
		GetScrollInfo(WindowHandle, SB_VERT, &ScrollInfo);
		VerticalScrollPosition = ScrollInfo.nPos;
		GetScrollInfo(WindowHandle, SB_HORZ, &ScrollInfo);
		HorizontalScrollPosition = ScrollInfo.nPos;

		TextOut(DeviceContext, CharWidth * 12, 0, "x0 x1 x2 x3 x4 x5 x6 x7 x8 x9 xA xB xC xD xE xF", 47);

		TextOut(DeviceContext, CharWidth* (47 + 12 + 3 +  0), 0, "0", 1);
		TextOut(DeviceContext, CharWidth* (47 + 12 + 3 +  1), 0, "1", 1);
		TextOut(DeviceContext, CharWidth* (47 + 12 + 3 +  2), 0, "2", 1);
		TextOut(DeviceContext, CharWidth* (47 + 12 + 3 +  3), 0, "3", 1);
		TextOut(DeviceContext, CharWidth* (47 + 12 + 3 +  4), 0, "4", 1);
		TextOut(DeviceContext, CharWidth* (47 + 12 + 3 +  5), 0, "5", 1);
		TextOut(DeviceContext, CharWidth* (47 + 12 + 3 +  6), 0, "6", 1);
		TextOut(DeviceContext, CharWidth* (47 + 12 + 3 +  7), 0, "7", 1);
		TextOut(DeviceContext, CharWidth* (47 + 12 + 3 +  8), 0, "8", 1);
		TextOut(DeviceContext, CharWidth* (47 + 12 + 3 +  9), 0, "9", 1);
		TextOut(DeviceContext, CharWidth* (47 + 12 + 3 + 10), 0, "A", 1);
		TextOut(DeviceContext, CharWidth* (47 + 12 + 3 + 11), 0, "B", 1);
		TextOut(DeviceContext, CharWidth* (47 + 12 + 3 + 12), 0, "C", 1);
		TextOut(DeviceContext, CharWidth* (47 + 12 + 3 + 13), 0, "D", 1);
		TextOut(DeviceContext, CharWidth* (47 + 12 + 3 + 14), 0, "E", 1);
		TextOut(DeviceContext, CharWidth* (47 + 12 + 3 + 15), 0, "F", 1);

		for (int i = VerticalScrollPosition; i < LineCount; i++)
		{
			int Height = CharHeight * (i - VerticalScrollPosition + 1);
			SetTextAlign(DeviceContext, TA_LEFT | TA_TOP);

			// Display the address
			const size_t BufferSizeInCharacters = 10;
			const size_t BufferSizeInBytes = BufferSizeInCharacters * sizeof(TCHAR);
			TCHAR Buffer[BufferSizeInCharacters];
			HRESULT Result = StringCbPrintf(Buffer, BufferSizeInBytes, TEXT("%08dx"), i);
			TextOut(DeviceContext, 0, Height, Buffer, BufferSizeInCharacters - 1);


			int BytesOnThisLine = 16;
			if (i == LineCount - 1)
			{
				BytesOnThisLine = TestWorkspace.BufferLength % 16;
			}

			// TODO: Display the hex values
			//TextOut(DeviceContext, CharWidth * 12, Height, "00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00", 47);


			// Display the hax & ASCII
			const char* HexString = "0123456789ABCDEF";
			for (int j = 0; j < BytesOnThisLine; j++)
			{
				const char CurrentChar = TestWorkspace.Buffer[(i * 16) + j];
				int HighNibble = CurrentChar >> 4;
				int LowNibble = CurrentChar & 0x0f;
				TextOut(DeviceContext, CharWidth * (12 + (3 * j) + 0), Height, &HexString[HighNibble], 1);
				TextOut(DeviceContext, CharWidth * (12 + (3 * j) + 1), Height, &HexString[LowNibble], 1);
				TextOut(DeviceContext, CharWidth * (47 + 12 + 3 + j), Height, &CurrentChar, 1);
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

			TestWorkspace = maxHex::CreateWorkspaceFromFile(Buffer);
			int LineCount = (TestWorkspace.BufferLength / 16) + 1;
			SetScrollRange(WindowHandle, SB_VERT, 0, LineCount - 1, FALSE);
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

namespace maxHex
{
} // namespace maxHex