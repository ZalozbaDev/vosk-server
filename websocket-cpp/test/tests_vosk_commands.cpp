
#include "doctest.h"

#include <stddef.h>
#include <stdint.h>


#include <iostream>

#include <fstream>
#include <vector>

#include "vosk_commands.h"

TEST_CASE("test buffer parsing")
{
	SUBCASE("test long message") {
		const char longMsg[200] = {0};	
		CHECK(VoskCommands::isCommand(longMsg, sizeof(longMsg)) == false);
		
		CHECK(VoskCommands::isCommand(longMsg, 101) == false);
	}

}

