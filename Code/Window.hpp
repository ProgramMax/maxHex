// Copyright 2019, The maxHex Contributors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MAXHEX_WINDOW_HPP
#define MAXHEX_WINDOW_HPP

#include <maxGUI/Form.hpp>

#ifndef WIN32_LEAN_AND_MEAN
#define WIn32_LEAN_AND_MEAN
#endif
#include <Windows.h>

#include "Font.hpp"
#include "Rasterizer.hpp"

namespace maxHex
{

	extern std::unique_ptr<maxHex::Font> TestFont; // TODO: Make this not a global
	extern std::unique_ptr<maxHex::Rasterizer> TestRasterizer; // TODO: Make this not a global

	class MainForm {
	public:

		void OnCreated(maxGUI::FormConcept* form) noexcept;
		void OnClosed(maxGUI::FormConcept* form) noexcept;
		void OnResized(maxGUI::FormConcept* form, int new_height, int new_width) noexcept;

		LRESULT OnWindowMessage(maxGUI::FormConcept* form, UINT message, WPARAM wparam, LPARAM lparam) noexcept;

	};

} // namespace maxHex

#endif // #ifndef MAXHEX_WINDOW_HPP