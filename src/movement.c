/*
** EPITECH PROJECT, 2017
** PSU_my_sokoban_2017
** File description:
** Function that make the player move
** Edited by Thomas Minuzzo 2024 - adapted to Chessformer
*/


#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include "../include/libmy.h"
#include "../include/chessformer.h"

//Tries to apply any legal queen or horse movement
chessformer_t move_location(chessformer_t chessformer, int letter, int number){
	bool move_is_legal = false;
	mvprintw(chessformer.lines+4,0,"                                                                          ");//Overwrite print above...

	int letterMap = letter - 'a'+1;//Offset from border
	int numberMap = number - '1'+1;//Offset from border

	if(!(letterMap >= 1 && letterMap < chessformer.num_chars_map/chessformer.lines-1)
		|| !(numberMap >= 1 && numberMap < chessformer.lines-1) ){
		mvprintw(chessformer.lines+4,0,"Illegal movement location: %c%c not on map", letter, number);
		return chessformer;
	}

	if(letterMap == chessformer.player_x && numberMap == chessformer.player_y){
		mvprintw(chessformer.lines+4,0,"Illegal movement location: Same as player");
		return chessformer;
	}
		if(chessformer.map[numberMap][letterMap] == '#'){
		mvprintw(chessformer.lines+4,0,"Illegal movement location: Cannot move onto a #");
		return chessformer;
	}

	int delta_x = letterMap - chessformer.player_x;
	int delta_y = numberMap - chessformer.player_y;

	if (delta_x == 0){ //Must be moving vertical
		mvprintw(chessformer.lines+4,0,"'Silly' movement location: Don't fight gravity");
		return chessformer;
	}

	//Check for horizontal okay
	if(delta_y == 0){
		if(delta_x>0){
			//Moving right
			for(int x = chessformer.player_x; x <= letterMap ; x++){
				if(chessformer.map[chessformer.player_y][x] == '#' || (x!= letterMap && chessformer.map[chessformer.player_y][x] == '$')){
					mvprintw(chessformer.lines+4,0,"Illegal movement location: # or $ in the way");
					return chessformer;
				}
			}
			move_is_legal = true;
		}
		else{
			//Moving left
			for(int x = chessformer.player_x; x >= letterMap ; x--){
				if(chessformer.map[chessformer.player_y][x] == '#' || (x!= letterMap && chessformer.map[chessformer.player_y][x] == '$')){
					mvprintw(chessformer.lines+4,0,"Illegal movement location: # or $ in the way");
					return chessformer;
				}
			}
			move_is_legal = true;
		}
	}
	//Check for diagonal okay
	// absoulte values of delta x and delta y should be the same...
	if(delta_x == delta_y){
		//Moving down and right or up and left
		if(delta_x>0){
			//down and right
			for(int places_moved = 1; places_moved <= delta_x ; places_moved++){
				if(chessformer.map[chessformer.player_y+places_moved][chessformer.player_x+places_moved] == '#' || (places_moved != delta_x && chessformer.map[chessformer.player_y+places_moved][chessformer.player_x+places_moved] == '$' )){
					mvprintw(chessformer.lines+4,0,"Illegal movement location: # or $ in the way");
					return chessformer;
				}
			}
		}
		else{
			//up and left
			for(int places_moved = 1; places_moved <= -delta_x ; places_moved++){
				if(chessformer.map[chessformer.player_y-places_moved][chessformer.player_x-places_moved] == '#' || (places_moved != -delta_x && chessformer.map[chessformer.player_y-places_moved][chessformer.player_x-places_moved] == '$')){
					mvprintw(chessformer.lines+4,0,"Illegal movement location: # or $ in the way");
					return chessformer;
				}
			}
		}
		move_is_legal = true;
	}
	else if(delta_x == -delta_y){
		//Moving down and left or up and right
		if(delta_x>0){
			//up and right
			for(int places_moved = 1; places_moved <= delta_x ; places_moved++){
				if(chessformer.map[chessformer.player_y-places_moved][chessformer.player_x+places_moved] == '#' || (places_moved != delta_x && chessformer.map[chessformer.player_y-places_moved][chessformer.player_x+places_moved] == '$')){
					mvprintw(chessformer.lines+4,0,"Illegal movement location: # or $ in the way");
					return chessformer;
				}
			}
		}
		else{
			//down and left
			for(int places_moved = 1; places_moved <= -delta_x ; places_moved++){
				if(chessformer.map[chessformer.player_y+places_moved][chessformer.player_x-places_moved] == '#' || (places_moved != -delta_x && chessformer.map[chessformer.player_y+places_moved][chessformer.player_x-places_moved] == '$')){
					mvprintw(chessformer.lines+4,0,"Illegal movement location: # or $ in the way");
					return chessformer;
				}
			}
		}
		move_is_legal = true;
	}
	
	//horse move
	if(   (delta_x == -2 && delta_y == -1)||(delta_x ==  2 && delta_y == -1)||(delta_x == -2 && delta_y ==  1)
		||(delta_x ==  2 && delta_y ==  1)||(delta_x == -1 && delta_y == -2)||(delta_x ==  1 && delta_y == -2)
		||(delta_x == -1 && delta_y ==  2)||(delta_x ==  1 && delta_y ==  2) ){
			if(chessformer.map[numberMap][letterMap]== '#'){
				mvprintw(chessformer.lines+4,0,"Illegal movement location: # in the way");
				return chessformer;
			}
			move_is_legal = true;
	}

	if(move_is_legal){
		mvprintw(chessformer.lines+4,0,"Moving to location: %c%c",letter,number);
		chessformer.map[numberMap][letterMap] = '@';
		chessformer.map[chessformer.player_y][chessformer.player_x] = ' ';
		chessformer.player_x = letterMap;
		chessformer.player_y = numberMap;
	}	
	else{
		mvprintw(chessformer.lines+4,0,"Not a valid chess move: %c%c",letter,number);
	}
	return chessformer;
}

chessformer_t move_down(chessformer_t chessformer)
{
	if (chessformer.map[chessformer.player_y+1][chessformer.player_x] != '#') {
		chessformer.map[chessformer.player_y+1][chessformer.player_x] = '@';
		chessformer.map[chessformer.player_y][chessformer.player_x] = ' ';
		chessformer.player_y++;
	}
	//chessformer = apply_gravity(chessformer);
	return (chessformer);
}



