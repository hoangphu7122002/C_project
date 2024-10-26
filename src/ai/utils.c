#include <assert.h>
#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include "utils.h"

static SCREEN *mainScreen = NULL;
static WINDOW *mainWindow = NULL;

/**************************************
* Player Moves given a state state  *
***************************************/
bool valid_queen_move(state_t* state, int move_delta_x, int move_delta_y) {
	if ((move_delta_x * move_delta_x) != (move_delta_y * move_delta_y)
		 && (move_delta_x != 0 && move_delta_y != 0)){
		// Queen must move diagonally or cardinally
		return false;
	}
	int squaresToMove = move_delta_x;
	if(squaresToMove < 0) {
		// Absolute value.
		squaresToMove = -squaresToMove;
	}
	if(squaresToMove == 0) {
		// No movement made.
		return false;
	}
	int x_dir;
	int y_dir;

	if (move_delta_x > 0) {
		// Positive
		x_dir = 1;
	} else if (move_delta_x == 0) {
		x_dir = 0;
	} else {
		// Negative
		x_dir = -1;
	}

	if (move_delta_y > 0) {
		// Positive
		y_dir = 1;
	} else if (move_delta_y == 0) {
		y_dir = 0;
	} else {
		// Negative
		y_dir = -1;
	}

	for (int i = 1; i <= squaresToMove; i++) {
		int new_x = state->player_x + (x_dir * i);
		int new_y = state->player_y + (y_dir * i);
		if(state->map[new_y][new_x] == '#'){
			// Can't move past a wall.
			return false;
		}
		if(state->map[new_y][new_x] == '$' && i != squaresToMove){
			// Can't move through a capturable piece.
			return false;
		}
	}

	return true;
}

bool valid_knight_move(int move_delta_x, int move_delta_y){
	// Possible knight deltas.
	int knight_delta_x[] = { -1,  1, -2,  2, -2,  2, -1,  1 };
	int knight_delta_y[] = { -2, -2, -1, -1,  1,  1,  2,  2 };
	for(size_t i = 0; i < sizeof(knight_delta_x) / sizeof(int); i++){
		if(knight_delta_x[i] == move_delta_x && knight_delta_y[i] == move_delta_y) {
			assert(move_delta_x * move_delta_x != move_delta_y * move_delta_y);
			assert(move_delta_x != 0);
			assert(move_delta_y != 0);
			return true;
		}
	}
	// Can reduce to this assert.
	assert((move_delta_x * move_delta_x == move_delta_y * move_delta_y) 
		|| move_delta_x != 0 || move_delta_y != 0);

	return false;
}

bool move_legal(chessformer_t* init_data, state_t* state, int move_delta_x, int move_delta_y) {
	int old_x = state->player_x;
	int old_y = state->player_y;
	int new_x = old_x + move_delta_x;
	int new_y = old_y + move_delta_y;
	
	// Basic checks.
	if(new_x < 0 || new_y < 0 || new_x >= 
		(init_data->num_chars_map / init_data->lines) || new_y >= init_data->lines){
		// Out of bounds
		return false;
	} 

	// Move up alone not legal
	if(move_delta_x == 0 && move_delta_y < 0) {
		return false;
	}


	if(valid_queen_move(state, move_delta_x, move_delta_y)){
		return true;
	}
	if(valid_knight_move(move_delta_x, move_delta_y)){
		return true;
	}

	return false;
}

bool make_move(chessformer_t* init_data, state_t* state, int move_delta_x, int move_delta_y) {
	int old_x = state->player_x;
	int old_y = state->player_y;
	int new_x = old_x + move_delta_x;
	int new_y = old_y + move_delta_y;
	// Not used but could be.
	(void) init_data;

	if(move_delta_x == 0 && move_delta_y == 0) {
		// No movement doesn't do anything.
		return false;
	}

	if(state->map[old_y][old_x] != '@') {
		// Player not in right place.
		return false;
	}
	if(state->map[new_y][new_x] == '#') {
		// Destination is wall.
		return false;
	}

	state->map[old_y][old_x] = ' ';
	state->map[new_y][new_x] = '@';
	state->player_x = new_x;
	state->player_y = new_y;

	// Gravity
	if(state->map[new_y + 1][new_x] != '#'){
		// Move down.
		make_move(init_data, state, 0, 1);
	}

	// Check if - after gravity - player moved.
	if(state->player_x == old_x && state->player_y == old_y) {
		return false;
	}

	return true;
}


bool execute_move(chessformer_t* init_data, state_t* state, int move_delta_x, int move_delta_y) {
    bool player_moved = false;

    // Determine which button is pushed
	if(move_legal(init_data, state, move_delta_x, move_delta_y)){
		player_moved = make_move(init_data, state, move_delta_x, move_delta_y);
	}

	return player_moved;
}

/*****************************************************************************
* Function:    winning_condition                                             *
* Parameters:  chessformer_t* init_data, state_t* state                          *
* Returns:     bool                                                          *
* Description: Check if all boxes are in a destination                       *
*****************************************************************************/


