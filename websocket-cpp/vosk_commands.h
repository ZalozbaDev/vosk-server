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
	float getSampleRate() { return sample_rate; }
	std::string getModel() { return model; }
	bool getWords() { return words; }
	~VoskCommands();
private:
	void resetValues();
	
	const static int max_cmd_len = 100;
	
	bool eof;
	float sample_rate;
	std::string model;
	bool words;
};


#endif // VOSK_COMMANDS_H
