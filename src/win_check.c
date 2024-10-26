/*
** EPITECH PROJECT, 2017
** PSU_my_sokoban_2017
** File description:
** Function that check if the game is won
** Edited by Thomas Minuzzo 2024 - adapted to Chessformer
*/

#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include "../include/libmy.h"
#include "../include/chessformer.h"

void win_check(chessformer_t chessformer)
{
	for (int i = 0; i < chessformer.lines; i++) {
		for (int j = 0; chessformer.map_save[i][j] != '\0'; j++) {
			if (chessformer.map[i][j] == '$')
				return ;
		}
	}

	endwin();
	exit (0);
	
}
