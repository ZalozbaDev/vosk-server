#ifndef VOSK_COMMANDS_H
#define VOSK_COMMANDS_H

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
