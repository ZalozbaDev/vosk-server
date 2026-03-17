#include "vosk_commands.h"

#include <cstddef>
#include <iostream>

//////////////////////////////////////////////////////
VoskCommands::VoskCommands()
{
	valids.reset();
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
	
	// time stamp likely to be most frequent message
	if (parser_res.contains("ts"))
	{
		timestamp.valid = false;
		
		if ((parser_res["ts"].contains("s")) && (parser_res["ts"].contains("ms")))
		{
			if ((parser_res["ts"]["s"].is_number_unsigned()) && (parser_res["ts"]["ms"].is_number_unsigned()))
			{
				timestamp.seconds = parser_res["ts"]["s"].get<unsigned int>();
				timestamp.milliseconds = parser_res["ts"]["ms"].get<unsigned int>();
				timestamp.valid = true;
				
				valids.set(static_cast<std::size_t>(ValidBitsPositions::TIMESTAMP));
			}
		}
	}
	
	// check for several config settings
	if (parser_res.contains("config"))
	{
		// sample_rate
		if (parser_res["config"].contains("sample_rate"))
		{
			if (parser_res["config"]["sample_rate"].is_number_integer())
			{
				sample_rate = (float) (parser_res["config"]["sample_rate"].get<int>());
				valids.set(static_cast<std::size_t>(ValidBitsPositions::SAMPLE_RATE));
			}
			else
			{
				sample_rate = -1.0f;
			}
		}
		
		// model
		if (parser_res["config"].contains("model"))
		{
			if (parser_res["config"]["model"].is_string())
			{
				model = parser_res["config"]["model"].get<std::string>();
				valids.set(static_cast<std::size_t>(ValidBitsPositions::MODEL));
			}
		}

		// words
		if (parser_res["config"].contains("words"))
		{
			if (parser_res["config"]["words"].is_boolean())
			{
				words = parser_res["config"]["words"].get<bool>();
				valids.set(static_cast<std::size_t>(ValidBitsPositions::WORDS));
			}
		}
		
		// sample_format
		if (parser_res["config"].contains("sample_format"))
		{
			format = SampleFormat::PCMS16LE;
			
			if (parser_res["config"]["sample_format"].is_string())
			{
				if (parser_res["config"]["sample_format"].get<std::string>() == "ULAW")
				{
					format = SampleFormat::ULAW;
				}
				
				valids.set(static_cast<std::size_t>(ValidBitsPositions::FORMAT));
			}
		}

		// chunklen
		if (parser_res["config"].contains("chunklen"))
		{
			if (parser_res["config"]["chunklen"].is_number_unsigned())
			{
				chunklen = parser_res["config"]["chunklen"].get<unsigned int>();
				valids.set(static_cast<std::size_t>(ValidBitsPositions::CHUNKLEN));
			}
			else
			{
				chunklen = std::numeric_limits<unsigned int>::max();
			}
		}
		
		
	}
	
	// check for eof
	eof = false;
	if (parser_res.value("eof", 0) == 1)
	{
		eof = true;
		valids.set(static_cast<std::size_t>(ValidBitsPositions::VOSK_EOF));
	}
	
	return true;
}

//////////////////////////////////////////////////////
void VoskCommands::resetValues()
{
	eof = false;
	sample_rate = -1.0f;
	model.erase();
	words = false;
	format = SampleFormat::PCMS16LE;
	chunklen = std::numeric_limits<unsigned int>::max();
	timestamp.valid = false;
	
	valids.reset();
}

//////////////////////////////////////////////////////
VoskCommands::~VoskCommands()
{
	
}

