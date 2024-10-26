#ifndef __UTILS__
#define __UTILS__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include <signal.h>
#include "../../include/chessformer.h"

#define SIZE 4
#define _XOPEN_SOURCE 500

/**
 * Data structure containing the information about the game state 
 * representing the state of the game.
 * Useful to determine 
 */
struct state_s {
    char **map;
	int player_x;
	int player_y;
};

typedef struct state_s state_t;

/**
 * Helper functions
 */
bool execute_move(chessformer_t *init_data, state_t *state, int move_delta_x, int move_delta_y);
bool winning_condition(chessformer_t *init_data, state_t *state);
void play_solution(chessformer_t init_data, char* solution);
void print_map(chessformer_t *init_data, state_t *state);
#endif

