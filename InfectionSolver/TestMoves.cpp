#include "InfectionSolver.h"
#include "Utils.h"
#include <iostream>

static position_t best_play_from;
static position_t best_play_to;

void get_play(position_t* from, position_t* to) {
	*from = best_play_from;
	*to = best_play_to;
}

static int score_board(board_t* board) {
	// always calculate from perspective of player 1
	int num_pieces = 0;
	for (int x = 0; x < BOARD_WIDTH; x++) {
		for (int y = 0; y < BOARD_HEIGHT; y++) {
			if (board->position[x][y] != 0) {
				if (board->position[x][y] == 1) {
					num_pieces++;
				}
				else {
					num_pieces--;
				}
			}
		}
	}
	return num_pieces;
}

/// \brief Calculate the best move to make for the player
/// \param[in] board the board including level information
/// \param[in] player which player to move
/// \param[out] best_play_from the best "from" move to play (most pieces on board belong to player)
/// \param[out] best_play_to the best "to" move to play (most pieces on board belong to player)
/// \return the number of pieces we end up with for the best move
int makeNextMove(board_t board, int player) {

	int player_colour = (2 == player) ? -1 : 1;

	// Brute force test each possible move, calling ourselves to find out worst resulting score for that move
	position_t from = { 0 }, to = { 0 };
	board.moves_ahead++;

	if (board.moves_ahead > LOOK_AHEAD) {
		// Simply return score for this board
		return player_colour * score_board(&board);
	}

	int value = -BOARD_WIDTH * BOARD_HEIGHT;

	int lx, rx, ty, by;
	for (from.x = 0; from.x < BOARD_WIDTH; from.x++) {
		lx = from.x - 2;
		rx = from.x + 2;
		if (lx < 0) lx = 0;
		if (rx >= BOARD_WIDTH) rx = BOARD_WIDTH-1;

		for (from.y = 0; from.y < BOARD_HEIGHT; from.y++) {

			if (board.moves_ahead == 1) {
				std::cout << "*";
			}

			if (board.position[from.x][from.y] == player) {
				ty = from.y + 2;
				by = from.y - 2;
				if (by < 0) by = 0;
				if (ty >= BOARD_HEIGHT) ty = BOARD_HEIGHT-1;

				for (to.x = lx; to.x <= rx; to.x++) {
					for (to.y =by; to.y <= ty; to.y++) {
						if ((to.x != from.x) && (to.y != from.y)) {
							if (board.position[to.x][to.y] == 0) {
								board_t testboard = board;
								updateBoard(&testboard, from, to, player);
								int movescore = -makeNextMove(testboard, 1 == player ? 2 : 1);
								if (movescore > value) {
									value = movescore;
									if (1 == board.moves_ahead) {
										best_play_from = from;
										best_play_to = to;
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return value;
}