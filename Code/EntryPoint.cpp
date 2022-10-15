// Copyright 2019, The maxHex Contributors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <memory>
#include <utility>

#include <maxGUI/EntryPoint.hpp>
#include <maxGUI/Form.hpp>

#include "Font.hpp"
#include "Window.hpp"

namespace {

	class Rect {
	public:

		Rect(int width, int height) noexcept
			: width_(std::move(width))
			, height_(std::move(height))
		{}

		int width_;
		int height_;

	};

	Rect InitializeAndGetWindowSize() noexcept {
		HDC DeviceContext = CreateCompatibleDC(NULL);

		// Windows Vista+ comes with Consolas
		maxHex::TestFont = std::make_unique<maxHex::Font>(maxHex::Font::Create("Consolas", 14));
		if (maxHex::TestFont->font == NULL)
		{
			maxHex::TestFont->font = (HFONT)GetStockObject(SYSTEM_FIXED_FONT);
		}
		SelectObject(DeviceContext, maxHex::TestFont->font);

		TEXTMETRIC TextMetrics;
		GetTextMetrics(DeviceContext, &TextMetrics);
		int CharWidth = TextMetrics.tmAveCharWidth;
		int CharHeight = TextMetrics.tmHeight + TextMetrics.tmExternalLeading;

		int VerticalScrollBarWidth = GetSystemMetrics(SM_CXVSCROLL);
		//int HorizontalScrollBarHeight = GetSystemMetrics(SM_CXVSCROLL);

		int ClientWidth = CharWidth * 78 + VerticalScrollBarWidth;
		int ClientHeight = CharHeight * 40;
		DeleteObject(DeviceContext);


		return Rect(ClientWidth, ClientHeight);

		// TODO: The window needs these styles.
		//DWORD WindowStyle = WS_OVERLAPPEDWINDOW | WS_VSCROLL;
		//DWORD ExtraWindowStyle = WS_EX_ACCEPTFILES;
	}

} // anonymous namespace

int maxGUIEntryPoint(maxGUI::FormContainer form_container) noexcept {
	Rect FormRectangle = InitializeAndGetWindowSize();

	maxGUI::FormFactory main_form_factory(maxGUI::GetDefaultFormAllocator<maxHex::MainForm>());
	if (!form_container.CreateForm(main_form_factory, FormRectangle.height_, FormRectangle.width_, "maxHex")) {
		return -1;
	}

	return maxGUI::MessagePump(form_container);
}