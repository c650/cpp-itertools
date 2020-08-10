IncludeDir=./src/include
CFLAGS= --std=c++17 -Wall -Werror -I ${IncludeDir}

test: tests/main.cpp ./src/include/itertools.hpp
	g++ tests/main.cpp -o bin/tests ${CFLAGS} && ./bin/tests

