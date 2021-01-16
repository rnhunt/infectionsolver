#pragma once
#include "InfectionSolver.h"

void newBoard(board_t* board);
void displayBoard(const board_t* board, position_t last_from, position_t last_to);
bool updateBoard(board_t* board, position_t from, position_t to, char player);
bool prompt(board_t* board, position_t* returned_from, position_t* returned_to);