#ifndef MAXHEX_FONT_HPP
#define MAXHEX_FONT_HPP

#include <max/Compiling/Configuration.hpp>

#include <string>

#if defined(MAX_PLATFORM_WINDOWS)
#include <Windows.h>
#endif

namespace maxHex
{

	class Font
	{
	public:

		static Font Create(const std::string& Name, long Size) noexcept;

		Font() = delete;
		#if defined(MAX_PLATFORM_WINDOWS)
		explicit Font(HFONT font) noexcept;
		#endif
		// TODO: Duplicating a font is a pain. Skipping for now.
		Font(const Font&) = delete;
		Font(Font&& rhs) noexcept;

		~Font() noexcept;

		// TODO: Duplicating a font is a pain. Skipping for now.
		Font& operator =(const Font&) = delete;
		Font& operator =(Font&& rhs) noexcept;

		//private:

		#if defined(MAX_PLATFORM_WINDOWS)
		HFONT font;
		#endif

	};

} // namespace maxHex

#endif // #ifndef MAXHEX_FONT_HPP