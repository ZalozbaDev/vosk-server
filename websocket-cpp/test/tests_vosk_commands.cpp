
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
	}

}

