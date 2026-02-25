#ifndef VOSK_COMMANDS_H
#define VOSK_COMMANDS_H

#include <nlohmann/json.hpp>

using json = nlohmann::json;

class VoskCommands
{
public:
	static bool isCommand(const char *message, int len);
	~VoskCommands();
private:
	VoskCommands();
	
	const static int max_cmd_len = 100;
};


#endif // VOSK_COMMANDS_H
