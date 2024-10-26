#ifndef __NODE__
#define __NODE__

#include "utils.h"

/**
 * Data structure containing the node information
 */
struct node_s {
    // If using a priority queue, the priority for this item.
    // int priority;
    // The number of moves used (or other cost metric).
    int depth;
    // The number of active children (useful for memory management).
    int num_childs;
    // The movement in the x dimension of the move generating this state.
    int move_delta_x;
    // The movement in the y dimension of the move generating this state.
    int move_delta_y;
    state_t state;
    struct node_s* parent;
};

typedef struct node_s node_t;


#endif
