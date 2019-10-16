#ifndef MAXHEX_RASTERIZER_HPP
#define MAXHEX_RASTERIZER_HPP

#include <max/Compiling/Configuration.hpp>

#if defined(MAX_PLATFORM_WINDOWS)
#include <Windows.h>
#endif
#include "BufferChain.hpp"
#include "Font.hpp"
#include "UserInteractionState.hpp"

namespace maxHex
{

	class Rasterizer
	{
	public:

		static Rasterizer Create(HDC DeviceContext, const maxHex::Font& font) noexcept;

		Rasterizer() = delete;
		Rasterizer(int CharacterHeight, int CharacterWidth) noexcept;
		Rasterizer(const Rasterizer&) = default;
		Rasterizer(Rasterizer&&) = default;

		Rasterizer& operator =(const Rasterizer&) = default;
		Rasterizer& operator =(Rasterizer&&) = default;

		#if defined(MAX_PLATFORM_WINDOWS)
		void Rasterize(HWND WindowHandle, HDC DeviceContext, int VirtualTop, int VirtualLeft, int Height, int Width, const BufferChain& Buffers, const UserInteractionState& InterationState) noexcept;
		#endif

	//private:

		int CharacterHeight;
		int CharacterWidth;

	};

} // namespace maxHex

#endif // #ifndef MAXHEX_RASTERIZER_HPP