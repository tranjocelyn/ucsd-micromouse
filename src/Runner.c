/*
 * Runner.c
 *
 *  Created on: Apr 19, EXPLORE_RUNNING_SPEED17
 *      Author: nguyenbui
 */

#include "stdlib.h"
#include "delay.h"
#include "Driver.h"
#include "global.h"


int x_coord = 0;
int y_coord = 0;
int curr_dir = NORTH;
int turn = STRAIGHT;
int maze[16][16];
int maze_flood_fill[16][16];
STACK s = { .stack = {{0}}, .top = 0 };


/* Initialize the maze with distances from target */
void Runner_maze_init(int x_target, int y_target){
	for (int x = 0; x< 16; x++){
		for (int y = 0; y< 16; y++){
			maze_flood_fill[x][y] = abs(x-x_target) + abs(y-y_target);
		}
	}
	maze[x_target][x_target] &= !(1<<VISITED_BIT_POSITION);
}

/* Update the the distances of the maze with new wall info */
void Runner_flood_fill(){
	for (int i = 0; i<256; i++)
		for (int x = 0; x< 16; x++)
			for (int y = 0; y< 16; y++){
				if ( maze[x][y]&(1<<VISITED_BIT_POSITION) ){
					int smallest_neighbor = 256;
					if (!(maze[x][y] & (1 << NORTH))) {
						if (smallest_neighbor > maze_flood_fill[x][y+1]){
							smallest_neighbor = maze_flood_fill[x][y+1];
						}
					}

					if (!(maze[x][y] & (1 << EAST))) {
						if (smallest_neighbor > maze_flood_fill[x+1][y]){
								smallest_neighbor = maze_flood_fill[x+1][y];
						}
					}

					if (!(maze[x][y] & (1 << SOUTH))) {
						if (smallest_neighbor > maze_flood_fill[x][y-1]){
							smallest_neighbor = maze_flood_fill[x][y-1];
						}
					}

					if (!(maze[x][y] & (1 << WEST))) {
						if (smallest_neighbor > maze_flood_fill[x-1][y]){
							smallest_neighbor = maze_flood_fill[x-1][y];
						}
					}

					if (smallest_neighbor < 256){
						maze_flood_fill[x][y] = smallest_neighbor + 1;
					}

				}
			}
}


void Runner_speed_run(){

	for (int x = 0; x < 16; x++) {
		for (int y = 0; y < 16; y++) {
			if (!((maze[x][y] >> (DIRECTION + 2)) & 1)) {
				return;
			}
			int turn = (maze[x][y] >> DIRECTION) & 0x3;
			if (turn == STRAIGHT) {
				Driver_go_straight(CELL_WIDTH, RUNNING_SPEED);
				Driver_go_straight(0, 0);
			}
			else if (turn == LEFT) {
						Driver_go_straight(HALF_CELL_WIDTH, RUNNING_SPEED);
						Driver_turn_left(0, 90, TURNING_SPEED);
						Driver_go_straight(HALF_CELL_WIDTH, RUNNING_SPEED);
			}
			else if (turn == RIGHT) {
						Driver_go_straight(HALF_CELL_WIDTH, RUNNING_SPEED);
						Driver_turn_right(0, 90, TURNING_SPEED);
						Driver_go_straight(HALF_CELL_WIDTH, RUNNING_SPEED);
			}
		}
	}


}



void Runner_speed_curve_trun(){

	for (int x = 0; x < 16; x++) {
		for (int y = 0; y < 16; y++) {
			if (!((maze[x][y] >> (DIRECTION + 2)) & 1)) {
				return;
			}
			int turn = (maze[x][y] >> DIRECTION) & 0x3;
			if (turn == STRAIGHT) {
				Driver_go_straight(CELL_WIDTH, RUNNING_SPEED);
			}
			else if (turn == LEFT) {
						Driver_turn_left(90, 90, TURNING_SPEED);
			}
			else if (turn == RIGHT) {
						Driver_turn_right(90, 90, TURNING_SPEED);
			}
		}
	}


}




