#pragma once

#define LOOK_AHEAD		5	///< How many moves to look ahead
#define BOARD_WIDTH		7	///< Width of board
#define BOARD_HEIGHT	7	///< Height of board

#define HUMAN_PLAYER    1
#define COMPUTER_PLAYER 2

typedef struct {
	char x;		///< zero-indexed count across the board from bottom left
	char y;		///< zero-indexed count up the board from bottom left
} position_t;

typedef struct {
	char position[BOARD_WIDTH][BOARD_HEIGHT];	///< The board itself, populated with 0 (empty) or player number
	char moves_ahead;							///< How many moves ahead is this board
} board_t;
