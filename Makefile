FLAGS = -std=c++11 \
		-Wall -Wno-uninitialized \
		      -Wno-deprecated-declarations \
			  -Wsign-compare

game: tictactoe.cpp
	g++ $(FLAGS) tictactoe.cpp -o game
