// InfectionSolver.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "InfectionSolver.h"
#include "Utils.h"
#include "TestMoves.h"

int main()
{
    board_t board;
    position_t last_from = { 0 }, last_to = { 0 };
    newBoard(&board);
    makeNextMove(board, COMPUTER_PLAYER);
    get_play(&board, &last_from, &last_to);
    updateBoard(&board, last_from, last_to, COMPUTER_PLAYER);
    while (true) {
        displayBoard(&board, last_from, last_to);
        if (prompt(&board, &last_from, &last_to)) {
            // update with out move
            displayBoard(&board, last_from, last_to);
            // we (stauf) have moved, so work out best move to respond with
            makeNextMove(board, COMPUTER_PLAYER);
            get_play(&board, &last_from, &last_to);
            updateBoard(&board, last_from, last_to, COMPUTER_PLAYER);
        }

    }
}
