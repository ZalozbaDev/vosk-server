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
	// we must find a { in the first characters, skipping spaces only (if any)
	std::size_t i = 0;
	while (i < 5 && message[i] == ' ') ++i;
	if (message[i] != '{')
	{
		return false;
	}
	
	// this buffer is a candidate for processing, now we can try if it parses properly
	
	// the parser can handle non-null terminated strings, no need to check for trailing 0
	json parser_res = json::parse(message, message + len, nullptr, false);
	
	// simply return parser result
	return !parser_res.is_discarded();
}

