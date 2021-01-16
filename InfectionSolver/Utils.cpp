#include <iostream>
#include <cstring>
#include <stdlib.h>

#include "InfectionSolver.h"

static void clear() {
#if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
	system("clear");
#endif

#if defined(_WIN32) || defined(_WIN64)
	system("cls");
#endif
}

void newBoard(board_t* board) {
	memset(board, 0, sizeof(board_t));
	board->position[0][BOARD_HEIGHT - 1] = 2;
	board->position[BOARD_WIDTH - 1][0] = 2;
	board->position[BOARD_WIDTH - 1][BOARD_HEIGHT - 1] = 1;
	board->position[0][0] = 1;
}

void displayBoard(const board_t* board, position_t last_from, position_t last_to) {
	clear();
	for (int y = (BOARD_HEIGHT - 1); y >= 0; y--) {
		char line[2 + BOARD_WIDTH * 4 + 1];
		memset(line, ' ', sizeof(line));
		line[2 + BOARD_WIDTH * 4] = 0;
		for (int x = 0; x < BOARD_WIDTH; x++) {
			if (board->position[x][y]) {
				char piece = 0xb3-board->position[x][y];
				line[(x * 4) + 2] = piece;
				line[(x * 4) + 3] = piece;
				line[(x * 4) + 4] = piece;
			}
		}
		for (int i = 0; i < 3; i++) {
			line[0] = (1 == i) ? '0' + y : ' ';
			std::cout << line << std::endl;
		}
		std::cout << std::endl;
	}
	std::cout << "   A   B   C   D   E   F   G" << std::endl;
	std::cout << std::endl;
	std::cout << "Computer moved " << char('A'+ last_from.x) << char('0'+ last_from.y) << " to " << char('A' + last_to.x) << char('0' + last_to.y) << std::endl;
	std::cout << std::endl;
}

bool updateBoard(board_t* board, position_t from, position_t to, char player) {
	bool legalmove = false;
	if (!((from.x == to.x) && (from.y == to.y)) &&
		(player == board->position[from.x][from.y]) &&
		(0 == board->position[to.x][to.y])) {
		if ((abs(to.x - from.x) <= 2) && (abs(to.y - from.y) <= 2)) {
			if (((abs(from.x - to.x)) > 1) || (abs(from.y - to.y) > 1)) {
				// A jump, so blank where we came from
				board->position[from.x][from.y] = 0;
			}
			// Put piece in new position
			board->position[to.x][to.y] = player;
			// Infect any surrounding pieces
			for (int x = (to.x - 1); x <= (to.x + 1); x++) {
				if ((x >= 0) && (x < BOARD_WIDTH)) {
					for (int y = (to.y - 1); y <= (to.y + 1); y++) {
						if ((y >= 0) && (y < BOARD_HEIGHT)) {
							if (board->position[x][y] != 0) {
								board->position[x][y] = player;
							}
						}
					}
				}
			}
			legalmove = true;
		}
	}
	return legalmove;
}

bool prompt(board_t* board, position_t* returned_from, position_t* returned_to) {
	bool moved = false;
	std::string input;
	while (4 != input.length()) {
		std::cout << "Enter move (e.g. A1B4 or A1=x to force set)" << std::endl;
		std::cin >> input;
	}

	position_t from, to;
	from.x = ::toupper(input[0]) - 'A';
	from.y = ::toupper(input[1]) - '0';
	if ((from.x >= 0) && (from.x < BOARD_WIDTH) &&
		(from.y >= 0) && (from.y < BOARD_HEIGHT)) {

		if ('=' == input[2]) {
			// Force set piece
			if ((input[3] >= '0') && (input[3] <= '2')) {
				board->position[from.x][from.y] = input[3] - '0';
			}
		}
		else {
			// Move it if allowed
			to.x = ::toupper(input[2]) - 'A';
			to.y = ::toupper(input[3]) - '0';
			if ((to.x >= 0) && (to.x < BOARD_WIDTH) &&
				(to.y >= 0) && (to.y < BOARD_HEIGHT)) {
				moved = updateBoard(board, from, to, HUMAN_PLAYER);
				*returned_from = from;
				*returned_to = to;
			}
		}
	}
	return moved;
}