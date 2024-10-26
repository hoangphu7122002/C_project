/*
** EPITECH PROJECT, 2017
** PSU_my_sokoban_2017
** File description:
** Function used to check if a map is valid or not
** Edited by Thomas Minuzzo 2024 - adapted to Chessformer
*/

#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include "../include/libmy.h"
#include "../include/chessformer.h"

void map_check(chessformer_t chessformer)
{

	int player = 0;

	for (int i = 0; i < chessformer.lines; i++) {
		for (int j = 0; chessformer.map_save[i][j] != '\0'; j++) {

			check_tile(i, j, chessformer);
			player += count_player(i, j, chessformer);
		}
	}
	
}

int check_tile(int y, int x, chessformer_t chessformer)
{
	if (chessformer.map_save[y][x] != '@' && chessformer.map_save[y][x] != '$'
	&& chessformer.map_save[y][x] != '#' && chessformer.map_save[y][x] != ' '
	&& chessformer.map_save[y][x] != '\n'
    )
	{
		write(2, "Unknown read character in map\n", 26);
		exit(84);
	}
	return (0);
}

int count_player(int y, int x, chessformer_t chessformer)
{
	int i = 0;

	if (chessformer.map_save[y][x] == '@') {
		i++;
	}
	return (i);
}
