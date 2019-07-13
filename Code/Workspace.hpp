#ifndef MAXHEX_WORKSPACE_HPP
#define MAXHEX_WORKSPACE_HPP

#include <Windows.h>

namespace maxHex
{

	class Workspace
	{
	public:

		Workspace();
		Workspace(Workspace&& rhs);
		~Workspace();

		Workspace& operator =(Workspace&& rhs);

		const char* Buffer;
		size_t BufferLength;

		friend Workspace CreateWorkspaceFromFile(LPTSTR FilePath);

	private:

		// TODO: This should really take a buffer list, not the file
		Workspace(LPTSTR FilePath);

	};

	Workspace CreateWorkspaceFromFile(LPTSTR FilePath);

} // namespace maxHex

#endif // #ifndef MAXHEX_WORKSPACE_HPP