/*
// Update distances according to new wall info found at x and y
void Runner_update_distances(int x, int y) {

	// Make sure stack is empty
	s = { .stack = {{0}}, .top = 0 };

	// Push current cell into stack
	COORD curr = {.row = x, .col = y };
	push(&s, curr);


	// Push cells adjacent to walls onto stack
	if ((maze[x][y] & (1 << NORTH))) {
		COORD n = {.row = x, .col = y + 1 };
		push(&s, n);
	}

	if (!(maze[x][y] & (1 << EAST))) {
		COORD n = {.row = x + 1, .col = y };
		push(&s, n);
	}

	if (!(maze[x][y] & (1 << SOUTH))) {
		COORD n = {.row = x, .col = y - 1 };
		push(&s, n);
	}

	if (!(maze[x][y] & (1 << WEST))) {
		COORD n = {.row = x - 1, .col = y };
		push(&s, n);
	}

	// Update all cells in stack
	while(!isEmpty(&s)) {

		// Get cell from stack
		COORD curr = pop(&s);

		// Find smallest neighbor of current cell
		int smallest_neighbor = 256;
		int curr_x = curr.row;
		int curr_y = curr.col;
		int distN = (maze[curr_x][curr_y] & (1 << NORTH)) ? 256: maze_flood_fill[curr_x][curr_y + 1];
		int distE = (maze[curr_x][curr_y] & (1 << EAST)) ? 256: maze_flood_fill[curr_x + 1][curr_y];
		int distS = (maze[curr_x][curr_y] & (1 << SOUTH)) ? 256: maze_flood_fill[curr_x][curr_y - 1];
		int distW = (maze[curr_x][curr_y] & (1 << WEST)) ? 256: maze_flood_fill[curr_x - 1][curr_y];

		int min = distN;
		min = (distE < min) ? distE : min;
		min = (distS < min) ? distS : min;
		min = (distW < min) ? distW : min;

		// Don't need to update adjacent cells if curr cell = 1 + min neighbor
		if (maze_flood_fill[curr_x][curr_y] == min + 1) {
			continue;
		}

		// Other push open neighbors to stack
		if ((maze[curr_x][curr_y] & (1 << NORTH))) {
			COORD n = {.row = curr_x, .col = curr_y + 1 };
			push(&s, n);
		}

		if (!(maze[curr_x][curr_y] & (1 << EAST))) {
			COORD n = {.row = curr_x + 1, .col = curr_y };
			push(&s, n);
		}

		if (!(maze[curr_x][curr_y] & (1 << SOUTH))) {
			COORD n = {.row = curr_x, .col = curr_y - 1 };
			push(&s, n);
		}

		if (!(maze[curr_x][curr_y] & (1 << WEST))) {
			COORD n = {.row = curr_x - 1, .col = curr_y };
			push(&s, n);
		}

	}
}
*/


