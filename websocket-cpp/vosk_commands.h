#ifndef VOSK_COMMANDS_H
#define VOSK_COMMANDS_H

#include <nlohmann/json.hpp>

using json = nlohmann::json;

enum class SampleFormat {
    PCMS16LE,  // 16-bit PCM, signed endian
    ULAW      // μ-law encoding
};

struct audio_timestamp
{
	bool valid;
	uint64_t seconds;
	uint64_t milliseconds;
};

class VoskCommands
{
public:
	VoskCommands();
	bool parseCommand(const char *message, int len);
	bool isEof() { return eof; }
	float getSampleRate() { return sample_rate; }
	std::string getModel() { return model; }
	bool getWords() { return words; }
	SampleFormat getSampleFormat() { return format; }
	unsigned int getChunklen() { return chunklen; }
	audio_timestamp getAudioTimestamp() { return timestamp; }
	~VoskCommands();
private:
	void resetValues();
	
	const static int max_cmd_len = 100;
	
	bool eof;
	float sample_rate;
	std::string model;
	bool words;
	SampleFormat format;
	unsigned int chunklen;
	audio_timestamp timestamp;
};


#endif // VOSK_COMMANDS_H
