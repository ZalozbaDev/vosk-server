
#include "doctest.h"

#include <stddef.h>
#include <stdint.h>


#include <iostream>

#include <fstream>
#include <vector>

#include "vosk_commands.h"

TEST_CASE("test buffer parsing")
{
	VoskCommands cmds;
	
	SUBCASE("test long message") {
		const char longMsg[200] = {0};	
		CHECK(cmds.parseCommand(longMsg, sizeof(longMsg)) == false);
		
		CHECK(cmds.parseCommand(longMsg, 101) == false);
	}

	SUBCASE("test short message") {
		const char shortMsg[8] = {0};	
		CHECK(cmds.parseCommand(shortMsg, sizeof(shortMsg)) == false);
	}

	SUBCASE("test invalid message") {
		const char testMsg[9] = {-0x07};	
		CHECK(cmds.parseCommand(testMsg, sizeof(testMsg)) == false);
	}

	SUBCASE("test too many spaces") {
		const char testMsg[9] = {0x20};	
		CHECK(cmds.parseCommand(testMsg, sizeof(testMsg)) == false);
	}

	SUBCASE("test invalid string") {
		const char testMsg[] = "sample_rate=";	
		CHECK(cmds.parseCommand(testMsg, sizeof(testMsg)) == false);
	}

	SUBCASE("test no closing bracket") {
		const char testMsg[] = "{ \"eof\": 1";	
		CHECK(cmds.parseCommand(testMsg, sizeof(testMsg)) == false);
	}

	SUBCASE("test valid msg") {
		const char testMsg[] = "{ \"eof\": 1}";	
		CHECK(cmds.parseCommand(testMsg, sizeof(testMsg)) == true);
		CHECK(cmds.isEof() == true);
	}

	SUBCASE("test valid msg but unexpected value") {
		const char testMsg[] = "{ \"eof\": 15}";	
		CHECK(cmds.parseCommand(testMsg, sizeof(testMsg)) == true);
		CHECK(cmds.isEof() == false);
	}

	SUBCASE("test valid msg but unexpected value") {
		const char testMsg[] = "{ \"eof\": 0}";	
		CHECK(cmds.parseCommand(testMsg, sizeof(testMsg)) == true);
		CHECK(cmds.isEof() == false);
	}

}

TEST_CASE("API tests")
{
	VoskCommands cmds;
	
	SUBCASE("test EOF again") {
		const char testMsg[] = "{\"eof\":1}";	
		CHECK(cmds.parseCommand(testMsg, sizeof(testMsg)) == true);
		CHECK(cmds.isEof() == true);
	}

	SUBCASE("test config --> sample_rate") {
		const char testMsg[] = "{ \"config\" : { \"sample_rate\" : 16000 }}";	
		CHECK(cmds.parseCommand(testMsg, sizeof(testMsg)) == true);
		CHECK(cmds.isEof() == false);
		CHECK(cmds.getSampleRate() == 16000.0f);
	}

	SUBCASE("test config --> sample_rate (invalid)") {
		const char testMsg[] = "{ \"config\" : { \"sample_rate\" : \"murks\" }}";	
		CHECK(cmds.parseCommand(testMsg, sizeof(testMsg)) == true);
		CHECK(cmds.isEof() == false);
		CHECK(cmds.getSampleRate() == -1.0f);
	}

	SUBCASE("test config --> model") {
		const char testMsg[] = "{ \"config\" : { \"model\" : \"test.ggml\" }}";	
		CHECK(cmds.parseCommand(testMsg, sizeof(testMsg)) == true);
		CHECK(cmds.isEof() == false);
		CHECK(cmds.getSampleRate() == -1.0f);
		// std::cout << "Model: '" << cmds.getModel() << "'" << std::endl;
		CHECK(cmds.getModel() == "test.ggml");
	}

	SUBCASE("test config --> words") {
		const char testMsg[] = "{ \"config\" : { \"words\" : true }}";	
		CHECK(cmds.parseCommand(testMsg, sizeof(testMsg)) == true);
		CHECK(cmds.isEof() == false);
		CHECK(cmds.getSampleRate() == -1.0f);
		// std::cout << "Model: '" << cmds.getModel() << "', length=" << cmds.getModel().length() << std::endl;
		CHECK(cmds.getModel().length() == 0);
		CHECK(cmds.getWords() == true);
	}

	SUBCASE("test config --> words (invalid value)") {
		const char testMsg[] = "{ \"config\" : { \"words\" : True }}";	
		CHECK(cmds.parseCommand(testMsg, sizeof(testMsg)) == false);
	}

	SUBCASE("test config --> words") {
		const char testMsg[] = "{ \"config\" : { \"words\" : false }}";	
		CHECK(cmds.parseCommand(testMsg, sizeof(testMsg)) == true);
		CHECK(cmds.getWords() == false);
	}

	SUBCASE("test config --> sample_format") {
		const char testMsg[] = "{ \"config\" : { \"sample_format\" : \"ULAW\" }}";	
		CHECK(cmds.parseCommand(testMsg, sizeof(testMsg)) == true);
		CHECK(cmds.getSampleFormat() == SampleFormat::ULAW);
	}

	SUBCASE("test config --> sample_format") {
		const char testMsg[] = "{ \"config\" : { \"sample_format\" : \"PCMS16LE\" }}";	
		CHECK(cmds.parseCommand(testMsg, sizeof(testMsg)) == true);
		CHECK(cmds.getSampleFormat() == SampleFormat::PCMS16LE);
	}

	SUBCASE("test config --> chunklen") {
		const char testMsg[] = "{ \"config\" : { \"chunklen\" : 4096 }}";	
		CHECK(cmds.parseCommand(testMsg, sizeof(testMsg)) == true);
		CHECK(cmds.getChunklen() == 4096);
	}

	SUBCASE("test config --> chunklen (invalid)") {
		const char testMsg[] = "{ \"config\" : { \"chunklen\" : -20 }}";	
		CHECK(cmds.parseCommand(testMsg, sizeof(testMsg)) == true);
		CHECK(cmds.getChunklen() == std::numeric_limits<unsigned int>::max());
	}

	
}
