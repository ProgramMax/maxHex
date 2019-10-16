#include "Font.hpp"

namespace maxHex
{

	Font Font::Create(const std::string& Name, long Size) noexcept
	{
		// Windows Vista+ comes with Consolas
		LOGFONT lf = { 0 };
		for (size_t i = 0; i < Name.size(); i++) {
			lf.lfFaceName[i] = Name[i];
		}
		lf.lfHeight = Size;
		HFONT NewFont = CreateFontIndirect(&lf);
		return Font(NewFont);
	}

	#if defined(MAX_PLATFORM_WINDOWS)
	Font::Font(HFONT font) noexcept
		: font(font)
	{
	}
	#endif

	Font::Font(Font&& rhs) noexcept
		: font(rhs.font)
	{
		rhs.font = NULL;
	}

	Font::~Font() noexcept
	{
		if (font != NULL)
		{
			DeleteObject(font);
		}
	}

	Font& Font::operator =(Font&& rhs) noexcept
	{
		font = rhs.font;
		rhs.font = NULL;

		return *this;
	}

} // namespace maxHex