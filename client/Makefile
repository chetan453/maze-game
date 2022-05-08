FILE=main.cpp
COMPILER_FLAGS = -g -w -std=c++17 -O2 -Wall -Wextra -pedantic -Wformat=2 -Wstrict-aliasing=2 -MMD

run:
	g++ $(FILE) $(COMPILER_FLAGS) -lSDL2 -lSDL2_image -lSDL2_ttf -o client;

clean :
	rm *.out 