void Runner_random_turn() {
	 //Start from the middle of cell move half then read value
	int random_num;

	Driver_go_straight(HALF_CELL_WIDTH, EXPLORE_RUNNING_SPEED);
	y_coord++;

	while (1){
		/* CHECK IF USER SEND A SIGNAL TO TERMINATE CURRENT FUCTION */
		if (FUNC_TERMINATED){
			return;
		}

		int walls_info = Driver_checkwalls();

		random_num = 1;

		switch(random_num){
			case 0: // front -> left -> right
				if (!(walls_info & (1 << FRONTWALL_BIT_POSITION))) {
							turn = STRAIGHT;
							Driver_go_straight(CELL_WIDTH, EXPLORE_RUNNING_SPEED);
							Driver_go_straight(0, 0);
				}
				else if (!(walls_info & (1 << LEFTWALL_BIT_POSITION))) {
							turn = LEFT;
							Driver_go_straight(HALF_CELL_WIDTH, EXPLORE_RUNNING_SPEED);
							Controller_frontwall_corection();
							Driver_go_straight(0, 0);
							Driver_turn_left(0, 90, EXPLORE_TURNING_SPEED);
							Driver_go_straight(0, 0);
							Driver_go_straight(HALF_CELL_WIDTH, EXPLORE_RUNNING_SPEED);
							Driver_go_straight(0, 0);
				}
				else if (!(walls_info & (1 << RIGHTWALL_BIT_POSITION))) {
							turn = RIGHT;
							Driver_go_straight(HALF_CELL_WIDTH, EXPLORE_RUNNING_SPEED);
							Controller_frontwall_corection();
							Driver_go_straight(0, 0);
							Driver_turn_right(0, 90, EXPLORE_TURNING_SPEED);
							Driver_go_straight(0, 0);
							Driver_go_straight(HALF_CELL_WIDTH, EXPLORE_RUNNING_SPEED);
							Driver_go_straight(0, 0);
				}
				else {
							turn = UTURN;
							Driver_go_straight(HALF_CELL_WIDTH, EXPLORE_RUNNING_SPEED);
							Driver_go_straight(0, 0);
							Controller_frontwall_corection();
							Driver_go_straight(0, 0);
							Driver_turn_left(0, 90, EXPLORE_TURNING_SPEED);
							Driver_go_straight(0, 0);
							Controller_frontwall_corection();
							Driver_go_straight(0, 0);
							Driver_turn_left(0, 90, EXPLORE_TURNING_SPEED);
							Driver_go_straight(0, 0);
							Driver_go_straight(HALF_CELL_WIDTH, EXPLORE_RUNNING_SPEED);
							Driver_go_straight(0, 0);
				}
				break;

			case 1://left -> front -> right
				if (!(walls_info & (1 << LEFTWALL_BIT_POSITION))) {
						turn = LEFT;
						Driver_go_straight(HALF_CELL_WIDTH, EXPLORE_RUNNING_SPEED);
						Driver_go_straight(0, 0);
						Driver_turn_left(90, 90, EXPLORE_TURNING_SPEED);
											Driver_go_straight(0, 0);
						Driver_go_straight(HALF_CELL_WIDTH, EXPLORE_RUNNING_SPEED);
											Driver_go_straight(0, 0);
				}
				else if (!(walls_info & (1 << FRONTWALL_BIT_POSITION))) {
							turn = STRAIGHT;
							Driver_go_straight(CELL_WIDTH, EXPLORE_RUNNING_SPEED);
							Driver_go_straight(0, 0);
				}

				else if (!(walls_info & (1 << RIGHTWALL_BIT_POSITION))) {
							turn = RIGHT;
							Driver_go_straight(HALF_CELL_WIDTH, EXPLORE_RUNNING_SPEED);
							Controller_frontwall_corection();
							Driver_go_straight(0, 0);
							Driver_turn_right(0, 90, EXPLORE_TURNING_SPEED);
							Driver_go_straight(0, 0);
							Driver_go_straight(HALF_CELL_WIDTH, EXPLORE_RUNNING_SPEED);
							Driver_go_straight(0, 0);
				}

				else {
							turn = UTURN;
							Driver_go_straight(HALF_CELL_WIDTH, EXPLORE_RUNNING_SPEED);
							Driver_go_straight(0, 0);
							Controller_frontwall_corection();
							Driver_go_straight(0, 0);
							Driver_turn_left(0, 90, EXPLORE_TURNING_SPEED);
							Driver_go_straight(0, 0);
							Controller_frontwall_corection();
							Driver_go_straight(0, 0);
							Driver_turn_left(0, 90, EXPLORE_TURNING_SPEED);
							Driver_go_straight(0, 0);
							Driver_go_straight(HALF_CELL_WIDTH, EXPLORE_RUNNING_SPEED);
							Driver_go_straight(0, 0);
				}
				break;
			case 2: //right -> front -> left
				if (!(walls_info & (1 << RIGHTWALL_BIT_POSITION))) {
					turn = RIGHT;
					Driver_go_straight(HALF_CELL_WIDTH, EXPLORE_RUNNING_SPEED);
					Driver_go_straight(0, 0);
					Driver_turn_right(90, 90, EXPLORE_TURNING_SPEED);
					Driver_go_straight(0, 0);
					Driver_go_straight(HALF_CELL_WIDTH, EXPLORE_RUNNING_SPEED);
					Driver_go_straight(0, 0);
				}
				else if (!(walls_info & (1 << FRONTWALL_BIT_POSITION))) {
					turn = STRAIGHT;
					Driver_go_straight(CELL_WIDTH, EXPLORE_RUNNING_SPEED);
								Driver_go_straight(0, 0);
				}

				else if (!(walls_info & (1 << LEFTWALL_BIT_POSITION))) {
							turn = LEFT;
							Driver_go_straight(HALF_CELL_WIDTH, EXPLORE_RUNNING_SPEED);
							Controller_frontwall_corection();
							Driver_go_straight(0, 0);
							Driver_turn_left(0, 90, EXPLORE_TURNING_SPEED);
							Driver_go_straight(0, 0);
							Driver_go_straight(HALF_CELL_WIDTH, EXPLORE_RUNNING_SPEED);
							Driver_go_straight(0, 0);
				}

				else {
							turn = UTURN;
							Driver_go_straight(HALF_CELL_WIDTH, EXPLORE_RUNNING_SPEED);
							Driver_go_straight(0, 0);
							Controller_frontwall_corection();
							Driver_go_straight(0, 0);
							Driver_turn_left(0, 90, EXPLORE_TURNING_SPEED);
							Driver_go_straight(0, 0);
							Controller_frontwall_corection();
							Driver_go_straight(0, 0);
							Driver_turn_left(0, 90, EXPLORE_TURNING_SPEED);
							Driver_go_straight(0, 0);
							Driver_go_straight(HALF_CELL_WIDTH, EXPLORE_RUNNING_SPEED);
							Driver_go_straight(0, 0);
				}
				break;
		}

		if (turn == RIGHT) {
			curr_dir++;
			curr_dir%=4;
		}

		else if (turn == LEFT) {
			if(curr_dir == NORTH) {
				curr_dir = WEST;
			}
			else {
				curr_dir--;
			}
		}

		else if ( turn == UTURN) {
			curr_dir+=2;
			curr_dir%=4;
		}

		switch(curr_dir) {
		case NORTH:
			y_coord++;
			break;

		case EAST:
			x_coord++;
			break;

		case SOUTH:
			y_coord--;
			break;

		case WEST:
			x_coord--;
			break;
		}

		if(x_coord == 7 && y_coord == 7) {
			Driver_go_straight(0,0);
			return;
		}

	}

}



