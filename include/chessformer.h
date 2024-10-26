/*
** EPITECH PROJECT, 2017
** bsq.h
** File description:
** Contain all the prototypes needed for BSQ
*/

#ifndef BSQ_H
#define BSQ_H
	typedef struct chessformer {
		char *buffer;
		char **map; //A line by line map of chars representing the game state
		char **map_save;
		int lines; //The number of rows
		int player_x; //The player's current x location
		int player_y; //The player's current y location
		char const *base_path;
		char* soln; //To store the current moves made in this state
		int num_chars_map; //The number of chars total (divide by lines to get columns)
	} chessformer_t;
	int helper(void);
	char *read_map(int reading);
	char *open_map(char const *path);
	chessformer_t make_map(char const *path, chessformer_t chessformer);
	int play(char const *path);
	chessformer_t count_lines(chessformer_t chessformer);
	int count_columns(chessformer_t chessformer, int position);
	chessformer_t check_if_player(chessformer_t chessformer, int y, int x);
	chessformer_t find_player(chessformer_t chessformer);
	chessformer_t key_check(chessformer_t chessformer, int letter, int number);
	chessformer_t attempt_move(chessformer_t chessformer, int letter, int number);
	chessformer_t move_location(chessformer_t chessformer, int letter, int number);
	chessformer_t move_down(chessformer_t chessformer);
	void win_check(chessformer_t chessformer);
	void map_check(chessformer_t chessformer);
	int count_case_number(int y, int x, chessformer_t chessformer);
	int count_player(int y, int x, chessformer_t chessformer);
	chessformer_t game_management(chessformer_t chessformer);
	int check_tile(int y, int x, chessformer_t chessformer);
#endif
