/*
 * global.h
 *
 *  Created on: Apr 17, 2017
 *      Author: nguyenbui
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

// Type define here
// Check development guide for complete info
typedef int Speed;
typedef int Distance;
typedef int Radius;
typedef int Angle;
typedef int Walls;
typedef int Cell_info;
typedef int Cell_label;


/*
 * Bit manipulation macro
 */
#define CLR_B(p,n) ((p) &= ~((1) << (n)))
#define SET_B(p,n) ((p) |= ((1) << (n)))
#define READ_B(p,n) ((p) & ((1) << (n)))

/*
 * Runner Define
 */
#define ROW_DEST 7
#define COLUMN_DEST 7
#define DIRECTION 8


/*
 * Driver Define
 */
#define M_PI 31416 //x10^(-4)
#define E_6	1000000 // 10^6
#define E_5	100000 // 10^5
#define E_4	10000 // 10^4
#define E_3	1000 // 10^3
#define E_2	100 // 10^2
#define CELL_WIDTH 180
#define HALF_CELL_WIDTH 90
#define HALF_CELL_WIDTH_WITH_ERR 50

/*
 * Using the new mice
 */
#define L_100MM2COUNT_RATIO 88  // calibrated on 05/19
#define R_100MM2COUNT_RATIO 88  //calibrated on 05/19
#define MOUSE_WIDTH 68	// calibrated on 05/19



/*
 * These setting are for the basic mouse board
 * #define L_100MM2COUNT_RATIO 1150  // 100 millimeters = 1110 count
 * #define R_100MM2COUNT_RATIO 1150  // 100 millimeters = 1110 count
 * #define MOUSE_WIDTH 83	// 65mm from left wheels to right wheels
 */
/*
 * Controller Define
 */
/*
 * WALL INFO
 * This depend on type of wall
 */
#define VISITED_BIT_POSITION 4
#define FRONTWALL_BIT_POSITION 0
#define LEFTWALL_BIT_POSITION 3
#define RIGHTWALL_BIT_POSITION 1
#define CENTER_TO_LEFT_WALL 856
#define CENTER_TO_RIGHT_WALL 883
#define CENTER_TO_FRONT_RIGHT 3241
#define CENTER_TO_FRONT_LEFT 2416

#define FRONT_LEFT_THRESHOLD 220
#define FRONT_RIGHT_THRESHOLD 230
#define DIAGNAL_LEFT_THRESHOLD 150
#define DIAGNAL_RIGHT_THRESHOLD 150

#define SENSOR_RATIO 25
#define SENSOR_NEGATIVE_RATIO 2
#define MAX_SENSOR_ERR 2
#define FRONT_WALL_CORRECTION_SPEED_LIMIT 10
#define FRONT_WALL_SENSOR_RATIO 5

/*
 * DIRECTIONS
 * This depends on which way the mouse is facing
 */
#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3
#define UNKNOWN_DIRECTION 5

/*
 * TURNS
 * This depends on which way the mouse is turning
 */
#define STRAIGHT 0
#define RIGHT 1
#define UTURN 2
#define LEFT 3


#define EXPLORE_RUNNING_SPEED 20
#define EXPLORE_TURNING_SPEED 20
#define RUNNING_SPEED 30
#define TURNING_SPEED 30


#define CURVE_TURN_DIA	90

#define EXPLORE_RIGHT_TURN_ANGLE 90
#define EXPLORE_LEFT_TURN_ANGLE 90

/*
 * UI mode
 *
 */
#define MODE_DEFAULT 0
#define MODE_TEST_GO_STRAIGHT 1
#define MODE_TEST_SHARP_TURN 2
#define MODE_TEST_CURVE_TURN 3
#define MODE_EXPLORE 4
#define MODE_TEST_SENSORS 5
#define MODE_TEST_FLASH 6
#define MODE_TEST_SPEED_RUN 7
#define MODE_READ_FLASH 8
#define MODE_WRITE_FLASH 9
#define MODE_EXPLORE_WITHOUT_SAVE_MAZE 10

/*
 * WRITE DATA ONTO FLASH
 *
 */
#define MAZE_SIZE 16
#define MAZE_ADRESS 0x080C0000



/*
 * Extern Global variables
 */
volatile extern int mode;
volatile extern int FUNC_TERMINATED;
extern int x_coord;
extern int y_coord;
extern int curr_dir;
extern int turn;
extern int maze[16][16];
extern int maze_flood_fill[16][16];


struct Coord {
	unsigned char row;
	unsigned char col;
};


typedef struct Coord COORD;

/* Stack */
#define STACKSIZE 512
struct Stack {
	struct Coord stack[STACKSIZE];
	int top;
};
typedef struct Stack STACK;




#endif /* GLOBAL_H_ */
