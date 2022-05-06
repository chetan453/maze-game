FILE=hello_sdl.cpp
run : 
	g++ main2.cpp game.cpp -w -lSDL2 -lSDL2_image -lSDL2_ttf -o a.out

main:
	g++ main.cpp -w -lSDL2 -lSDL2_image -lSDL2_ttf -o game.out;
	./game.out

image :
	g++ $(FILE) -w -lSDL2 -lSDL2_image -o a.out;
	./a.out;
	
main2:
	g++ main2.cpp game.cpp -w -lSDL2 -lSDL2_image -lSDL2_ttf -o game.out
clean :
	rm *.out 