/*
** EPITECH PROJECT, 2017
** PSU_my_sokoban_2017
** File description:
** Function that manage the game
** ----------
** Adapted by Nir Lipo, 2021
** Edited by Thomas Minuzzo 2024 - adapted to Chessformer
*/


#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include "../include/libmy.h"
#include "../include/chessformer.h"

/*********
* MACROS *
*********/
#include <string.h>
#define TERMINAL_TYPE (strcmp(getenv("TERM"), "xterm") == 0 ? "rxvt" : \
  getenv("TERM"))

static SCREEN *mainScreen = NULL;
static WINDOW *mainWindow = NULL;


int play(char const *path)
{
	/**
	 * Load Map
	*/
	chessformer_t chessformer = make_map(path, chessformer);
	
	/**
	 * Count number of boxes and Storage locations
	*/
	map_check(chessformer);

	/**
	 * Locate player x,y position
	*/
	chessformer = find_player(chessformer);
	
	chessformer.base_path = path;

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
	
	while (1) {
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
			

			//Print graph again for final update (TO DO: write a function for this)
			int columns = chessformer.num_chars_map/chessformer.lines;
			char letter[2] = "a";
			for(int i = 1; i<columns-1; i++){
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
			win_check(chessformer);// extra win check for gravity
			//Check for key presses
			chessformer = game_management(chessformer);	
	}
}

chessformer_t game_management(chessformer_t chessformer)
{
	int letter = 0 , number = 0;

	mvprintw(chessformer.lines+1,0,"Enter a letter and a number to move to that space");
	mvprintw(chessformer.lines+2,0,"Letter:    ");
	mvprintw(chessformer.lines+3,0,"Number:    ");

	letter = getch();
	mvprintw(chessformer.lines+2,0,"Letter: %c",letter);
	number = getch();
	mvprintw(chessformer.lines+3,0,"Number: %c",number);

	chessformer = key_check(chessformer, letter, number);
	win_check(chessformer);
	return (chessformer);
}