void Runner_random_turn_two() {
	 //Start from the middle of cell move half then read value
	int random_num;

	Driver_go_straight(HALF_CELL_WIDTH, EXPLORE_RUNNING_SPEED);
//	Driver_go_straight(0, 0);

	y_coord++;

	while (1){
		/* CHECK IF USER SEND A SIGNAL TO TERMINATE CURRENT FUCTION */
		if (FUNC_TERMINATED){
			return;
		}

		int walls_info = Driver_checkwalls();


		// Update wall info
		//define walls to be SWEN (last 4 positions)
		int n, e, s, w;
		switch(curr_dir){
		case NORTH:
			n = (walls_info >> FRONTWALL_BIT_POSITION) & 1;
			e = (walls_info >> RIGHTWALL_BIT_POSITION)  & 1;
			w = (walls_info >> LEFTWALL_BIT_POSITION) & 1;
			s = 0;
			break;
		case EAST:
			e = (walls_info >> FRONTWALL_BIT_POSITION) & 1;
			s = (walls_info >> RIGHTWALL_BIT_POSITION)  & 1;
			n = (walls_info >> LEFTWALL_BIT_POSITION) & 1;
			w = 0;
			break;
		case SOUTH:
			s = (walls_info >> FRONTWALL_BIT_POSITION) & 1;
			w = (walls_info >> RIGHTWALL_BIT_POSITION)  & 1;
			e = (walls_info >> LEFTWALL_BIT_POSITION) & 1;
			n = 0;
			break;
		case WEST:
			w = (walls_info >> FRONTWALL_BIT_POSITION) & 1;
			n = (walls_info >> RIGHTWALL_BIT_POSITION)  & 1;
			s = (walls_info >> LEFTWALL_BIT_POSITION) & 1;
			e = 0;
			break;
		}
		int curr = 0;
		curr = curr | (n << NORTH);
		curr = curr | (e << EAST);
		curr = curr | (s << SOUTH);
		curr = curr | (w << WEST);
		maze[y_coord][x_coord]&= 0xFFFFFFF0;
		maze[y_coord][x_coord]|= curr;


		// Make the turn

		random_num = 2;

		switch(random_num){
			case 0: // front -> left -> right
				if (!(walls_info & (1 << FRONTWALL_BIT_POSITION))) {
							turn = STRAIGHT;
							Driver_go_straight(CELL_WIDTH, EXPLORE_RUNNING_SPEED);
							Driver_go_straight(0, 0);
				}
				else if (!(walls_info & (1 << LEFTWALL_BIT_POSITION))) {
							turn = LEFT;
							Driver_go_straight(HALF_CELL_WIDTH, EXPLORE_RUNNING_SPEED);
							Controller_frontwall_corection();
							Driver_go_straight(0, 0);
							Driver_turn_left(0, 90, EXPLORE_TURNING_SPEED);
							Driver_go_straight(0, 0);
							Driver_go_straight(HALF_CELL_WIDTH, EXPLORE_RUNNING_SPEED);
							Driver_go_straight(0, 0);
				}
				else if (!(walls_info & (1 << RIGHTWALL_BIT_POSITION))) {
							turn = RIGHT;
							Driver_go_straight(HALF_CELL_WIDTH, EXPLORE_RUNNING_SPEED);
							Controller_frontwall_corection();
							Driver_go_straight(0, 0);
							Driver_turn_right(0, 90, EXPLORE_TURNING_SPEED);
							Driver_go_straight(0, 0);
							Driver_go_straight(HALF_CELL_WIDTH, EXPLORE_RUNNING_SPEED);
							Driver_go_straight(0, 0);
				}
				else {
							turn = UTURN;
							Driver_go_straight(HALF_CELL_WIDTH, EXPLORE_RUNNING_SPEED);
							Driver_go_straight(0, 0);
							Controller_frontwall_corection();
							Driver_go_straight(0, 0);
							Driver_turn_left(0, 90, EXPLORE_TURNING_SPEED);
							Driver_go_straight(0, 0);
							Controller_frontwall_corection();
							Driver_go_straight(0, 0);
							Driver_turn_left(0, 90, EXPLORE_TURNING_SPEED);
							Driver_go_straight(0, 0);
							Driver_go_straight(HALF_CELL_WIDTH, EXPLORE_RUNNING_SPEED);
							Driver_go_straight(0, 0);
				}
				break;

			case 1://left -> front -> right
				if (!(walls_info & (1 << LEFTWALL_BIT_POSITION))) {
						turn = LEFT;
						Driver_go_straight(HALF_CELL_WIDTH, EXPLORE_RUNNING_SPEED);
						Driver_go_straight(0, 0);
						Driver_turn_left(90, 90, EXPLORE_TURNING_SPEED);
											Driver_go_straight(0, 0);
						Driver_go_straight(HALF_CELL_WIDTH, EXPLORE_RUNNING_SPEED);
											Driver_go_straight(0, 0);
				}
				else if (!(walls_info & (1 << FRONTWALL_BIT_POSITION))) {
							turn = STRAIGHT;
							Driver_go_straight(CELL_WIDTH, EXPLORE_RUNNING_SPEED);
							Driver_go_straight(0, 0);
				}

				else if (!(walls_info & (1 << RIGHTWALL_BIT_POSITION))) {
							turn = RIGHT;
							Driver_go_straight(HALF_CELL_WIDTH, EXPLORE_RUNNING_SPEED);
							Controller_frontwall_corection();
							Driver_go_straight(0, 0);
							Driver_turn_right(0, 90, EXPLORE_TURNING_SPEED);
							Driver_go_straight(0, 0);
							Driver_go_straight(HALF_CELL_WIDTH, EXPLORE_RUNNING_SPEED);
							Driver_go_straight(0, 0);
				}

				else {
							turn = UTURN;
							Driver_go_straight(HALF_CELL_WIDTH, EXPLORE_RUNNING_SPEED);
							Driver_go_straight(0, 0);
							Controller_frontwall_corection();
							Driver_go_straight(0, 0);
							Driver_turn_left(0, 90, EXPLORE_TURNING_SPEED);
							Driver_go_straight(0, 0);
							Controller_frontwall_corection();
							Driver_go_straight(0, 0);
							Driver_turn_left(0, 90, EXPLORE_TURNING_SPEED);
							Driver_go_straight(0, 0);
							Driver_go_straight(HALF_CELL_WIDTH, EXPLORE_RUNNING_SPEED);
							Driver_go_straight(0, 0);
				}
				break;
			case 2:// right -> front -> left
				if (!(walls_info & (1 << RIGHTWALL_BIT_POSITION))) {
					turn = RIGHT;
					Driver_go_straight(HALF_CELL_WIDTH, EXPLORE_RUNNING_SPEED);
					Driver_go_straight(0, 0);
					Driver_turn_right(90, 90, EXPLORE_TURNING_SPEED);
					Driver_go_straight(0, 0);
					Driver_go_straight(HALF_CELL_WIDTH, EXPLORE_RUNNING_SPEED);
					Driver_go_straight(0, 0);
				}
				else if (!(walls_info & (1 << FRONTWALL_BIT_POSITION))) {
					turn = STRAIGHT;
					Driver_go_straight(CELL_WIDTH, EXPLORE_RUNNING_SPEED);
								Driver_go_straight(0, 0);
				}

				else if (!(walls_info & (1 << LEFTWALL_BIT_POSITION))) {
							turn = LEFT;
							Driver_go_straight(HALF_CELL_WIDTH, EXPLORE_RUNNING_SPEED);
							Controller_frontwall_corection();
							Driver_go_straight(0, 0);
							Driver_turn_left(0, 90, EXPLORE_TURNING_SPEED);
							Driver_go_straight(0, 0);
							Driver_go_straight(HALF_CELL_WIDTH, EXPLORE_RUNNING_SPEED);
							Driver_go_straight(0, 0);
				}

				else {
							turn = UTURN;
							Driver_go_straight(HALF_CELL_WIDTH, EXPLORE_RUNNING_SPEED);
							Driver_go_straight(0, 0);
							Controller_frontwall_corection();
							Driver_go_straight(0, 0);
							Driver_turn_left(0, 90, EXPLORE_TURNING_SPEED);
							Driver_go_straight(0, 0);
							Controller_frontwall_corection();
							Driver_go_straight(0, 0);
							Driver_turn_left(0, 90, EXPLORE_TURNING_SPEED);
							Driver_go_straight(0, 0);
							Driver_go_straight(HALF_CELL_WIDTH, EXPLORE_RUNNING_SPEED);
							Driver_go_straight(0, 0);
				}
				break;
		}

		if (turn == RIGHT) {
			curr_dir++;
			curr_dir%=4;
		}

		else if (turn == LEFT) {
			if(curr_dir == NORTH) {
				curr_dir = WEST;
			}
			else {
				curr_dir--;
			}
		}

		else if ( turn == UTURN) {
			curr_dir+=2;
			curr_dir%=4;
		}

		switch(curr_dir) {
		case NORTH:
			y_coord++;
			break;

		case EAST:
			x_coord++;
			break;

		case SOUTH:
			y_coord--;
			break;

		case WEST:
			x_coord--;
			break;
		}

		if(x_coord == 2 && y_coord == 2) {
			Driver_go_straight(0,0);
			return;
		}



	}
}


