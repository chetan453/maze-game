FILE=hello_sdl.cpp
run : 
	g++ $(FILE) -w -lSDL2 -o a.out

main:
	g++ main.cpp -w -lSDL2 -lSDL2_image -o game.out;
	./game.out

image :
	g++ $(FILE) -w -lSDL2 -lSDL2_image -o a.out;
	./a.out;
	
clean :
	rm *.out 