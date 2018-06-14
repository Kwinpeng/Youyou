FLAGS = -std=c++11 \
		-Wall -Wno-uninitialized \
		      -Wno-deprecated-declarations \
			  -Wsign-compare \
		-O0

game: tictactoe.cpp
	g++ $(FLAGS) tictactoe.cpp -o game

clean:
	@rm -f *.o game
