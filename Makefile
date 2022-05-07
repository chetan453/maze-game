FILE=main.cpp

main2:
	g++ $(FILE) -w -lSDL2 -lSDL2_image -lSDL2_ttf -o a.out;
	./a.out

clean :
	rm *.out 