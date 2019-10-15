#include "UserInteractionState.hpp"

namespace maxHex
{

	UserInteractionState::UserInteractionState(int VerticalScrollOffset, int HorizontalScrollOffset) noexcept
		: VerticalScrollOffset(VerticalScrollOffset)
		, HorizontalScrollOffset(HorizontalScrollOffset)
	{
	}

} // namespace maxHex