void Runner_explore(int x_target, int y_target) {
	 //Start from the middle of cell move half then read value


	Runner_maze_init (x_target,y_target);


	Driver_go_straight(HALF_CELL_WIDTH, EXPLORE_RUNNING_SPEED);
//	Driver_go_straight(0, 0);

	/* For first cell - always run forward */
	if (x_coord == 0 && y_coord == 0){
		maze[0][0]&= 0xFFFFFFE0;
		maze[0][0]|= 0x1E;
	}
	switch(curr_dir) {
	case NORTH:
		y_coord++;
		break;

	case EAST:
		x_coord++;
		break;

	case SOUTH:
		y_coord--;
		break;

	case WEST:
		x_coord--;
		break;
	}

	while (1){

		/* CHECK IF USER SEND A SIGNAL TO TERMINATE CURRENT FUCTION */
		if (FUNC_TERMINATED){
			return;
		}

		int walls_info = Driver_checkwalls();

		int cur_dst, next_direction ;


		// Update wall info
		//define walls to be SWEN (last 4 positions)
		int n, e, s, w, f, l, r;
		switch(curr_dir){
		case NORTH:
			n = f = (walls_info >> FRONTWALL_BIT_POSITION) & 1;
			e = r = (walls_info >> RIGHTWALL_BIT_POSITION)  & 1;
			w = l = (walls_info >> LEFTWALL_BIT_POSITION) & 1;
			s = 0;
			break;
		case EAST:
			e = f = (walls_info >> FRONTWALL_BIT_POSITION) & 1;
			s = r = (walls_info >> RIGHTWALL_BIT_POSITION)  & 1;
			n = l = (walls_info >> LEFTWALL_BIT_POSITION) & 1;
			w = 0;
			break;
		case SOUTH:
			s = f = (walls_info >> FRONTWALL_BIT_POSITION) & 1;
			w = r = (walls_info >> RIGHTWALL_BIT_POSITION)  & 1;
			e = l = (walls_info >> LEFTWALL_BIT_POSITION) & 1;
			n = 0;
			break;
		case WEST:
			w = f = (walls_info >> FRONTWALL_BIT_POSITION) & 1;
			n = r = (walls_info >> RIGHTWALL_BIT_POSITION)  & 1;
			s = l = (walls_info >> LEFTWALL_BIT_POSITION) & 1;
			e = 0;
			break;
		}
		int curr = 0;
		curr = curr | (n << NORTH);
		curr = curr | (e << EAST);
		curr = curr | (s << SOUTH);
		curr = curr | (w << WEST);
		maze[x_coord][y_coord]&= 0xFFFFFFF0;
		maze[x_coord][y_coord]|= curr;

		SET_B(maze[x_coord][y_coord], VISITED_BIT_POSITION);



		/*************************/

		/*
		 * After saze the current cell info, check if current cell is target or not
		 */
		/* Check if we reach the target - if yes rotate back to be ready for new run*/
		if(x_coord == x_target && y_coord == y_target) {
			Driver_go_straight(HALF_CELL_WIDTH_WITH_ERR, EXPLORE_RUNNING_SPEED);
			Driver_turn_left(0, EXPLORE_LEFT_TURN_ANGLE, EXPLORE_TURNING_SPEED);
//			Driver_go_straight(0, 0);
			Driver_turn_left(0, EXPLORE_LEFT_TURN_ANGLE, EXPLORE_TURNING_SPEED);
			Driver_go_straight(0, 0);

			/* Update direction before return */
			curr_dir+=2;
			curr_dir%=4;
			/* Return to main */
			return;
		}





		/*
		 * If target is not reached - Flood fill and run
		 */
		Runner_flood_fill();

		/* Run either update distances of flood_fill */
		/* Using Stack */
		/* Runner_update_distances(x_coord, y_coord, curr);*/

		/*
		 * Check distance of open neighbors
		 */
		cur_dst = maze_flood_fill[x_coord][y_coord];

		next_direction = UNKNOWN_DIRECTION;
		/* Check NORTH neighbor */
		if (!n){
			if (maze_flood_fill[x_coord][y_coord+1] == cur_dst - 1){
				next_direction = NORTH;
			}
		}
		/* Check EAST neighbor */
		if (!e){
			if (maze_flood_fill[x_coord+1][y_coord] == cur_dst - 1){
				next_direction = EAST;
			}
		}
		/* Check SOUTH neighbor */
		if (!s){
			if (maze_flood_fill[x_coord][y_coord-1] == cur_dst - 1){
				next_direction = SOUTH;
			}
		}
		/* Check WEST neighbor */
		if (!w){
			if (maze_flood_fill[x_coord-1][y_coord] == cur_dst - 1){
				next_direction = WEST;
			}
		}

		/*
		 * Decide where to turn
		 */
		if (next_direction == UNKNOWN_DIRECTION){
			/*
			 * ERORR_________
			 */
			int error_count = 0;
			while (1){
				if (error_count < 10) {
					Driver_turn_right(0, EXPLORE_RIGHT_TURN_ANGLE, EXPLORE_TURNING_SPEED);
					error_count ++;
				} else {
					Driver_go_straight(0, 0);
				}

				/* CHECK IF USER SEND A SIGNAL TO TERMINATE CURRENT FUCTION */
				if (FUNC_TERMINATED){
					return;
				}

			}
		} else {
			if (next_direction == curr_dir)
				turn = STRAIGHT;
			else if (next_direction == (curr_dir + 1)%4)
				turn = RIGHT;
			else if (next_direction == (curr_dir + 3)%4)
				turn = LEFT;
			else if (next_direction == (curr_dir + 2)%4)
				turn = UTURN;
		}





		if (turn == RIGHT) {
			/* If there is front wall use front wall to correct error */
			Driver_go_straight(HALF_CELL_WIDTH_WITH_ERR, EXPLORE_RUNNING_SPEED);
			if (f){
				Controller_frontwall_corection();
			}
//			Driver_go_straight(0, 0);
			Driver_turn_right(0, EXPLORE_RIGHT_TURN_ANGLE, EXPLORE_TURNING_SPEED);
//			Driver_go_straight(0, 0);
			Driver_go_straight(HALF_CELL_WIDTH, EXPLORE_RUNNING_SPEED);
//			Driver_go_straight(0, 0);


		}

		else if (turn == LEFT) {
			/* If there is front wall use front wall to correct error */

				Driver_go_straight(HALF_CELL_WIDTH_WITH_ERR, EXPLORE_RUNNING_SPEED);
				if (f){
					Controller_frontwall_corection();
				}
//				Driver_go_straight(0, 0);
				Driver_turn_left(0, EXPLORE_LEFT_TURN_ANGLE, EXPLORE_TURNING_SPEED);
//				Driver_go_straight(0, 0);
				Driver_go_straight(HALF_CELL_WIDTH, EXPLORE_RUNNING_SPEED);
//				Driver_go_straight(0, 0);

		}

		else if ( turn == UTURN) {
			/* GO straight half cell and use front wall to correct err if possible */
			Driver_go_straight(HALF_CELL_WIDTH_WITH_ERR, EXPLORE_RUNNING_SPEED);
			if (f){
				Controller_frontwall_corection();
			}
//			Driver_go_straight(0, 0);

			/* Make either left or right turn with wall correction */
			if (l){
				Driver_turn_left(0, EXPLORE_LEFT_TURN_ANGLE, EXPLORE_TURNING_SPEED);
//				Driver_go_straight(0, 0);
				Controller_frontwall_corection();
//				Driver_go_straight(0, 0);
				Driver_turn_left(0, EXPLORE_LEFT_TURN_ANGLE, EXPLORE_TURNING_SPEED);
//				Driver_go_straight(0, 0);
				Driver_go_straight(HALF_CELL_WIDTH, EXPLORE_RUNNING_SPEED);
//				Driver_go_straight(0, 0);
			} else if (r){
				Driver_turn_right(0, EXPLORE_RIGHT_TURN_ANGLE, EXPLORE_TURNING_SPEED);
//				Driver_go_straight(0, 0);
				Controller_frontwall_corection();
//				Driver_go_straight(0, 0);
				Driver_turn_right(0, EXPLORE_RIGHT_TURN_ANGLE, EXPLORE_TURNING_SPEED);
//				Driver_go_straight(0, 0);
				Driver_go_straight(HALF_CELL_WIDTH, EXPLORE_RUNNING_SPEED);
//				Driver_go_straight(0, 0);
			} else {
				Driver_turn_left(0, EXPLORE_LEFT_TURN_ANGLE, EXPLORE_TURNING_SPEED);
//				Driver_go_straight(0, 0);
				Driver_turn_left(0, EXPLORE_LEFT_TURN_ANGLE, EXPLORE_TURNING_SPEED);
//				Driver_go_straight(0, 0);
				Driver_go_straight(HALF_CELL_WIDTH, EXPLORE_RUNNING_SPEED);
//				Driver_go_straight(0, 0);
			}

		} else {
			Driver_go_straight(HALF_CELL_WIDTH+HALF_CELL_WIDTH_WITH_ERR, EXPLORE_RUNNING_SPEED);
//			Driver_go_straight(0, 0);
		}



		curr_dir = next_direction;

		switch(curr_dir) {
		case NORTH:
			y_coord++;
			break;

		case EAST:
			x_coord++;
			break;

		case SOUTH:
			y_coord--;
			break;

		case WEST:
			x_coord--;
			break;
		}


	}
}



