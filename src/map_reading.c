/*
** EPITECH PROJECT, 2017
** PSU_my_sokoban_2017
** File description:
** Main function for the my_sokoban
** Edited by Thomas Minuzzo 2024 - adapted to Chessformer
*/

#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include "../include/libmy.h"
#include "../include/chessformer.h"

char *open_map(char const *path)
{
	int reading;
	char *buffer;

	reading = open(path, O_RDONLY);
	if (reading == -1) {
		write(2, "No such file or directory\n", 26);
		exit (84);
	}
	buffer = read_map(reading);
	close(reading);
	return (buffer);
}

char *read_map(int reading)
{
	char *buffer = malloc(sizeof(char) * 10000);
	int size = 32;

	size = read(reading, buffer, 10000);
	if (size == -1)
		exit(84);
	buffer[size] = '\0';
	return (buffer);
}

int count_columns(chessformer_t chessformer, int position)
{
	int columns = 0;

	for (; chessformer.buffer[position] != '\n'; position++) {
		columns++;
	}
	return (columns);
}

chessformer_t count_lines(chessformer_t chessformer)
{
	chessformer.lines = 0;

	for (int i = 0; chessformer.buffer[i] != '\0'; i++) {
		if (chessformer.buffer[i] == '\n' || chessformer.buffer[i] == '\0')
			chessformer.lines++;
	}
	return (chessformer);
}

#define MAX_COLUMNS (26+2)
#define MAX_ROWS (9+2)
#include <assert.h>

chessformer_t make_map(char const *path, chessformer_t chessformer)
{
	chessformer.buffer = open_map(path);
	chessformer = count_lines(chessformer);
	int k = 0;
	int columns = 0;
	chessformer.num_chars_map = 0;
	chessformer.map = malloc(sizeof(char *) * chessformer.lines);
	chessformer.map_save = malloc(sizeof (char *) * chessformer.lines);
	for (int j = 0; j < chessformer.lines; j++) {
		columns = count_columns(chessformer, k);
		chessformer.num_chars_map += columns;
		chessformer.map[j] = malloc(sizeof(char) * columns + 1);
		chessformer.map_save[j] = malloc(sizeof(char) * columns + 1);
		for (int i = 0; i < columns; i++) {
			chessformer.map[j][i] = chessformer.buffer[k];
			chessformer.map_save[j][i] = chessformer.buffer[k];
			chessformer.map[j][i+1] = '\0';
			chessformer.map_save[j][i+1] = '\0';
			k++;
		}
		k++;
	}
	assert(columns<=MAX_COLUMNS);
	assert(chessformer.lines<=MAX_ROWS);
	return (chessformer);
}
