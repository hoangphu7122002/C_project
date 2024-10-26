/*
** EPITECH PROJECT, 2017
** PSU_my_sokoban_2017
** File description:
** Function for finding the player on the map
** Edited by Thomas Minuzzo 2024 - adapted to Chessformer
*/

#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include "../include/libmy.h"
#include "../include/chessformer.h"

chessformer_t find_player(chessformer_t chessformer)
{
	chessformer.player_x = 0;
	chessformer.player_y = 0;
	for (int i = 0; i < chessformer.lines; i++) {
		for (int j = 0; chessformer.map[i][j] != '\0'; j++) {
			chessformer = check_if_player(chessformer, i, j);
		}
	}
	return (chessformer);
}

chessformer_t check_if_player(chessformer_t chessformer, int y, int x)
{
	if (chessformer.map[y][x] == '@') {
		chessformer.player_x = x;
		chessformer.player_y = y;
	}
	return (chessformer);
}
