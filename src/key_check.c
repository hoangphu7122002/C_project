/*
** EPITECH PROJECT, 2017
** PSU_my_sokoban_2017
** File description:
** Function that manage key press for sokoban
** Edited by Thomas Minuzzo 2024 - adapted to Chessformer
*/

#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include "../include/libmy.h"
#include "../include/chessformer.h"

chessformer_t key_check(chessformer_t chessformer, int letter, int number)
{
	chessformer = move_location(chessformer, letter, number);
	//animated gravity in play()
	return (chessformer);
}

chessformer_t attempt_move(chessformer_t chessformer, int letter, int number)
{
	chessformer = move_location(chessformer, letter, number);
	//Apply gravity
	while(chessformer.map[chessformer.player_y+1][chessformer.player_x] != '#'){
		chessformer = move_down(chessformer);
	}
	return (chessformer);
}
