#include <time.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <assert.h>
#include <stdbool.h>


#include "ai.h"
#include "utils.h"
#include "hashtable.h"
#include "chessformer.h"
#include "node.h"
#include "queue.h"

node_t *create_init_node(chessformer_t* chessformer) {
	node_t *n = (node_t *) malloc(sizeof(node_t));
	assert(n);

	n->depth = 0;
	n->num_childs = 0;
	n->move_delta_x = 0;
	n->move_delta_y = 0;
	n->state.map = (char **) malloc(sizeof(char *) * chessformer->lines);
	assert(n->state.map);
	int mapLength = chessformer->num_chars_map / chessformer->lines;
	for(int i = 0; i < chessformer->lines; i++){
		n->state.map[i] = (char *) malloc(sizeof(char) * (mapLength + 1));
		assert(n->state.map[i]);
		for(int j = 0; j < mapLength; j++){
			n->state.map[i][j] = chessformer->map[i][j];
		}
		n->state.map[i][mapLength] = '\0';
	}

	n->state.player_x = chessformer->player_x;
	n->state.player_y = chessformer->player_y;

	n->parent = NULL;

	return n;
}

/**
 * Copy a src into a dst state
*/
void copy_state(chessformer_t* init_data, state_t* dst, state_t* src){
	dst->map = malloc(sizeof(char *) * init_data->lines);
	assert(dst->map);
	for (int i = 0; i < init_data->lines; i++){
		int width = strlen(src->map[i]) + 1;
		dst->map[i] = malloc(width);
		assert(dst->map[i]);
		memcpy(dst->map[i], src->map[i], width);
	}
	dst->player_x = src->player_x;
	dst->player_y = src->player_y;
}

node_t* create_node(chessformer_t* init_data, node_t* parent){
	node_t *new_n = (node_t *) malloc(sizeof(node_t));
	new_n->parent = parent;
	new_n->depth = parent->depth + 1;
	copy_state(init_data, &(new_n->state), &(parent->state));
	return new_n;
}

/**
 * Apply an action to node n, create a new node resulting from 
 * executing the action, and return if the player moved
*/
bool applyAction(chessformer_t *init_data, node_t *n, node_t **new_node, int move_delta_x, int move_delta_y){
	bool player_moved = false;

    *new_node = create_node(init_data, n);
    (*new_node)->move_delta_x = move_delta_x;
	(*new_node)->move_delta_y = move_delta_y;

    player_moved = execute_move(init_data, &((*new_node)->state), move_delta_x, move_delta_y);

	return player_moved;
}

void free_state(chessformer_t *init_data, node_t* n){
	//******************* PHU CODE********************
	//Fill in
	/*
	Hint:
	Free all of:
	dynamically allocated map strings
	dynamically allocated map string pointers
	solution string
	state
	*/
	for (int i = 0; i < init_data->lines; i++) {
        free(n->state.map[i]);
    }
    free(n->state.map);
	//******************* PHU CODE********************
}

/**
 * Given a 2D map, returns a 1D map
*/
void flatten_map(chessformer_t* init_data, char **dst_map, char **src_map){

	int current_i = 0;
	for (int i = 0; i < init_data->lines; ++i)
	{
		int width = strlen(src_map[i]);
		for (int j = 0; j < width; j++){
			(*dst_map)[current_i] = src_map[i][j];
			current_i++;
		}
	}
}

bool winning_state(chessformer_t chessformer){
	for (int i = 0; i < chessformer.lines; i++) {
		for (int j = 0; j < chessformer.num_chars_map/chessformer.lines; j++) {
			if (chessformer.map[i][j] == '$') {
				return false;
			}
		}
	}
	return true;
}

