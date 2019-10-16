#ifndef MAXHEX_USERINTERACTIONSTATE_HPP
#define MAXHEX_USERINTERACTIONSTATE_HPP

namespace maxHex
{

	class UserInteractionState
	{
	public:

		UserInteractionState() = default;
		UserInteractionState(int VerticalScrollOffset, int HorizontalScrollOffset) noexcept;
		UserInteractionState(const UserInteractionState&) = default;
		UserInteractionState(UserInteractionState&&) = default;
		~UserInteractionState() = default;

		UserInteractionState& operator =(const UserInteractionState&) = default;
		UserInteractionState& operator =(UserInteractionState&&) = default;

		int VerticalScrollOffset = 0;
		int HorizontalScrollOffset = 0;

	};

} // namespace maxHex

#endif // #ifndef MAXHEX_RASTERER_HPP