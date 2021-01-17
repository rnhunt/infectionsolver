#include "InfectionSolver.h"
#include "Utils.h"
#include <iostream>

static position_t best_play_from;
static position_t best_play_to;

///\brief Return the determined best move to play
///\param[in] board the board
///\param[out] from from
///\param[out] to to
void get_play(board_t* board, position_t* from, position_t* to) {
	// For some reason, the AI will sometimes jump when it can slurp to this position instead!
	// So, see if there is one of our pieces closer to the to we can just slurp from instead
	int lx = best_play_to.x - 1;
	int rx = best_play_to.x + 1;
	int ty = best_play_to.y + 1;
	int by = best_play_to.y - 1;
	if (lx < 0) lx = 0;
	if (rx >= BOARD_WIDTH) rx = BOARD_WIDTH - 1;
	if (by < 0) by = 0;
	if (ty >= BOARD_HEIGHT) ty = BOARD_HEIGHT - 1;
	for (int x = lx; x <= rx; x++) {
		for (int y = by; y <= ty; y++) {
			if (board->position[x][y] == board->position[best_play_from.x][best_play_from.y]) {
				// Choose this, same outcome but we gain a piece
				best_play_from.x = x;
				best_play_from.y = y;
				break;
			}
		}
	}
	// Set the best play
	*from = best_play_from;
	*to = best_play_to;
}


///\brief Score the board from player 1's perspective
///\param[in] board the board
///\return the score
static int score_board(board_t* board) {
	// always calculate from perspective of player 1
	int num_pieces = 0;
	for (int x = 0; x < BOARD_WIDTH; x++) {
		for (int y = 0; y < BOARD_HEIGHT; y++) {
			if (board->position[x][y] != 0) {
				if (board->position[x][y] == 1) {
					num_pieces++;
				}
			}
		}
	}
	return num_pieces;
}


/// \brief Calculate the best move to make for the player, negmax method
/// \param[in] board the board including level information
/// \param[in] player which player to move (1 or 2)
/// \return the best value player 1 can make as a result of this move
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