void Runner_find_directions(int x_target, int y_target) {
	 //Start from the middle of cell move half then read value

	int x=0,y=0;

	Runner_maze_init (x_target,y_target);


	maze[x][y] = (maze[x][y]&0xFFFF0FF) | (STRAIGHT << DIRECTION);
	//this marks that this is on the path
	maze[x][y] = maze[x][y] | (1 << (DIRECTION + 2));
	y = (y + 1) % 16;
	if (y == 0) {
		x++;
	}


	/* For first cell - always run forward */
	if (x_coord == 0 && y_coord == 0){
		maze[0][0]&= 0xFFFFFFE0;
		maze[0][0]|= 0x1E;
	}
	switch(curr_dir) {
	case NORTH:
		y_coord++;
		break;

	case EAST:
		x_coord++;
		break;

	case SOUTH:
		y_coord--;
		break;

	case WEST:
		x_coord--;
		break;
	}


	/*
	 * If target is not reached - Flood fill and run
	 */
	Runner_flood_fill();

	while (1){

		/* CHECK IF USER SEND A SIGNAL TO TERMINATE CURRENT FUCTION */
		if (FUNC_TERMINATED){
			return;
		}


		int cur_dst, next_direction ;
		/*************************/

		/*
		 * After saze the current cell info, check if current cell is target or not
		 */
		/* Check if we reach the target - if yes rotate back to be ready for new run*/
		if(x_coord == x_target && y_coord == y_target) {
			maze[x][y] = (maze[x][y]&0xFFFF0FF);
			return;
		}







		/* Run either update distances of flood_fill */
		/* Using Stack */
		/* Runner_update_distances(x_coord, y_coord, curr);*/

		/*
		 * Check distance of open neighbors
		 */
		cur_dst = maze_flood_fill[x_coord][y_coord];

		next_direction = UNKNOWN_DIRECTION;

		int n, e, s, w;
		n = (maze[x_coord][y_coord] >> NORTH) & 1;
		e = (maze[x_coord][y_coord] >> EAST)  & 1;
		s = (maze[x_coord][y_coord] >> SOUTH) & 1;
		w = (maze[x_coord][y_coord] >> WEST) & 1;
		/* Check NORTH neighbor */
		if (!n){
			if ((maze_flood_fill[x_coord][y_coord+1] == cur_dst - 1) && ((maze[x_coord][y_coord+1] >> VISITED_BIT_POSITION) & 1)){
				next_direction = NORTH;
			}
		}
		/* Check EAST neighbor */
		if (!e){
			if ((maze_flood_fill[x_coord+1][y_coord] == cur_dst - 1) && ((maze[x_coord+1][y_coord] >> VISITED_BIT_POSITION) & 1)){
				next_direction = EAST;
			}
		}
		/* Check SOUTH neighbor */
		if (!s){
			if ((maze_flood_fill[x_coord][y_coord-1] == cur_dst - 1) && ((maze[x_coord][y_coord-1] >> VISITED_BIT_POSITION) & 1)){
				next_direction = SOUTH;
			}
		}
		/* Check WEST neighbor */
		if (!w){
			if ((maze_flood_fill[x_coord-1][y_coord] == cur_dst - 1) && ((maze[x_coord-1][y_coord] >> VISITED_BIT_POSITION) & 1)){
				next_direction = WEST;
			}
		}

		/*
		 * Decide where to turn
		 */
		if (next_direction == UNKNOWN_DIRECTION){
			/*
			 * ERORR_________
			 */
			int error_count = 0;
			while (1){
				if (error_count < 10) {
					Driver_turn_right(0, EXPLORE_RIGHT_TURN_ANGLE, EXPLORE_TURNING_SPEED);
					error_count ++;
				} else {
					Driver_go_straight(0, 0);
				}

				/* CHECK IF USER SEND A SIGNAL TO TERMINATE CURRENT FUCTION */
				if (FUNC_TERMINATED){
					return;
				}

			}
		} else {
			if (next_direction == curr_dir)
				turn = STRAIGHT;
			else if (next_direction == (curr_dir + 1)%4)
				turn = RIGHT;
			else if (next_direction == (curr_dir + 3)%4)
				turn = LEFT;
			else if (next_direction == (curr_dir + 2)%4)
				turn = UTURN;
		}





		maze[x][y] = (maze[x][y]&0xFFFF0FF) | (turn << DIRECTION);
		//this marks that this is on the path
		maze[x][y] = maze[x][y] | (1 << (DIRECTION + 2));
		y = (y + 1) % 16;
		if (y == 0) {
			x++;
		}


		curr_dir = next_direction;

		switch(curr_dir) {
		case NORTH:
			y_coord++;
			break;

		case EAST:
			x_coord++;
			break;

		case SOUTH:
			y_coord--;
			break;

		case WEST:
			x_coord--;
			break;
		}


	}
}

