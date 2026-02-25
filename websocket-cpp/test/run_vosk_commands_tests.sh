#!/bin/bash

rm -f ./tests_vosk_commands

g++ -o tests_vosk_commands -g3 -Wall -Wextra -I. -I.. -I../doctest/doctest/ test_support.cpp tests_vosk_commands.cpp \
../vosk_commands.cpp

./tests_vosk_commands

