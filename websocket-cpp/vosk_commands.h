#ifndef VOSK_COMMANDS_H
#define VOSK_COMMANDS_H

#include <nlohmann/json.hpp>

using json = nlohmann::json;

class VoskCommands
{
public:
	VoskCommands();
	bool parseCommand(const char *message, int len);
	bool isEof() { return eof; }
	~VoskCommands();
private:
	void resetValues();
	
	const static int max_cmd_len = 100;
	
	bool eof;
};


#endif // VOSK_COMMANDS_H
