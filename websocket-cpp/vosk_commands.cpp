#include "vosk_commands.h"

#include <cstddef>

//////////////////////////////////////////////////////
bool VoskCommands::isCommand(const char *message, int len)
{
	// first decision must be fast, base it simply on buffer length
	// filter out too short packets too (EOF packet is the shortest)
	if ((len > max_cmd_len) || (len < 9))
	{
		return false;	
	}
	
	// second decision shall still be fast, check if it could be a JSON string at all
	std::size_t i = 0;
	while (i < 5 && message[i] == ' ') ++i;
	if (message[i] != '{')
	{
		return false;
	}
	
	return true;
}