bool winning_condition(chessformer_t* init_data, state_t* state) {
    for (int y = 0; y < init_data->lines; y++) {
		for (int x = 0; init_data->map_save[y][x] != '\0'; x++) {
			if (state->map[y][x] == '$') {
				return false;
			}
		}
    }

    return true;
}

/*********
* MACROS *
*********/
#include <string.h>
#define TERMINAL_TYPE (strcmp(getenv("TERM"), "xterm") == 0 ? "rxvt" : \
  getenv("TERM"))

void play_solution(chessformer_t init_data, char *solution){
	// Alias
	chessformer_t chessformer = init_data;
	unsigned int moves_to_make = strlen(solution) / 2;

	mainScreen = newterm(TERMINAL_TYPE, stdout, stdin);
	set_term(mainScreen);
	int cols = 1;
	for(int i = 0; i < chessformer.lines; i++){
		if(strlen(chessformer.map[i]) > (size_t) cols){
			cols = strlen(chessformer.map[i]);
		}
	}
	mainWindow = newwin(chessformer.lines, cols, 0, 0);

	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	clear();
	
	for (long unsigned int i = 0; i <= moves_to_make; i++) {
			while(chessformer.map[chessformer.player_y+1][chessformer.player_x] != '#'){
				refresh();
				//Print graph
				int columns = chessformer.num_chars_map/chessformer.lines;
				char letter[2] = "a";
				for(int i = 1; i<columns-1; i++){
					mvprintw(chessformer.lines, i, letter);
					letter[0] ++; //Move letter along, by editing string
				}
				char number[2] = "1";
				for (int i = 0; i < chessformer.lines; i++){
					if(i!=0 && i!=chessformer.lines-1){
						mvprintw(i, chessformer.lines-3, number); //-3 for new line and null byte and array zero offset
						number[0]++; //Move number along, by editing string
					}
					mvprintw(i, 0, chessformer.map[i]);
				}
				move(chessformer.player_y, chessformer.player_x);

				usleep(500000);
				chessformer = move_down(chessformer);
				//Apply gravity in loop for visual effects
			}
			refresh();
			

			// Print graph again for final update (TO DO: write a function for this)
			int columns = chessformer.num_chars_map/chessformer.lines;
			char letter[2] = "a";
			for(int i = 1; i < columns - 1; i++){
				mvprintw(chessformer.lines, i, letter);
				letter[0] ++; //Move letter along, by editing string
			}
			char number[2] = "1";
			for (int i = 0; i < chessformer.lines; i++){
				if(i!=0 && i!=chessformer.lines-1){
					mvprintw(i, columns, number); //-3 for new line and null byte and array zero offset
					//mvprintw(i, chessformer.lines-3, number); //-3 for new line and null byte and array zero offset
					number[0]++; //Move number along, by editing string
				}
				mvprintw(i, 0, chessformer.map[i]);
			}
			move(chessformer.player_y, chessformer.player_x);

			usleep(500000);
			int moveLetter = 0, moveNumber = 0;
			moveLetter = solution[i * 2];
			moveNumber = solution[i * 2 + 1];

			win_check(chessformer);// extra win check for gravity
			//Check for key presses
			chessformer = key_check(chessformer, moveLetter, moveNumber);

			win_check(chessformer);
	}

	/*
	SCREEN *mainScreen = newterm(TERMINAL_TYPE, stdout, stdin);
	set_term(mainScreen);
	int cols = 1;
	for(int i = 0; i < init_data.lines; i++){
		if(strlen(init_data.map[i]) > (size_t) cols){
			cols = strlen(init_data.map[i]);
		}
	}
	WINDOW *mainWindow = newwin(init_data.lines, cols, 0, 0);

	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	clear();
	
	for (long unsigned int i = 0; i <= strlen(solution); i++) { 
		touchwin(mainWindow);
		wnoutrefresh(mainWindow);
		doupdate();
		refresh();
		for (int i = 0; i < init_data.lines; i++) {
			mvprintw(i, 0, init_data.map[i]);
		}
		move(init_data.player_y, init_data.player_x);

		int key_pressed = 0;

		if( solution[i] == 'u' || solution[i] == 'U')
			key_pressed = KEY_UP;
		else if( solution[i] == 'd' || solution[i] == 'D')
			key_pressed = KEY_DOWN;	
		else if( solution[i] == 'l' || solution[i] == 'L')
			key_pressed = KEY_LEFT;	
		else if( solution[i] == 'r' || solution[i] == 'R')
			key_pressed = KEY_RIGHT;	
		init_data = key_check(init_data, key_pressed);
		init_data = check_zone_reset(init_data);
		usleep(500000);
	}
	touchwin(mainWindow);
	wnoutrefresh(mainWindow);
	doupdate();
	refresh();
	usleep(1500000);
	*/
}

void print_map(chessformer_t* init_data, state_t* state ){
	initscr();
	cbreak();
	noecho();
	clear();
	for (int i = 0; i < init_data->lines; i++){
		mvprintw(i, 0, state->map[i]);
		move(state->player_y, state->player_x);
	}
	refresh();
}
