#include "Rasterizer.hpp"

#include <strsafe.h>
#include <algorithm>

namespace maxHex
{

	Rasterizer Rasterizer::Create(HDC DeviceContext, const maxHex::Font& font) noexcept
	{
		SelectObject(DeviceContext, font.font);

		TEXTMETRIC TextMetrics;
		GetTextMetrics(DeviceContext, &TextMetrics);
		int CharacterHeight = TextMetrics.tmHeight + TextMetrics.tmExternalLeading;
		int CharacterWidth = TextMetrics.tmAveCharWidth;

		return Rasterizer{CharacterHeight, CharacterWidth};
	}

	Rasterizer::Rasterizer(int CharacterHeight, int CharacterWidth) noexcept
		: CharacterHeight(CharacterHeight)
		, CharacterWidth(CharacterWidth)
	{
	}

	#if defined(MAX_PLATFORM_WINDOWS)
	void Rasterizer::Rasterize(HWND WindowHandle, HDC DeviceContext, int VirtualTop, int VirtualLeft, int Height, int Width, const BufferChain& Buffers, const UserInteractionState& InteractionState) noexcept
	{
		RECT BlankRect;
		BlankRect.top = VirtualTop;
		BlankRect.left = VirtualLeft;
		BlankRect.bottom = VirtualTop + Height;
		BlankRect.right = VirtualLeft + Width;
		FillRect(DeviceContext, &BlankRect, (HBRUSH)GetStockObject(COLOR_APPWORKSPACE));

		size_t TotalSize = 0;
		for (const std::unique_ptr<maxHex::Buffer>& CurrentBuffer : Buffers.BufferList)
		{
			TotalSize += CurrentBuffer->Length;
		}
		size_t LineCount = 0;
		if (TotalSize != 0) {
			LineCount = (TotalSize / 16) + 1;
		}
		/*
		SCROLLINFO ScrollInfo;
		ScrollInfo.cbSize = sizeof(ScrollInfo);
		ScrollInfo.fMask = SIF_POS;
		GetScrollInfo(WindowHandle, SB_VERT, &ScrollInfo);
		InteractionState.VerticalScrollOffset = ScrollInfo.nPos;
		GetScrollInfo(WindowHandle, SB_HORZ, &ScrollInfo);
		InteractionState.HorizontalScrollOffset = ScrollInfo.nPos;
		*/
		int HeaderHeight = CharacterHeight;
		if (VirtualTop < HeaderHeight)
		{
			TextOut(DeviceContext, CharacterWidth * (12 + (3 * 0)) - (InteractionState.HorizontalScrollOffset * CharacterWidth), 0, TEXT("x0"), 2);
			TextOut(DeviceContext, CharacterWidth * (12 + (3 * 1)) - (InteractionState.HorizontalScrollOffset * CharacterWidth), 0, TEXT("x1"), 2);
			TextOut(DeviceContext, CharacterWidth * (12 + (3 * 2)) - (InteractionState.HorizontalScrollOffset * CharacterWidth), 0, TEXT("x2"), 2);
			TextOut(DeviceContext, CharacterWidth * (12 + (3 * 3)) - (InteractionState.HorizontalScrollOffset * CharacterWidth), 0, TEXT("x3"), 2);
			TextOut(DeviceContext, CharacterWidth * (12 + (3 * 4)) - (InteractionState.HorizontalScrollOffset * CharacterWidth), 0, TEXT("x4"), 2);
			TextOut(DeviceContext, CharacterWidth * (12 + (3 * 5)) - (InteractionState.HorizontalScrollOffset * CharacterWidth), 0, TEXT("x5"), 2);
			TextOut(DeviceContext, CharacterWidth * (12 + (3 * 6)) - (InteractionState.HorizontalScrollOffset * CharacterWidth), 0, TEXT("x6"), 2);
			TextOut(DeviceContext, CharacterWidth * (12 + (3 * 7)) - (InteractionState.HorizontalScrollOffset * CharacterWidth), 0, TEXT("x7"), 2);
			TextOut(DeviceContext, CharacterWidth * (12 + (3 * 8)) - (InteractionState.HorizontalScrollOffset * CharacterWidth), 0, TEXT("x8"), 2);
			TextOut(DeviceContext, CharacterWidth * (12 + (3 * 9)) - (InteractionState.HorizontalScrollOffset * CharacterWidth), 0, TEXT("x9"), 2);
			TextOut(DeviceContext, CharacterWidth * (12 + (3 * 10)) - (InteractionState.HorizontalScrollOffset * CharacterWidth), 0, TEXT("xA"), 2);
			TextOut(DeviceContext, CharacterWidth * (12 + (3 * 11)) - (InteractionState.HorizontalScrollOffset * CharacterWidth), 0, TEXT("xB"), 2);
			TextOut(DeviceContext, CharacterWidth * (12 + (3 * 12)) - (InteractionState.HorizontalScrollOffset * CharacterWidth), 0, TEXT("xC"), 2);
			TextOut(DeviceContext, CharacterWidth * (12 + (3 * 13)) - (InteractionState.HorizontalScrollOffset * CharacterWidth), 0, TEXT("xD"), 2);
			TextOut(DeviceContext, CharacterWidth * (12 + (3 * 14)) - (InteractionState.HorizontalScrollOffset * CharacterWidth), 0, TEXT("xE"), 2);
			TextOut(DeviceContext, CharacterWidth * (12 + (3 * 15)) - (InteractionState.HorizontalScrollOffset * CharacterWidth), 0, TEXT("xF"), 2);

			TextOut(DeviceContext, CharacterWidth * (47 + 12 + 3 + 0) - (InteractionState.HorizontalScrollOffset * CharacterWidth), 0, TEXT("0"), 1);
			TextOut(DeviceContext, CharacterWidth * (47 + 12 + 3 + 1) - (InteractionState.HorizontalScrollOffset * CharacterWidth), 0, TEXT("1"), 1);
			TextOut(DeviceContext, CharacterWidth * (47 + 12 + 3 + 2) - (InteractionState.HorizontalScrollOffset * CharacterWidth), 0, TEXT("2"), 1);
			TextOut(DeviceContext, CharacterWidth * (47 + 12 + 3 + 3) - (InteractionState.HorizontalScrollOffset * CharacterWidth), 0, TEXT("3"), 1);
			TextOut(DeviceContext, CharacterWidth * (47 + 12 + 3 + 4) - (InteractionState.HorizontalScrollOffset * CharacterWidth), 0, TEXT("4"), 1);
			TextOut(DeviceContext, CharacterWidth * (47 + 12 + 3 + 5) - (InteractionState.HorizontalScrollOffset * CharacterWidth), 0, TEXT("5"), 1);
			TextOut(DeviceContext, CharacterWidth * (47 + 12 + 3 + 6) - (InteractionState.HorizontalScrollOffset * CharacterWidth), 0, TEXT("6"), 1);
			TextOut(DeviceContext, CharacterWidth * (47 + 12 + 3 + 7) - (InteractionState.HorizontalScrollOffset * CharacterWidth), 0, TEXT("7"), 1);
			TextOut(DeviceContext, CharacterWidth * (47 + 12 + 3 + 8) - (InteractionState.HorizontalScrollOffset * CharacterWidth), 0, TEXT("8"), 1);
			TextOut(DeviceContext, CharacterWidth * (47 + 12 + 3 + 9) - (InteractionState.HorizontalScrollOffset * CharacterWidth), 0, TEXT("9"), 1);
			TextOut(DeviceContext, CharacterWidth * (47 + 12 + 3 + 10) - (InteractionState.HorizontalScrollOffset * CharacterWidth), 0, TEXT("A"), 1);
			TextOut(DeviceContext, CharacterWidth * (47 + 12 + 3 + 11) - (InteractionState.HorizontalScrollOffset * CharacterWidth), 0, TEXT("B"), 1);
			TextOut(DeviceContext, CharacterWidth * (47 + 12 + 3 + 12) - (InteractionState.HorizontalScrollOffset * CharacterWidth), 0, TEXT("C"), 1);
			TextOut(DeviceContext, CharacterWidth * (47 + 12 + 3 + 13) - (InteractionState.HorizontalScrollOffset * CharacterWidth), 0, TEXT("D"), 1);
			TextOut(DeviceContext, CharacterWidth * (47 + 12 + 3 + 14) - (InteractionState.HorizontalScrollOffset * CharacterWidth), 0, TEXT("E"), 1);
			TextOut(DeviceContext, CharacterWidth * (47 + 12 + 3 + 15) - (InteractionState.HorizontalScrollOffset * CharacterWidth), 0, TEXT("F"), 1);
		}

		size_t AccumulatedBufferSize = 0;
		size_t CurrentBuffer = 0;

		int ContentTop = VirtualTop;
		int ContentHeight = Height;
		if (ContentTop < HeaderHeight)
		{
			ContentHeight -= (HeaderHeight - ContentTop);
			ContentTop = HeaderHeight;
		}
		int FirstVisibleLine = std::max(((ContentTop - HeaderHeight) / CharacterHeight) + InteractionState.VerticalScrollOffset, 0);
		int LastVisibleLine = std::min(FirstVisibleLine + (ContentHeight / CharacterHeight), static_cast<int>(LineCount));
		for (int i = FirstVisibleLine; i < LastVisibleLine; i++)
		{
			int Height = CharacterHeight * (i - InteractionState.VerticalScrollOffset + 1);
			SetTextAlign(DeviceContext, TA_LEFT | TA_TOP);

			// Display the address
			const size_t BufferSizeInCharacters = 10;
			const size_t BufferSizeInBytes = BufferSizeInCharacters * sizeof(TCHAR);
			TCHAR Buffer[BufferSizeInCharacters];
			StringCbPrintf(Buffer, BufferSizeInBytes, TEXT("%08dx"), i);
			TextOut(DeviceContext, -(InteractionState.HorizontalScrollOffset * CharacterWidth), Height, Buffer, BufferSizeInCharacters - 1);

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
				while (AccumulatedBufferSize + Buffers.BufferList[CurrentBuffer]->Length <= BufferIndex)
				{
					AccumulatedBufferSize += Buffers.BufferList[CurrentBuffer]->Length;
					CurrentBuffer++;
				}
				BufferIndex -= AccumulatedBufferSize;

				const unsigned char CurrentChar = Buffers.BufferList[CurrentBuffer]->Storage[BufferIndex];
				size_t HighNibble = CurrentChar >> 4;
				size_t  LowNibble = CurrentChar & 0x0f;
				TextOutA(DeviceContext, (CharacterWidth * (12 + (3 * j) + 0)) - (InteractionState.HorizontalScrollOffset * CharacterWidth), Height, &HexString[HighNibble], 1);
				TextOutA(DeviceContext, (CharacterWidth * (12 + (3 * j) + 1)) - (InteractionState.HorizontalScrollOffset * CharacterWidth), Height, &HexString[LowNibble], 1);
				TextOutA(DeviceContext, (CharacterWidth * (47 + 12 + 3 + j)) - (InteractionState.HorizontalScrollOffset * CharacterWidth), Height, reinterpret_cast<const char*>(&CurrentChar), 1);
			}
		}
	}
	#endif

} // namespace maxHex