char *saveSolution(node_t *finalNode) {
	int hierarchyCount = 0;
	node_t *current = finalNode;
	while(current){
		hierarchyCount++;
		current = current->parent;
	}
	char *soln = (char *) malloc(sizeof(char) * hierarchyCount * 2 + 1);
	assert(soln);
	
	current = finalNode;
	int left = hierarchyCount - 1;
	while(current){
		if(!current->parent){
			current = current->parent;
			continue;
		}
		left--;
		char x = current->parent->state.player_x + current->move_delta_x + '`';
		char y = current->parent->state.player_y + current->move_delta_y + '0';
		soln[2 * left] = x;
		soln[2 * left + 1] = y;
		current = current->parent;
	}

	soln[2 * (hierarchyCount - 1)] = '\0';

	return soln;
}

//******************* PHU CODE********************
int min(int a,int b) {
	if (a > b) return b;
	return a;
}

int max(int a,int b) {
	if (a > b) return a;
	return b;
}
//******************* PHU CODE********************

#define DEBUG 0

#include <assert.h>
/**
 * Find a solution by exploring all possible paths
 */
void find_solution(chessformer_t* init_data, bool show_solution){
	// Statistics variables
	// Keep track of solving time
	clock_t start = clock();
	unsigned int exploredNodes = 0;
	unsigned int generatedNodes = 0;
	unsigned int duplicatedNodes = 0;
	// Maximum depth limit so far - e.g. in Iterative Deepening
	// int max_depth = 0;
	unsigned solution_size = 0;

	// Solution String containing the sequence of moves
	char* solution = NULL;

	// Optimisation (Algorithm 2 setup)
	// HashTable hashTable;
	// Key size
	int htKeySize = sizeof(char) * init_data->num_chars_map;
	// Data size - same as key size as we do not do anything with data pair.
	int htDataSize = sizeof(char) * init_data->num_chars_map;
	// Hash table capacity - limit not too important, but 26 * 9 covers all
	//		single piece locations on one maximum size board.
	int htCapacity = 26 * 9;
	HashTable hashTable;  // HashTable for duplicate detection
	ht_setup(&hashTable, htKeySize, htDataSize, htCapacity);
	// Data structure to create a 1D representation of the map
	// Temporary variable to store key for hash table. We look at
	// the state of the map to decide whether we have seen the state
	// before - other ways of flattening the map
	// char *flat_map = calloc(init_data->num_chars_map, sizeof(char));
	// assert(flat_map);

	// Buffer for dequeue
	// chessformer_t current_state = *init_data; 
	// current_state.soln = "";

	// FILL IN
	// Algorithm 1 (or 2) from slide

	//******************* PHU CODE********************
	node_t* start_node = create_init_node(init_data);
	
	// Initialize Queue and add starter node in this queue
	queue_t queue;
	initialize_queue(&queue);
	enqueue(&queue, start_node);

	// ExploredTable to record all node generated, benefit: free and remove this node is easier
	node_t* exploredTable[10000];

	//Generated all possible move on knight and queen
	int dx[1000];
	int dy[1000];

	// Queen move possible
	int mvx[8] = {1, -1, 1, -1, 0, 1, -1, 0};
	int mvy[8] = {1, -1, -1, 1, 1, 0, 0, -1};

	int num_col = init_data->num_chars_map / init_data->lines;
	int num_row = init_data->lines;
	int num_move = 0;
	for (int i = 1; i < max(num_col,num_row); ++i) {
		for (int j = 0; j < 8; ++j) {
			dx[num_move] = mvx[j] * i;
			dy[num_move] = mvy[j] * i;
			num_move += 1;
		}
	}

	// Knight move possible
	int knight_delta_x[] = { -1,  1, -2,  2, -2,  2, -1,  1 };
	int knight_delta_y[] = { -2, -2, -1, -1,  1,  1,  2,  2 };
	for(int i = 0; i < 8; i++){
		dx[num_move] = knight_delta_x[i];
        dy[num_move] = knight_delta_y[i];
        num_move += 1;
	}

	// Continue processing nodes in the queue until it is empty
	while (!is_queue_empty(&queue)) {
		// Dequeue the front node from the queue for processing
		node_t *current_node = dequeue_node(&queue);
		
		// Store the dequeued node in the exploredTable to avoid re-processing
		exploredTable[exploredNodes] = current_node; 
		exploredNodes++;
		
		// Check if the current node meets the winning condition
		if (winning_condition(init_data, &current_node->state)) {
			// If winning, save the solution path and the depth of the solution
			solution = saveSolution(current_node);
			solution_size = current_node->depth;
			break;  // Stop processing as the solution is found
		}
		
		// Loop through each possible move direction
		for (int a = 0; a < num_move; ++a) {
			node_t *new_node;
			
			// Apply the move to create a new node; check if player moved successfully
			bool playerMoved = applyAction(init_data, current_node, &new_node, dx[a], dy[a]);
			generatedNodes++;
			
			// If player did not move, free the unused node and skip further processing
			if (!playerMoved) {
				free_state(init_data, new_node);  // Free memory for the state map
				free(new_node);  // Free memory for the new node structure
				continue;
			}

			// Create a flattened representation of the map for duplicate detection
			char *flat_map = calloc(init_data->num_chars_map, sizeof(char));
			flatten_map(init_data, &flat_map, new_node->state.map);

			// Check if this map state has already been encountered
			if (ht_contains(&hashTable, flat_map)) {
				duplicatedNodes++;
				free_state(init_data, new_node);  // Free state map memory for duplicates
				free(new_node);  // Free node structure for duplicates
				free(flat_map);  // Free the flattened map memory
				continue;
			}

			// Insert the unique map state into the hash table for future duplicate checks
			ht_insert(&hashTable, flat_map, flat_map);
			
			// Enqueue the new node for further exploration in the BFS
			enqueue(&queue, new_node);
			
			// Free the temporary flat_map memory as it's no longer needed
			free(flat_map);
		}
	}

	// End FILL IN

	// Stop clock
	clock_t end = clock();
	double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

	// Clear memory
	// FILL IN
	// Free memory for nodes stored in explored table and generated nodes still 
	// 		in queue. In Algorithm 2 - also free flat_map used as temporary 
	// 		variable for storing map.
	// Free all pointer in exploredTable
	for (unsigned int i = 0; i < exploredNodes; ++i) {
		if (exploredTable[i] != NULL) {
			free_state(init_data,exploredTable[i]);
			free(exploredTable[i]);
		}
	}
	// Free all pointer in queue
	while (!is_queue_empty(&queue)) {
		node_t *current_node = dequeue_node(&queue);
		free_state(init_data,current_node);
		free(current_node);
	}
	//******************* PHU CODE********************
	// In Algorithm 2, free hash table.
	ht_clear(&hashTable);
	ht_destroy(&hashTable);
	// End FILL IN

	// Report statistics.

	// Show Solution	
	if(show_solution && solution != NULL) {
		play_solution(*init_data, solution);
	}

	// endwin();
	if(solution != NULL){
		printf("\nSOLUTION:                               \n");
		printf( "%s\n\n", solution);
		FILE *fptr = fopen("solution.txt", "w");
		if (fptr == NULL) {
			printf("Could not open file");
			return ;
		}
		fprintf(fptr,"%s\n", solution);
		fclose(fptr);
		
		free(solution);
	}
	
	printf("STATS: \n");
	printf("\tExpanded nodes: %'d\n\tGenerated nodes: %'d\n\tDuplicated nodes: %'d\n", exploredNodes, generatedNodes, duplicatedNodes);
	printf("\tSolution Length: %d\n", solution_size);
	printf("\tExpanded/seconds: %d\n", (int)(exploredNodes/cpu_time_used));
	printf("\tTime (seconds): %f\n", cpu_time_used);
	
}



void solve(char const *path, bool show_solution)
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

	find_solution(&chessformer, show_solution);

	for(int i = 0; i < chessformer.lines; i++){
		free(chessformer.map[i]);
		free(chessformer.map_save[i]);
	}
	free(chessformer.map);
	free(chessformer.map_save);
	free(chessformer.buffer);

}