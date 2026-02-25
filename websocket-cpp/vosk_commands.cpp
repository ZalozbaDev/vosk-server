#include "vosk_commands.h"

#include <cstddef>

//////////////////////////////////////////////////////
VoskCommands::VoskCommands()
{
	
}

//////////////////////////////////////////////////////
bool VoskCommands::parseCommand(const char *message, int len)
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
	
	// parse now
	// the parser can handle non-null terminated strings, no need to check for trailing 0
	json parser_res = json::parse(message, message + len, nullptr, false);
	
	// return parser failure
	if (parser_res.is_discarded()) 
	{
		return false;
	}
	
	// reset all values before checking 
	resetValues();
	
	// check for eof
	eof = (parser_res.value("eof", 0) == 1) ? true : false;
	
	
	return true;
}

//////////////////////////////////////////////////////
void VoskCommands::resetValues()
{
	eof = false;
}

//////////////////////////////////////////////////////
VoskCommands::~VoskCommands()
{
	
}

