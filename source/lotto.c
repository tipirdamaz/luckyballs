/**
 * Turkey Sayisal and Super Lotto Statistics and Ball Draw Algorithms
 * Author: İbrahim Tıpırdamaz  <itipirdamaz@gmail.com>
 * Copyright 2022
*/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#if defined(__linux__) || defined(__unix__) || defined(__APPLE__) || defined(SOLARIS) || defined(WIN32)
#include <libgen.h>
#include <unistd.h>
#endif



// If you want to compile for sayisal lotto, you must #define SAYISALLOTTO at following line. Otherwise it will be compiled for super lotto.

//#define SAYISALLOTTO 1



#ifdef WIN32
#include <windows.h> /* GetModuleFileName */
#endif

#ifdef __APPLE__
#include <sys/param.h>
#include <mach-o/dyld.h> /* _NSGetExecutablePath : must add -framework
CoreFoundation to link line */
# ifndef PATH_MAX
# define PATH_MAX MAXPATHLEN
# endif
#endif

#ifndef PATH_MAX
# define PATH_MAX 2048
#endif


#if defined(__linux__) || defined(__unix__) || defined(__APPLE__) || defined(SOLARIS) || defined(WIN32)
#define PATH_SEPARATOR "/"
#else
#define PATH_SEPARATOR "\\"
#endif


#ifdef SAYISALLOTTO
	#define TOTAL_BALL 90			// total ball count
	#define TOTAL_BALL_SS 90		// total super star ball count
	#define FILESTATS "sayisal.txt"	// statistics file (winning numbers, superstar numbers)
#else
	#define TOTAL_BALL 60			// total ball count
	#define TOTAL_BALL_SS 0			// total super star ball count
	#define FILESTATS "super.txt"   // statistics file (winning numbers)
#endif


#define OUTPUTFILE "output.txt"     // file to write results
#define DRAW_BALL 6		// number of balls to be drawn



/* GLOBAL VARIABLES */


/* current working directory */
char *cwd = NULL;
char *fileStats = NULL;		// cwd + PATH_SEPARATOR + FILESTATS
char *outputFile = NULL;	// cwd + PATH_SEPARATOR + OUTPUTFILE



struct Item {			/* List item (ball) */
	struct Item *next;	// pointer to next item (ball) in list (draw)
	int key;			// ball number
	int val;			// other info such as how many times the ball has been drawn so far
};



struct ListX {			/* List (globe, drawn balls, statistics file row etc) */
	char *date;         // old drawn date (for statistics file rows)
    char *label;        // list label (for new drawns)
	int val;            // how many times the numbers that love each other drawn together (for lucky numbers)
	struct Item *head;  // first item in the list
	struct ListX *next; // next list (next row, if two dimensions)
};



struct ListXY {			/* 2 dimensions List. Multi draw (coupon or drawn balls from file has been drawn so far) */
	struct ListX *list; // list
};



/* Drawn balls lists from file has been drawn so far */
struct ListXY *winningDrawnBallsList = NULL;
struct ListXY *superStarDrawnBallsList = NULL;



/* Matched combinations of numbers from previous draws */
unsigned int match2comb = 0;
unsigned int match3comb = 0;
unsigned int match4comb = 0;
unsigned int match5comb = 0;
unsigned int match6comb = 0;



/* FUNCTION DEFINITIONS */


/**
 * Returns the difference between two dates in days
 * Date 1 : m1/d1/y1 (mm/dd/yyyy)
 * Date 2 : m2/d2/y2 (mm/dd/yyyy)
 * 
 * @param {Integer} d1  : day 1
 * @param {Integer} m1  : month 1
 * @param {Integer} y1  : year 1
 * @param {Integer} d2  : day 2
 * @param {Integer} m2  : month 2
 * @param {Integer} y2  : year 2
 * 
 * @return {Integer}    : Returns the difference between two dates in days
*/
int dateDiff(int d1, int m1, int y1, int d2, int m2, int y2);



/** 
 * Clear Screen
 * 
 */
void clearScreen();



/**
 * Get application full path
 *
 */
int get_app_path (char *pname, size_t pathsize);



/**
 * Create Empty List (1 dimension)
 * 
 * @param {struct ListX *} pl : refers to a ball list
 * @param {char *} date       : old drawn date (for statistics file rows, otherwise NULL)
 * @param {char *} label      : list label (for new drawns lists, otherwise NULL)
 * @param {Integer} val       : how many times the numbers that love each other drawn together (for lucky numbers list)
 * 
 * @return {struct ListX *}   : refers to the ball list (memory allocated)
 */
struct ListX *createListX(struct ListX *pl, char *date, char *label, int val);



/** 
 * Create Empty 2 dimensions List
 * 
 * @param {struct ListXY *}    : refers to a 2 dimensions ball list
 * @return {struct ListXY *}   : refers to the ball list (memory allocated)
 */
struct ListXY *createListXY(struct ListXY *pl);



/** 
 * Add an item (ball) to the beginning of the list
 * 
 * @param {struct ListX *}     : refers to a ball list
 * @param {Integer} key        : the key to which the ball will be added
 */
void insertItem(struct ListX *pl, int key);



/** 
 * Add an item (ball) to the end of the list
 * 
 * @param {struct ListX *}     : refers to a ball list
 * @param {Integer} key        : the key to which the ball will be added
 */
void appendItem(struct ListX *pl, int key);



/** 
 * Add a list to the end of the 2 dimensions list
 * 
 * @param {struct ListXY *}    : refers to a 2 dimensions ball list (multiple draws)
 * @param {struct ListX *}     : refers to a ball list
 */
void appendList(struct ListXY *pl, struct ListX *plNext);



/** 
 * Removes the specified item (ball) by key (ball number) from the list
 * 
 * @param {struct ListX *}     : refers to a ball list
 * @param {Integer} key        : the key to which the ball will be removed
 */
void removeItemByKey(struct ListX *pl, int key);



/** 
 * Removes item and returns the key (ball number) at the specified index from the list
 * 
 * @param {struct ListX *}     : refers to a ball list
 * @param {Integer} ind        : the index to which the ball will be removed
 * @return {Integer}           : returns key (ball number)
 */
int removeItemByIndex(struct ListX *pl, int ind);



/** 
 * If the specified key is not in the list, it adds to the specified index and returns 1
 * 
 * @param {struct ListX *}     : refers to a ball list
 * @param {Integer} ind        : the index to which the ball will be added
 * @param {Integer} key        : key (ball number)
 * @return {Integer}           : If the key is in the list, it does not add it to the list and return 0
 */
int addItemByIndex(struct ListX *pl, int ind, int key);



/** 
 * Remove all items in the list
 * 
 * @param {struct ListX *}     : refers to a ball list
 */
void removeAllX(struct ListX *pl);



/** 
 * Remove all list in the 2 dimensions list
 * 
 * @param {struct ListXY *}    : refers to a 2 dimensions ball list (multiple draws)
 */
void removeAllXY(struct ListXY *pl);



/** 
 * Return the number of items in the list
 * 
 * @param {struct ListX *}     : refers to a ball list
 * @return {Integer}           : Return the number of items in the list
 */
int length(struct ListX *pl);



/** 
 * Return the number of list in the 2 dimensions list
 * 
 * @param {struct ListXY *}    : refers to a 2 dimensions ball list (multiple draws)
 * @return {Integer}           : Return the number of list in the 2 dimensions list
 */
int lengthY(struct ListXY *pl);



/** 
 * Find the pointer of the item (ball) at the end of the list
 * 
 * @param {struct ListX *}    : refers to a ball list
 * @return {struct Item *}    : returns pointer of the item at the end of the list
 */
struct Item *atEnd(struct ListX *pl);



/** 
 * Find the pointer of the list at the end of the 2 dimensions list
 * 
 * @param {struct ListXY *}    : refers to a 2 dimensions ball list (multiple draws)
 * @return {struct ListX *}    : returns pointer of the list at the end of the 2 dimensions list
 */
struct ListX *atEndY(struct ListXY *pl);



/** 
 * Searches for a key (ball number) from the list.
 * 
 * @param {struct ListX *}     : refers to a ball list
 * @param {Integer} key        : key (ball number) to be search
 * @return {Integer}           : It returns the index of the number if it finds it, or -1 if it doesn't.
 */
int seqSearch(struct ListX *pl, int key);



/**
 * Searches for a key (ball number) from the 2 dimensions list.
 * 
 * @param {struct ListXY *}    : refers to a 2 dimensions ball list
 * @param {Integer} key        : key (ball number) to be search
 * @return {Integer}           : It returns the index of the number if it finds it, or -1 if it doesn't.
 *                               This is index of the first list containing the key in the 2 dimensions list
 */
int seqSearchXY1(struct ListXY *pl, int key);



/**
 * Searches for 2 key (ball number) from the 2 dimensions list.
 * 
 * @param {struct ListXY *}    : refers to a 2 dimensions ball list
 * @param {Integer} key1       : key (ball number) to be search
 * @param {Integer} key2       : key (ball number) to be search
 * @return {Integer}           : It returns the index of the numbers if it finds it, or -1 if it doesn't.
 *                               This is index of the first list containing the key in the 2 dimensions list
 */
int seqSearchXY2(struct ListXY *pl, int key1, int key2);



/**
 * Searches for 3 key (ball number) from the 2 dimensions list.
 * 
 * @param {struct ListXY *}    : refers to a 2 dimensions ball list
 * @param {Integer} key1       : key (ball number) to be search
 * @param {Integer} key2       : key (ball number) to be search
 * @param {Integer} key3       : key (ball number) to be search
 * @return {Integer}           : It returns the index of the numbers if it finds it, or -1 if it doesn't.
 *                               This is index of the first list containing the key in the 2 dimensions list
 */
int seqSearchXY3(struct ListXY *pl, int key1, int key2, int key3);



/**
 * Searches for 4 key (ball number) from the 2 dimensions list.
 * 
 * @param {struct ListXY *}    : refers to a 2 dimensions ball list
 * @param {Integer} key1       : key (ball number) to be search
 * @param {Integer} key2       : key (ball number) to be search
 * @param {Integer} key3       : key (ball number) to be search
 * @param {Integer} key4       : key (ball number) to be search
 * @return {Integer}           : It returns the index of the numbers if it finds it, or -1 if it doesn't.
 *                               This is index of the first list containing the key in the 2 dimensions list
 */
int seqSearchXY4(struct ListXY *pl, int key1, int key2, int key3, int key4);



/** 
 * Assigns the specified value to the item at the specified index
 * 
 * @param {struct ListX *}     : refers to a ball list (ballStats)
 * @param {Integer} ind        : index of an item (ball) in the ball list
 * @param {Integer} val        : Item value
 */
void chgVal(struct ListX *pl, int ind, int val);



/** 
 * Increments the value (the number of times the ball has drawn so far) of the item at the specified index by 1
 * 
 * @param {struct ListX *}     : refers to a ball list (ballStats)
 * @param {Integer} ind        : index of an item (ball) in the ball list
 */
void incVal(struct ListX *pl, int ind);



/** 
 * Increments the value (how many times the numbers that love each other drawn together) of the list at the specified index by 1
 * 
 * @param {struct ListXY *}    : refers to a 2 dimensions ball list (luckyBalls)
 * @param {Integer} ind        : index of a list in the 2 dimensions ball list
 */
void incValXY(struct ListXY *pl, int ind);



/** 
 * Returns the value of the item at the specified index
 * 
 * @param {struct ListX *}     : refers to a ball list
 * @param {Integer} ind        : index of an item (ball) in the ball list
 * @return {Integer}           : Returns the value of the item
 */
int getVal(struct ListX *pl, int ind);



/** 
 * Returns the value of the list at the specified index
 * 
 * @param {struct ListXY *}    : refers to a 2 dimensions ball list (luckyBalls)
 * @param {Integer} ind        : index of a list in the 2 dimensions ball list
 * @return {Integer}           : Returns the value of the list at the specified index
 */
int getValXY(struct ListXY *pl, int ind);



/** 
 * Returns the key (ball number) of the item at the specified index
 * 
 * @param {struct ListX *}     : refers to a ball list
 * @param {Integer} ind        : index of an item (ball) in the ball list
 * @return {Integer}           : Returns the key (ball number)
 */
int getKey(struct ListX *pl, int ind);



/** 
 * Returns the list in the 2 dimensions list at the specified index
 * 
 * @param {struct ListXY *}    : refers to a 2 dimensions ball list (multiple draws)
 * @param {Integer} ind        : index of 1 dimension list (a draw) in the 2 dimensions list (multi draws)
 * @return {struct ListX *}    : refers to a ball list at the specified index
 */
struct ListX * getListXByIndex(struct ListXY *pl, int ind);



/** 
 * Bubble sort by val the items (balls) in the ball list
 * 
 * @param {struct ListX *}    : refers to a ball list
 * @param {Integer} inc       : If inc=1, sort from smallest to greager. If inc=-1, sort from greater to smallest
 */
void bubbleSortXByVal(struct ListX *pl, int inc);



/** 
 * Bubble sort by val the lists in the 2 dimensions list
 * 
 * @param {struct ListXY *}   : refers to a 2 dimensions ball list
 * @param {Integer} inc       : If inc=1, sort from smallest to greager. If inc=-1, sort from greater to smallest
 */
void bubbleSortYByVal(struct ListXY *pl, int inc);



/** 
 * Bubble sort by key (ball number) from smallest to greager the items (ball) in the ball list
 * 
 * @param {struct ListX *}    : refers to a ball list
 */
void bubbleSortXByKey(struct ListX *pl);



/** 
 * Function to swap data of two item (ball) a and b
 * 
 * @param {struct Item *} a   : refers to a ball
 * @param {struct Item *} b   : refers to a ball
 */
void swapX(struct Item *a, struct Item *b);



/** 
 * Function to swap data of two list a and b
 * 
 * @param {struct ListX *} a   : refers to a ball list
 * @param {struct ListX *} b   : refers to a ball list
 */
void swapY(struct ListX *a, struct ListX *b);



/** 
 * Print keys (ball numbers) of the items in the list
 * 
 * @param {struct ListX *}    : refers to a ball list
 * @param {Integer} printTo   : 0: print to screen and output file. 1: print to screen only. 2: print to output file only.
 * @param {FILE *} fp         : refers to output file. If fp != NULL print to output file
 */
void printListXByKey(struct ListX *pl, int printTo, FILE *fp);



/** 
 * Print keys of the items (row by row) in the 2 dimensions list
 * 
 * @param {struct ListXY *}    : refers to 2 dimensions ball list
 * @param {FILE *} fp          : refers to output file. If fp != NULL print to output file
 */
void printListXYByKey(struct ListXY *pl, FILE *fp);



/** 
 * print ListXY With SuperStar ByKey
 * Print keys of the items (row by row) in the 2 dimensions lists
 * 
 * @param {struct ListXY *}    : refers to 2 dimensions ball list (winning numbers)
 * @param {struct ListXY *}    : refers to 2 dimensions ball list (superStar)
 * @param {FILE *} fp          : refers to output file. If fp != NULL print to output file
 */
void printListXYWithSSByKey(struct ListXY *pl1, struct ListXY *pl2, FILE *fp);



/** 
 * How many times the balls has been drawn so far
 * 
 * @param {struct ListX *} winningBallStats      : How many times were the winning numbers drawn in the previous draws?
 * @param {struct ListX *} superStarBallStats    : How many times were the superStar numbers drawn in the previous draws?
 */
void getDrawnBallsStats(struct ListX *winningBallStats, struct ListX *superStarBallStats);



/** 
 * Print key-val pair of the items in the list by ball statistics (How many times the balls has been drawn so far)
 * 
 * @param {struct ListX *} ballStats      : refers to balls and the number of times each ball was drawn in previous draws.
 */
void printBallStats(struct ListX *ballStats);



/**
 * print numbers (double combinations or triple combinations) that love each other (lucky balls)

 * @param {struct ListXY *}     : refers to 2 dimensions ball list (2*y or 3*y)
 * @param {FILE *} fp           : refers to output file. If fp != NULL print to output file
*/
void printLuckyBalls(struct ListXY *pl, FILE *fp);



/**
 * Return number of term

 * @param {Integer}     : sum of numbers from 1 to n
 * @return {Integer}    : n
*/
int numberOfTerm(int sum);



/** 
 * The numbers that came out in the previous draws (ballStats) are arranged at the base of the pascal's triangle and the ball is dropped on 
 * (with gaussIndex function) it and the ball hit is drawn. The gaussIndex function returns the index (random) of one of the balls arranged 
 * at the base of pascal's triangle. If a ball is dropped from the top node of Pascal's triangle, it drops to the left or right each time 
 * it hits the node (like tossing a coin). The gaussIndex function moves towards the base of the triangle, making a random selection at 
 * each node. When it reaches the base of the triangle, it returns the index whichever node (index) it hits. 
 * 
 * @param {Integer} ballCount      : Ball count in the globe
 * @return {Integer}               : Returns the random ball number (or ballStats index, you substract 1 from the return value for index)
 *                                   For example, ballCount = 90, it returns number between 1-90 (for index 0-89) (theoretically)
 *                                   Usually retuns the middle numbers
 */
int gaussIndex(int ballCount);



/** 
 * Draw based on today's date
 * 
 * @param {struct ListX *} ballStats      : refers to balls and the number of times each ball was drawn in previous draws.
 * @return {struct ListXY *}              : Returns 2 drawn list.
 */
struct ListXY * drawBallByDate(struct ListX *ballStats);



/** 
 * Draw random numbers
 * 
 * @param {struct ListX *} drawnBallsRand : refers to the balls to be drawn. 
 * @param {struct ListX *} ballStats      : refers to balls and the number of times each ball was drawn in previous draws.
 * @param {Integer} totalBall             : Total ball count in the globe
 * @param {Integer} drawBall        	  : Number of balls to be drawn
 * @param {Integer} matchComb             : Number of combinations in which the drawn numbers must match any of the previous draws.
 * @param {Integer} elimComb              : If a combination of the drawn numbers matched with any of the previous draws, specified by the
 *                                          elimComb parameter, the draw is renewed.
 *                                          For example, a previous draw is [1, 2, 4, 6, 8, 9]
 *                                          the new draw (drawnBallsRand) is [1, 2, 5, 7, 9, 10]
 *                                          If matchComb = 2 and elimComb = 0, draw is OK, because (1,2) (1,9) or (2,9) matched.
 *                                          If matchComb = 2 and elimComb = 3, draw is renewed, because (1,2,9) eliminated. 
 * @return {struct ListX *} drawnBallsRand: Returns new drawn balls.
 */
struct ListX * drawBallByRand(struct ListX *drawnBallsRand, struct ListX *ballStats, int totalBall, int drawBall, int matchComb, int elimComb);



/** 
 * The numbers that came out in the previous draws (ballStats) are arranged at the base of the pascal triangle
 * (stacked to the left) according to the number that comes out the most from the number that comes out the least, 
 * and the ball is dropped on (with gaussIndex function) it and the ball hit is drawn.
 * 
 * @param {struct ListX *} drawnBallsLeft : refers to the balls to be drawn. 
 * @param {struct ListX *} ballStats      : refers to balls and the number of times each ball was drawn in previous draws.
 * @param {Integer} totalBall             : Total ball count in the globe
 * @param {Integer} drawBall        	  : Number of balls to be drawn
 * @param {Integer} matchComb             : Number of combinations in which the drawn numbers must match any of the previous draws.
 * @param {Integer} elimComb              : If a combination of the drawn numbers matched with any of the previous draws, specified by the
 *                                          elimComb parameter, the draw is renewed.
 * @return {struct ListX *} drawnBallsLeft: Returns new drawn balls.
 */
struct ListX * drawBallByLeft(struct ListX *drawnBallsLeft, struct ListX *ballStats, int totalBall, int drawBall, int matchComb, int elimComb);



/** 
 * Blend 1
 * Dividing the list of numbers (ballStats) in the middle, inverting the left and right parts and combining them.
 * The numbers that came out in the previous draws (ballStats) are arranged at the base of the
 * pascal triangle and the ball is dropped on (with gaussIndex function) it and the ball hit is drawn.
 * 
 * @param {struct ListX *} drawnBallsBlend1 : refers to the balls to be drawn. 
 * @param {struct ListX *} ballStats        : refers to balls and the number of times each ball was drawn in previous draws.
 * @param {Integer} totalBall				: Total ball count in the globe
 * @param {Integer} drawBall        	 	: Number of balls to be drawn
 * @param {Integer} matchComb               : Number of combinations in which the drawn numbers must match any of the previous draws.
 * @param {Integer} elimComb                : If a combination of the drawn numbers matched with any of the previous draws, specified by the
 *                                            elimComb parameter, the draw is renewed.
 * @return {struct ListX *} drawnBallsBlend1: Returns new drawn balls.
 */
struct ListX * drawBallByBlend1(struct ListX *drawnBallsBlend1, struct ListX *ballStats, int totalBall, int drawBall, int matchComb, int elimComb);



/** 
 * Blend 2
 * The balls are taken from the left and right of the ballStats and placed from the middle of the globe 
 * (The base of the Pascal's triangle) to the edges
 * 
 * @param {struct ListX *} drawnBallsBlend2 : refers to the balls to be drawn. 
 * @param {struct ListX *} ballStats        : refers to balls and the number of times each ball was drawn in previous draws.
 * @param {Integer} totalBall				: Total ball count in the globe
 * @param {Integer} drawBall        	 	: Number of balls to be drawn
 * @param {Integer} matchComb               : Number of combinations in which the drawn numbers must match any of the previous draws.
 * @param {Integer} elimComb                : If a combination of the drawn numbers matched with any of the previous draws, specified by the
 *                                            elimComb parameter, the draw is renewed.
 * @return {struct ListX *} drawnBallsBlend2: Returns new drawn balls.
 */
struct ListX * drawBallByBlend2(struct ListX *drawnBallsBlend2, struct ListX *ballStats, int totalBall, int drawBall, int matchComb, int elimComb);



/** 
 * The balls that drawn the least are placed in the middle of the globe, and the balls that drawn the most are placed on the edges.
 * 
 * @param {struct ListX *} drawnBallsSide   : refers to the balls to be drawn. 
 * @param {struct ListX *} ballStats        : refers to balls and the number of times each ball was drawn in previous draws.
 * @param {Integer} totalBall				: Total ball count in the globe
 * @param {Integer} drawBall        	  	: Number of balls to be drawn
 * @param {Integer} matchComb               : Number of combinations in which the drawn numbers must match any of the previous draws.
 * @param {Integer} elimComb                : If a combination of the drawn numbers matched with any of the previous draws, specified by the
 *                                            elimComb parameter, the draw is renewed.
 * @return {struct ListX *} drawnBallsSide  : Returns new drawn balls.
 */
struct ListX * drawBallBySide(struct ListX *drawnBallsSide, struct ListX *ballStats, int totalBall, int drawBall, int matchComb, int elimComb);



/** 
 * The most drawn balls are placed in the center of the globe,
 * the least drawn balls are placed at the edges (normal distribution)
 * 
 * @param {struct ListX *} drawnBallsNorm : refers to the balls to be drawn. 
 * @param {struct ListX *} ballStats      : refers to balls and the number of times each ball was drawn in previous draws.
 * @param {Integer} totalBall			  : Total ball count in the globe
 * @param {Integer} drawBall        	  : Number of balls to be drawn
 * @param {Integer} matchComb             : Number of combinations in which the drawn numbers must match any of the previous draws.
 * @param {Integer} elimComb              : If a combination of the drawn numbers matched with any of the previous draws, specified by the
 *                                          elimComb parameter, the draw is renewed.
 *                                          For example, a previous draw is [1, 2, 4, 6, 8, 9]
 *                                          the new draw (drawnBallsNorm) is [1, 2, 5, 7, 9, 10]
 *                                          If matchComb = 2 and elimComb = 0, draw is OK, because (1,2) (1,9) or (2,9) matched.
 *                                          If matchComb = 2 and elimComb = 3, draw is renewed, because (1,2,9) eliminated. 
 * @return {struct ListX *} drawnBallsNorm: Returns new drawn balls.
 */
struct ListX * drawBallByNorm(struct ListX *drawnBallsNorm, struct ListX *ballStats, int totalBall, int drawBall, int matchComb, int elimComb);



/** 
 * The findComb function searches for combinations specified by the comb parameter in previous draws. 
 * For example, the numbers [1, 2, 4, 6, 8, 9] were drawn in one of the previous draws. 
 * The newly drawn numbers are drawnBalls = [1, 2, 5, 7, 9, 10]. 
 * findComb(drawnBalls, 3) searches for 3-combinations in drawnBalls in previous draws.
 * The findComb function returns 1 because there are matching triple combination (1,2,9). 
 * The comb parameter can take values 2, 3, 4, 5 and 6. 
 *
 * @param {struct ListX *} drawnBalls : refers to balls drawn in a draw. 
 * @param {Integer} comb              : ball combinations
 * @return {Integer}                  : Returns 1 if found, 0 if not. 
 */
int findComb(struct ListX *drawnBalls, int comb);



/** 
 * The search6CombXY function searches for 6 combinations in all previous draws (searches in 2 dimensions list). 
 * 
 * @param {struct ListXY *} prvDrawnsList : refers to the balls has been drawn so far. 
 * @param {struct ListX *} drawnBalls     : refers to balls drawn in a new draw. 
 * @return {Integer}                      : Returns 1 if found, 0 if not. 
 */
int search6CombXY(struct ListXY *prvDrawnsList, struct ListX *drawnBalls);



/** 
 * The search6CombX function searches for 6 combinations in a previous draws (searches in 1 dimension list). 
 * 
 * @param {struct ListX *} aPrvDrawn      : refers to a drawn balls in the previous draws.
 * @param {struct ListX *} drawnBalls     : refers to balls drawn in a new draw. 
 * @return {Integer}                      : Returns 1 if found, 0 if not. 
 */
int search6CombX(struct ListX *aPrvDrawn, struct ListX *drawnBalls);



/** 
 * The search5CombXY function searches for 5 combinations in all previous draws. 
 * 
 * @param {struct ListXY *} prvDrawnsList : refers to the balls has been drawn so far. 
 * @param {struct ListX *} drawnBalls     : refers to balls drawn in a new draw. 
 * @return {Integer}                      : Returns 1 if found, 0 if not. 
 */
int search5CombXY(struct ListXY *prvDrawnsList, struct ListX *drawnBalls);



/** 
 * The search5CombX function searches for 5 combinations in a previous draw. 
 * 
 * @param {struct ListX *} aPrvDrawn      : refers to a drawn balls in the previous draws.
 * @param {struct ListX *} drawnBalls     : refers to balls drawn in a new draw. 
 * @param {char *} buf                    : If this parameter is not set to NULL, matching combinations are assigned to this address as string
 * @return {Integer}                      : Returns 1 if found, 0 if not. 
 */
int search5CombX(struct ListX *aPrvDrawn, struct ListX *drawnBalls, char *buf);



/** 
 * The search4CombXY function searches for 4 combinations in all previous draws. 
 * 
 * @param {struct ListXY *} prvDrawnsList : refers to the balls has been drawn so far. 
 * @param {struct ListX *} drawnBalls     : refers to balls drawn in a new draw. 
 * @return {Integer}                      : Returns 1 if found, 0 if not. 
 */
int search4CombXY(struct ListXY *prvDrawnsList, struct ListX *drawnBalls);



/** 
 * The search4CombX function searches for 4 combinations in a previous draw. 
 * 
 * @param {struct ListX *} aPrvDrawn      : refers to a drawn balls in the previous draws.
 * @param {struct ListX *} drawnBalls     : refers to balls drawn in a new draw. 
 * @param {struct ListXY *} luckyBalls    : quartet combinations numbers list that love each other drawn together (lucky numbers)
 * @param {char *} buf                    : If this parameter is not set to NULL, matching combinations are assigned to this address as string
 * @return {Integer}                      : Returns 1 if found, 0 if not. 
 */
int search4CombX(struct ListX *aPrvDrawn, struct ListX *drawnBalls, struct ListXY *luckyBalls, char *buf);



/** 
 * The search3CombXY function searches for 3 combinations in all previous draws. 
 * 
 * @param {struct ListXY *} prvDrawnsList : refers to the balls has been drawn so far. 
 * @param {struct ListX *} drawnBalls     : refers to balls drawn in a new draw. 
 * @return {Integer}                      : Returns 1 if found, 0 if not. 
 */
int search3CombXY(struct ListXY *prvDrawnsList, struct ListX *drawnBalls);



/** 
 * The search3CombX function searches for 3 combinations in a previous draw. 
 * 
 * @param {struct ListX *} aPrvDrawn      : refers to a drawn balls in the previous draws.
 * @param {struct ListX *} drawnBalls     : refers to balls drawn in a new draw. 
 * @param {struct ListXY *} luckyBalls    : triple combinations numbers list that love each other drawn together (lucky numbers)
 * @param {char *} buf                    : If this parameter is not set to NULL, matching combinations are assigned to this address as string
 * @return {Integer}                      : Returns 1 if found, 0 if not. 
 */
int search3CombX(struct ListX *aPrvDrawn, struct ListX *drawnBalls, struct ListXY *luckyBalls, char *buf);



/** 
 * The search2CombXY function searches for 2 combinations in all previous draws. 
 * 
 * @param {struct ListXY *} prvDrawnsList : refers to the balls has been drawn so far. 
 * @param {struct ListX *} drawnBalls     : refers to balls drawn in a new draw. 
 * @return {Integer}                      : Returns 1 if found, 0 if not. 
 */
int search2CombXY(struct ListXY *prvDrawnsList, struct ListX *drawnBalls);



/** 
 * The search2CombX function searches for 2 combinations in a previous draw. 
 * 
 * @param {struct ListX *} aPrvDrawn      : refers to a drawn balls in the previous draws.
 * @param {struct ListX *} drawnBalls     : refers to balls drawn in a new draw. 
 * @param {struct ListXY *} luckyBalls    : double combinations numbers list that love each other drawn together (lucky numbers)
 * @param {char *} buf                    : If this parameter is not set to NULL, matching combinations are assigned to this address as string
 * @return {Integer}                      : Returns 1 if found, 0 if not. 
 */
int search2CombX(struct ListX *aPrvDrawn, struct ListX *drawnBalls, struct ListXY *luckyBalls, char *buf);



/** 
 * The search1BallXY function searches for 1 ball in a draws list. 
 * 
 * @param {struct ListXY *} couponList    : refers to the balls has been new drawns. 
 * @param {struct ListX *} drawnBalls     : refers to balls drawn in a new draw. 
 * @param {Integer} drawBall     		  : DRAW_BALL for normal ball list or 1 for powerBall list (coupon column count)
 * @return {Integer}                      : Returns 1 if found, 0 if not. 
 */
int search1BallXY(struct ListXY *couponList, struct ListX *drawnBalls, int drawBall);



/**
 * Initialization
 * 
*/
void init();



/**
 * Calculate matching combinations count of numbers from previous draws
 * it prints the number of matching double, triple, quartet, quintuple, and six combinations.
*/
void calcMatchCombCount();



/**
 * Calculate matching combinations of numbers from previous draws
 * 
 * @param {Integer} comb    : If 2, it prints matching double combinations along with their dates
 *                            If 3, it prints matching triple combinations along with their dates
 *                            If 4, it prints matching quartet combinations along with their dates
 *                            If 5, it prints matching quintuple combinations along with their dates
 *                            If 6, it prints matching six combinations along with their dates
 * @param {FILE *} fp       : refers to output file. If fp != NULL print to output file
*/
void calcCombMatch(int comb, FILE *fp);



/**
 * Get the numbers that love each other drawn together (lucky numbers)
 * 
 * @param {struct ListXY *} luckyBalls    : refers to the lucky balls list
 * @param {Integer} comb                  : double, triple or quartet (2, 3 or 4) combinations
 * @return {struct ListXY *} luckyBalls   : refers to the lucky balls list
*/
struct ListXY * getLuckyBalls(struct ListXY *luckyBalls, int comb);



/**
 * Draw balls by lucky numbers (the numbers that love each other drawn together)
 * 
 * @param {struct ListXY *} coupon    : refers to the 2 dimensions balls list (coupon)
 * @param {Integer} drawCount         : how many draws will be made
 * @param {Integer} totalDrawCount    : 
*/
void drawBallsByLucky(struct ListXY *coupon, int drawCount, int totalDrawCount);



/**
 * Draw balls by
 * drawBallByDate
 * drawBallByNorm
 * drawBallByLeft
 * drawBallBySide
 * drawBallByBlend1
 * drawBallByBlend2
 * drawBallByRand
 * 
 * @param {struct ListXY *} coupon    : refers to the 2 dimensions balls list (coupon)
 * @param {Integer} drawCount         : how many draws will be made
 * @param {Integer} drawByDate		  : If 1 draw balls by date, if 0 don't draw balls by date.
*/
void drawBalls(struct ListXY *coupon, int drawCount, int drawByDate);



/**
 * Draw SuperStar ball
 * 
 * @param {struct ListXY *} coupon    : refers to the 2 dimensions balls list (coupon)
 * @param {Integer} drawCount         : how many draws will be made
*/
void drawBallsSS(struct ListXY *coupon, int drawCount);



/** 
 * Get drawn balls from file has been drawn so far
 * 
 * @param {struct ListXY *}      : refer to 2 dimensions list of winning numbers
 * @param {struct ListXY *}      : refer to 2 dimensions list of SuperStar numbers
 * @param {char *}               : Drawn list file
 */
void getDrawnBallsList(struct ListXY * winningDrawnBallsList, struct ListXY * superStarDrawnBallsList, char *fileName);




/* FUNCTIONS */

struct ListX *createListX(struct ListX *pl, char *date, char *label, int val)
{
	pl = (struct ListX *) malloc(sizeof(struct ListX));
	pl->head = NULL;
	pl->next = NULL;
	if (date) {
		pl->date = (char *) malloc(sizeof(char)*11);
		strcpy(pl->date, date);
	} else {
		pl->date = NULL;
	}
	if (label) {
		pl->label = (char *) malloc(sizeof(char)*30);
		strcpy(pl->label, label);
	} else {
		pl->label = NULL;
	}
	pl->val = val;
	return pl;
}



struct ListXY *createListXY(struct ListXY *pl)
{
	pl = (struct ListXY *) malloc(sizeof(struct ListXY));
	pl->list = NULL;
	return pl;
}



void insertItem(struct ListX *pl, int key)
{
	struct Item *pt = (struct Item *) malloc(sizeof(struct Item));
	pt->key = key;
	pt->next = pl->head;
	pl->head = pt;
}



struct Item *atEnd(struct ListX *pl)
{
	struct Item *curr;
	if (pl->head == NULL)
		return NULL;
	curr = pl->head;

	while (curr->next)
		curr = curr->next;
	return curr;
}



struct ListX *atEndY(struct ListXY *pl)
{
	struct ListX *curr;
	if (pl == NULL)
		return NULL;
	curr = pl->list;

	while (curr->next)
		curr = curr->next;
	return curr;
}



void appendItem(struct ListX *pl, int key)
{
	struct Item *pt = (struct Item *) malloc(sizeof(struct Item));
	pt->key = key;
	pt->next = NULL;
	if (pl->head == NULL)
		pl->head = pt;
	else
		(atEnd(pl))->next = pt;
}



void appendList(struct ListXY *pl, struct ListX *plNext)
{
	if (pl == NULL) return;
	if (pl->list == NULL)
		pl->list = plNext;
	else {
		(atEndY(pl))->next = plNext;
        plNext->next = NULL;
    }
}



void removeItemByKey(struct ListX *pl, int key)
{
	struct Item *prv, *pt = pl->head;

	if ((pt) && (pt->key==key))
	{
		pl->head = pt->next;
		free(pt);
	}
	else if (pt)
	{
		for (prv=pt, pt=pt->next; ((pt) && (pt->key != key)); prv=prv->next, pt=pt->next);
		if (pt->key == key)
		{
			prv->next = pt->next;
			free(pt);
		}
	}
}



int removeItemByIndex(struct ListX *pl, int ind)
{
	int i, key;
	struct Item *t=pl->head;

	for (i=0; (t) && i<ind; i++)
		t=t->next;

	key = t->key;
	removeItemByKey(pl, key);
	return key;
}



int addItemByIndex(struct ListX *pl, int ind, int key)
{
	struct Item *right = NULL, *left = pl->head;
	struct Item *pt = (struct Item *) malloc(sizeof(struct Item));
	int i;

	if(seqSearch(pl, key) >= 0) return 0;

	pt->key = key;

	if (left == NULL || ind == 0) {
		pl->head = pt;
		pt->next = left;
	} else {
		right = left->next;

		for (i=1; i<ind; i++) {
			if (left != NULL) left = left->next;
			if (right != NULL) right = right->next;
		}

		if (left == NULL) return 0;
		
		left->next = pt;
		pt->next = right;
	}

	return 1;
}



void removeAllX(struct ListX *pl)
{
	struct Item *prv, *pt = pl->head;
	
	while (pt != NULL) {
		prv = pt;
		pt = pt->next;
		free(prv);
	} 

	pl->head = NULL;
}



void removeAllXY(struct ListXY *pl)
{
	struct ListX *prvList, *pList = pl->list;

	while (pList != NULL) {
		prvList = pList;
		pList = pList->next;
		removeAllX(prvList);
		free(prvList);
	} 

	pl->list = NULL;
}



int length(struct ListX *pl)
{
	struct Item *tmp = pl->head;
	int i=0;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return i;
}



int lengthY(struct ListXY *pl)
{
	struct ListX *tmp = pl->list;
	int i=0;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return i;
}



int seqSearch(struct ListX *pl, int key)
{
	struct Item *t=pl->head;
	int i=0;
	while ( (t) && (t->key!=key) ) {
		t=t->next;
		i++;
	}

	if (t) return i;
	else return -1;
}



int seqSearchXY1(struct ListXY *pl, int key)
{
	struct ListX *tmp = pl->list;
	int i=0;
	while (tmp)
	{
		if (seqSearch(tmp, key) >= 0) break;
		tmp = tmp->next;
		i++;
	}
	if (tmp) return i;
	else return -1;
}



int seqSearchXY2(struct ListXY *pl, int key1, int key2)
{
	struct ListX *tmp = pl->list;
	int i=0;
	while (tmp)
	{
		if (seqSearch(tmp, key1) >= 0 && seqSearch(tmp, key2) >= 0) break;
		tmp = tmp->next;
		i++;
	}
	if (tmp) return i;
	else return -1;
}



int seqSearchXY3(struct ListXY *pl, int key1, int key2, int key3)
{
	struct ListX *tmp = pl->list;
	int i=0;
	while (tmp)
	{
		if (seqSearch(tmp, key1) >= 0 && seqSearch(tmp, key2) >= 0 && seqSearch(tmp, key3) >= 0) break;
		tmp = tmp->next;
		i++;
	}
	if (tmp) return i;
	else return -1;
}



int seqSearchXY4(struct ListXY *pl, int key1, int key2, int key3, int key4)
{
	struct ListX *tmp = pl->list;
	int i=0;
	while (tmp)
	{
		if (seqSearch(tmp, key1) >= 0 && seqSearch(tmp, key2) >= 0 && seqSearch(tmp, key3) >= 0 && seqSearch(tmp, key4) >= 0) break;
		tmp = tmp->next;
		i++;
	}
	if (tmp) return i;
	else return -1;
}



void chgVal(struct ListX *pl, int ind, int val)
{
	struct Item *t=pl->head;
	int i;
	for (i=0; (t) && i<ind; i++) {
		t=t->next;
	}
	t->val = val;
}



void chgValXY(struct ListXY *pl, int ind, int val)
{
	struct ListX *tmp = pl->list;
	int i;
	for (i=0; (tmp) && i<ind; i++) {
		tmp=tmp->next;
	}
	tmp->val = val;
}



void incVal(struct ListX *pl, int ind)
{
	struct Item *t=pl->head;
	int i;
	for (i=0; (t) && i<ind; i++) {
		t=t->next;
	}
	t->val++;
}



void incValXY(struct ListXY *pl, int ind)
{
	struct ListX *tmp = pl->list;
	int i;
	for (i=0; (tmp) && i<ind; i++) {
		tmp=tmp->next;
	}
	tmp->val++;
}



int getVal(struct ListX *pl, int ind)
{
	struct Item *t=pl->head;
	int i;
	for (i=0; (t) && i<ind; i++) {
		t=t->next;
	}
	return t->val;
}



int getValXY(struct ListXY *pl, int ind)
{
	struct ListX *tmp = pl->list;
	int i;
	for (i=0; (tmp) && i<ind; i++) {
		tmp=tmp->next;
	}
	return tmp->val;
}



int getKey(struct ListX *pl, int ind)
{
	struct Item *t=pl->head;
	int i;
	for (i=0; (t) && i<ind; i++) {
		t=t->next;
	}
	return t->key;
}



struct ListX * getListXByIndex(struct ListXY *pl, int ind)
{
	struct ListX *l = pl->list;
	int i;
	for (i=0; (l) && i<ind; i++) {
		l = l->next;
	}
	return l;
}



void bubbleSortXByVal(struct ListX *pl, int inc)
{
	int swapped, i;
	struct Item *ptr1;
	struct Item *lptr = NULL;
	
	if (pl->head == NULL) return;
	
	do {
		swapped = 0;
		ptr1 = pl->head;
		
		while (ptr1->next != lptr)
		{
			if (inc == 1) {
				if (ptr1->val > ptr1->next->val) {
					swapX(ptr1, ptr1->next);
					swapped = 1;
				}
			} else if (inc == -1) {
				if (ptr1->val < ptr1->next->val) {
					swapX(ptr1, ptr1->next);
					swapped = 1;
				}
			}
			ptr1 = ptr1->next;
		}
		lptr = ptr1;
	} while (swapped);
}



void bubbleSortYByVal(struct ListXY *pl, int inc)
{
	int swapped, i;
	struct ListX *ptr1;
	struct ListX *lptr = NULL;
	
	if (pl->list == NULL) return;
	
	do {
		swapped = 0;
		ptr1 = pl->list;
		
		while (ptr1->next != lptr)
		{
			if (inc == 1) {
				if (ptr1->val > ptr1->next->val) {
					swapY(ptr1, ptr1->next);
					swapped = 1;
				}
			} else if (inc == -1) {
				if (ptr1->val < ptr1->next->val) {
					swapY(ptr1, ptr1->next);
					swapped = 1;
				}
			}
			ptr1 = ptr1->next;
		}
		lptr = ptr1;
	} while (swapped);
}



void bubbleSortXByKey(struct ListX *pl)
{
	int swapped, i;
	struct Item *ptr1;
	struct Item *lptr = NULL;
	
	if (pl->head == NULL) return;
	
	do {
		swapped = 0;
		ptr1 = pl->head;
		
		while (ptr1->next != lptr)
		{
			if (ptr1->key > ptr1->next->key) {
				swapX(ptr1, ptr1->next);
				swapped = 1;
			}
			ptr1 = ptr1->next;
		}
		lptr = ptr1;
	} while (swapped);
}



void swapX(struct Item *a, struct Item *b)
{
	int temp;
	
	temp = a->key;
	a->key = b->key;
	b->key = temp;

	temp = a->val;
	a->val = b->val;
	b->val = temp;
}



void swapY(struct ListX *a, struct ListX *b)
{
	struct Item *head;
	char *date;
    char *label;
	int val;
	
	val = a->val;
	a->val = b->val;
	b->val = val;

	date = a->date;
	a->date = b->date;
	b->date = date;

	label = a->label;
	a->label = b->label;
	b->label = label;

	head = b->head;
	b->head = a->head;
	a->head = head;
}



void printListXByKey(struct ListX *pl, int printTo, FILE *fp)
{
	struct Item *t=pl->head;
	while (t) {
		if (printTo == 0) { // print to screen and output file
            printf("%2d ", t->key);
            fprintf(fp, "%2d ", t->key);
        } else if (printTo == 1) { // print to screen only
            printf("%2d ", t->key);
        } else if (printTo == 2) { // print to output file only
            fprintf(fp, "%2d ", t->key);
        }
		t=t->next;
	}
}



void printListXYByKey(struct ListXY *pl, FILE *fp)
{
	struct ListX *nl = pl->list;
	struct Item *pt = nl->head;
    int i;
	
	printf("     Numbers\n\n");
    if (fp != NULL) fprintf(fp, "     Numbers\n\n");

	for (i=1; (nl); i++) {
		printf("%2d - ", i);
		if (fp != NULL) fprintf(fp, "%2d - ", i);
		while (pt) {
			printf("%2d ", pt->key);
			if (fp != NULL) fprintf(fp, "%2d ", pt->key);
			pt = pt->next;
		}

		if (nl->label) {
			printf("  %s", nl->label);
			if (fp != NULL) fprintf(fp, "  %s", nl->label);
		}
		nl = nl->next;
		if (nl != NULL) pt = nl->head;
        printf("\n");
        if (fp != NULL) fprintf(fp, "\n");
	}
}



void printListXYWithSSByKey(struct ListXY *pl1, struct ListXY *pl2, FILE *fp)
{
	struct ListX *nl1 = pl1->list;
	struct ListX *nl2 = pl2->list;
	struct Item *pt1 = nl1->head;
	struct Item *pt2 = nl2->head;
    int i;

	printf("     Numbers          SuperStar\n\n");
    if (fp != NULL) fprintf(fp, "     Numbers          SuperStar\n\n");
    
    for (i=1; (nl1) && (nl2); i++) {
		printf("%2d - ", i);
		if (fp != NULL) fprintf(fp, "%2d - ", i);
		while (pt1) {
			printf("%2d ", pt1->key);
			if (fp != NULL) fprintf(fp, "%2d ", pt1->key);
			pt1 = pt1->next;
		}

		printf("  %2d ", pt2->key);
		if (fp != NULL) fprintf(fp, "  %2d ", pt2->key);

		if (nl1->label) {
            printf("     %s", nl1->label);
            if (fp != NULL) fprintf(fp, "     %s", nl1->label);
        }
		nl1 = nl1->next;
		nl2 = nl2->next;
		if (nl1 != NULL && nl2 != NULL) {
			pt1 = nl1->head;
			pt2 = nl2->head;
		}
        printf("\n");
        if (fp != NULL) fprintf(fp, "\n");
	}
}



void printBallStats(struct ListX *ballStats)
{
	struct Item *t = ballStats->head;
	int i = 0;

#ifdef __MSDOS__
	int col = 10;
#else
	int col = 15;
#endif

	while (t) {
		if (i%col == 0 && i!=0) printf("\n");
		printf("%2d:%3d  ", t->key, t->val);
		t=t->next;
		i++;
	}
}



void printLuckyBalls(struct ListXY *pl, FILE *fp)
{
	struct ListX *nl = pl->list;
	struct Item *pt = nl->head;
	int i = 0;
	int len = length(nl);

	while (nl) {
		if (i%(8-len) == 0 && i!=0) {
            printf("\n");
            if (fp != NULL) fprintf(fp, "\n");
        }
		while (pt) {
			printf("%2d ", pt->key);
            if (fp != NULL) fprintf(fp, "%2d ", pt->key);
			pt = pt->next;
		}
		//if (nl->val > 1) {
		if (1) {
			printf(": %2d times    ", numberOfTerm(nl->val)+1);
            if (fp != NULL) fprintf(fp, ": %2d times    ", numberOfTerm(nl->val)+1);
			i++;
		}
		nl = nl->next;
		if (nl != NULL) pt = nl->head;
	}
    printf("\n");
}



int numberOfTerm(int sum)
{
	int n;
	for (n=0; sum>0; n++) {
		sum = sum - (n+1);
	}

	return n;
}



int gaussIndex(int ballCount)
{
	int node=0, leftnode=0, level;

	for (level=1; level<ballCount;)
	{
        if (rand() % 100 < 49 + rand() % 2)
			node = node + level;
        else
			node = node + level + 1;

		leftnode = leftnode + level;
		level++;
	}
	
	return node - (leftnode-1);
}



int findComb(struct ListX *drawnBalls, int comb)
{
	int found = 0;

	switch (comb)
	{
	case 2 : found = search2CombXY(winningDrawnBallsList, drawnBalls); break;
	case 3 : found = search3CombXY(winningDrawnBallsList, drawnBalls); break;
	case 4 : found = search4CombXY(winningDrawnBallsList, drawnBalls); break;
	case 5 : found = search5CombXY(winningDrawnBallsList, drawnBalls); break;
	case 6 : found = search6CombXY(winningDrawnBallsList, drawnBalls); break;
	default: break;
	}

	return found;
}



int search6CombXY(struct ListXY *prvDrawnsList, struct ListX *drawnBalls)
{
	int i;
	int b1 = getKey(drawnBalls, 0);
	int b2 = getKey(drawnBalls, 1);
	int b3 = getKey(drawnBalls, 2);
	int b4 = getKey(drawnBalls, 3);
	int b5 = getKey(drawnBalls, 4);
	int b6 = getKey(drawnBalls, 5);
	struct ListX *aPrvDrawn = NULL;
	int found = 0;

	for (i=0; i<lengthY(prvDrawnsList); i++) 
	{
		aPrvDrawn = getListXByIndex(prvDrawnsList, i);
		if (seqSearch(aPrvDrawn, b1) >= 0 && seqSearch(aPrvDrawn, b2) >= 0 && seqSearch(aPrvDrawn, b3) >= 0 &&
			seqSearch(aPrvDrawn, b4) >= 0 && seqSearch(aPrvDrawn, b5) >= 0 && seqSearch(aPrvDrawn, b6) >= 0) {found=1; break;}
	}

	return found;
}



int search6CombX(struct ListX *aPrvDrawn, struct ListX *drawnBalls)
{
	int b1 = getKey(drawnBalls, 0);
	int b2 = getKey(drawnBalls, 1);
	int b3 = getKey(drawnBalls, 2);
	int b4 = getKey(drawnBalls, 3);
	int b5 = getKey(drawnBalls, 4);
	int b6 = getKey(drawnBalls, 5);
	int found = 0;

	if (seqSearch(aPrvDrawn, b1) >= 0 && seqSearch(aPrvDrawn, b2) >= 0 && seqSearch(aPrvDrawn, b3) >= 0 &&
		seqSearch(aPrvDrawn, b4) >= 0 && seqSearch(aPrvDrawn, b5) >= 0 && seqSearch(aPrvDrawn, b6) >= 0) found=1;

	return found;
}



int search5CombXY(struct ListXY *prvDrawnsList, struct ListX *drawnBalls)
{
	int i;
	int b1 = getKey(drawnBalls, 0);
	int b2 = getKey(drawnBalls, 1);
	int b3 = getKey(drawnBalls, 2);
	int b4 = getKey(drawnBalls, 3);
	int b5 = getKey(drawnBalls, 4);
	int b6 = getKey(drawnBalls, 5);
	struct ListX *aPrvDrawn = NULL;
	int found = 0;

	for (i=0; i<lengthY(prvDrawnsList); i++) 
	{
		aPrvDrawn = getListXByIndex(prvDrawnsList, i);
		if (seqSearch(aPrvDrawn, b1) >= 0 && seqSearch(aPrvDrawn, b2) >= 0 && seqSearch(aPrvDrawn, b3) >= 0 && seqSearch(aPrvDrawn, b4) >= 0 && seqSearch(aPrvDrawn, b5) >= 0) {found=1; break;}
		if (seqSearch(aPrvDrawn, b1) >= 0 && seqSearch(aPrvDrawn, b2) >= 0 && seqSearch(aPrvDrawn, b3) >= 0 && seqSearch(aPrvDrawn, b4) >= 0 && seqSearch(aPrvDrawn, b6) >= 0) {found=1; break;}
		if (seqSearch(aPrvDrawn, b1) >= 0 && seqSearch(aPrvDrawn, b2) >= 0 && seqSearch(aPrvDrawn, b3) >= 0 && seqSearch(aPrvDrawn, b5) >= 0 && seqSearch(aPrvDrawn, b6) >= 0) {found=1; break;}
		if (seqSearch(aPrvDrawn, b1) >= 0 && seqSearch(aPrvDrawn, b2) >= 0 && seqSearch(aPrvDrawn, b4) >= 0 && seqSearch(aPrvDrawn, b5) >= 0 && seqSearch(aPrvDrawn, b6) >= 0) {found=1; break;}
		if (seqSearch(aPrvDrawn, b1) >= 0 && seqSearch(aPrvDrawn, b3) >= 0 && seqSearch(aPrvDrawn, b4) >= 0 && seqSearch(aPrvDrawn, b5) >= 0 && seqSearch(aPrvDrawn, b6) >= 0) {found=1; break;}
		if (seqSearch(aPrvDrawn, b2) >= 0 && seqSearch(aPrvDrawn, b3) >= 0 && seqSearch(aPrvDrawn, b4) >= 0 && seqSearch(aPrvDrawn, b5) >= 0 && seqSearch(aPrvDrawn, b6) >= 0) {found=1; break;}
	}

	return found;
}



int search5CombX(struct ListX *aPrvDrawn, struct ListX *drawnBalls, char *buf)
{
	int b1 = getKey(drawnBalls, 0);
	int b2 = getKey(drawnBalls, 1);
	int b3 = getKey(drawnBalls, 2);
	int b4 = getKey(drawnBalls, 3);
	int b5 = getKey(drawnBalls, 4);
	int b6 = getKey(drawnBalls, 5);
	int found = 0;
	char fStr[20];

	if (buf) buf[0] = '\0';

	if (seqSearch(aPrvDrawn, b1) >= 0 && seqSearch(aPrvDrawn, b2) >= 0 && seqSearch(aPrvDrawn, b3) >= 0 && seqSearch(aPrvDrawn, b4) >= 0 && seqSearch(aPrvDrawn, b5) >= 0)
	{
		if (buf) {
			sprintf(fStr, "(%2d,%2d,%2d,%2d,%2d)", b1, b2, b3, b4, b5);
			strcat(buf, fStr);
		}
		found=1;
	}
	if (seqSearch(aPrvDrawn, b1) >= 0 && seqSearch(aPrvDrawn, b2) >= 0 && seqSearch(aPrvDrawn, b3) >= 0 && seqSearch(aPrvDrawn, b4) >= 0 && seqSearch(aPrvDrawn, b6) >= 0)
	{
		if (buf) {
			if (found) strcat(buf, ", ");
			sprintf(fStr, "(%2d,%2d,%2d,%2d,%2d)", b1, b2, b3, b4, b6);
			strcat(buf, fStr);
		}
		found=1;
	}
	if (seqSearch(aPrvDrawn, b1) >= 0 && seqSearch(aPrvDrawn, b2) >= 0 && seqSearch(aPrvDrawn, b3) >= 0 && seqSearch(aPrvDrawn, b5) >= 0 && seqSearch(aPrvDrawn, b6) >= 0)
	{
		if (buf) {
			if (found) strcat(buf, ", ");
			sprintf(fStr, "(%2d,%2d,%2d,%2d,%2d)", b1, b2, b3, b5, b6);
			strcat(buf, fStr);
		}
		found=1;
	}
	if (seqSearch(aPrvDrawn, b1) >= 0 && seqSearch(aPrvDrawn, b2) >= 0 && seqSearch(aPrvDrawn, b4) >= 0 && seqSearch(aPrvDrawn, b5) >= 0 && seqSearch(aPrvDrawn, b6) >= 0)
	{
		if (buf) {
			if (found) strcat(buf, ", ");
			sprintf(fStr, "(%2d,%2d,%2d,%2d,%2d)", b1, b2, b4, b5, b6);
			strcat(buf, fStr);
		}
		found=1;
	}
	if (seqSearch(aPrvDrawn, b1) >= 0 && seqSearch(aPrvDrawn, b3) >= 0 && seqSearch(aPrvDrawn, b4) >= 0 && seqSearch(aPrvDrawn, b5) >= 0 && seqSearch(aPrvDrawn, b6) >= 0)
	{
		if (buf) {
			if (found) strcat(buf, ", ");
			sprintf(fStr, "(%2d,%2d,%2d,%2d,%2d)", b1, b3, b4, b5, b6);
			strcat(buf, fStr);
		}
		found=1;
	}
	if (seqSearch(aPrvDrawn, b2) >= 0 && seqSearch(aPrvDrawn, b3) >= 0 && seqSearch(aPrvDrawn, b4) >= 0 && seqSearch(aPrvDrawn, b5) >= 0 && seqSearch(aPrvDrawn, b6) >= 0)
	{
		if (buf) {
			if (found) strcat(buf, ", ");
			sprintf(fStr, "(%2d,%2d,%2d,%2d,%2d)", b2, b3, b4, b5, b6);
			strcat(buf, fStr);
		}
		found=1;
	}

	return found;
}



int search4CombXY(struct ListXY *prvDrawnsList, struct ListX *drawnBalls)
{
	int i;
	int b1 = getKey(drawnBalls, 0);
	int b2 = getKey(drawnBalls, 1);
	int b3 = getKey(drawnBalls, 2);
	int b4 = getKey(drawnBalls, 3);
	int b5 = getKey(drawnBalls, 4);
	int b6 = getKey(drawnBalls, 5);
	struct ListX *aPrvDrawn = NULL;
	int found = 0;

	for (i=0; i<lengthY(prvDrawnsList); i++) 
	{
		aPrvDrawn = getListXByIndex(prvDrawnsList, i);
		if (seqSearch(aPrvDrawn, b1) >= 0 && seqSearch(aPrvDrawn, b2) >= 0 && seqSearch(aPrvDrawn, b3) >= 0 && seqSearch(aPrvDrawn, b4) >= 0) {found=1; break;}
		if (seqSearch(aPrvDrawn, b1) >= 0 && seqSearch(aPrvDrawn, b2) >= 0 && seqSearch(aPrvDrawn, b3) >= 0 && seqSearch(aPrvDrawn, b5) >= 0) {found=1; break;}
		if (seqSearch(aPrvDrawn, b1) >= 0 && seqSearch(aPrvDrawn, b2) >= 0 && seqSearch(aPrvDrawn, b3) >= 0 && seqSearch(aPrvDrawn, b6) >= 0) {found=1; break;}
		if (seqSearch(aPrvDrawn, b1) >= 0 && seqSearch(aPrvDrawn, b2) >= 0 && seqSearch(aPrvDrawn, b4) >= 0 && seqSearch(aPrvDrawn, b5) >= 0) {found=1; break;}
		if (seqSearch(aPrvDrawn, b1) >= 0 && seqSearch(aPrvDrawn, b2) >= 0 && seqSearch(aPrvDrawn, b4) >= 0 && seqSearch(aPrvDrawn, b6) >= 0) {found=1; break;}
		if (seqSearch(aPrvDrawn, b1) >= 0 && seqSearch(aPrvDrawn, b2) >= 0 && seqSearch(aPrvDrawn, b5) >= 0 && seqSearch(aPrvDrawn, b6) >= 0) {found=1; break;}
		if (seqSearch(aPrvDrawn, b1) >= 0 && seqSearch(aPrvDrawn, b3) >= 0 && seqSearch(aPrvDrawn, b4) >= 0 && seqSearch(aPrvDrawn, b5) >= 0) {found=1; break;}
		if (seqSearch(aPrvDrawn, b1) >= 0 && seqSearch(aPrvDrawn, b3) >= 0 && seqSearch(aPrvDrawn, b4) >= 0 && seqSearch(aPrvDrawn, b6) >= 0) {found=1; break;}
		if (seqSearch(aPrvDrawn, b1) >= 0 && seqSearch(aPrvDrawn, b3) >= 0 && seqSearch(aPrvDrawn, b5) >= 0 && seqSearch(aPrvDrawn, b6) >= 0) {found=1; break;}
		if (seqSearch(aPrvDrawn, b1) >= 0 && seqSearch(aPrvDrawn, b4) >= 0 && seqSearch(aPrvDrawn, b5) >= 0 && seqSearch(aPrvDrawn, b6) >= 0) {found=1; break;}
		if (seqSearch(aPrvDrawn, b2) >= 0 && seqSearch(aPrvDrawn, b3) >= 0 && seqSearch(aPrvDrawn, b4) >= 0 && seqSearch(aPrvDrawn, b5) >= 0) {found=1; break;}
		if (seqSearch(aPrvDrawn, b2) >= 0 && seqSearch(aPrvDrawn, b3) >= 0 && seqSearch(aPrvDrawn, b4) >= 0 && seqSearch(aPrvDrawn, b6) >= 0) {found=1; break;}
		if (seqSearch(aPrvDrawn, b2) >= 0 && seqSearch(aPrvDrawn, b3) >= 0 && seqSearch(aPrvDrawn, b5) >= 0 && seqSearch(aPrvDrawn, b6) >= 0) {found=1; break;}
		if (seqSearch(aPrvDrawn, b2) >= 0 && seqSearch(aPrvDrawn, b4) >= 0 && seqSearch(aPrvDrawn, b5) >= 0 && seqSearch(aPrvDrawn, b6) >= 0) {found=1; break;}
		if (seqSearch(aPrvDrawn, b3) >= 0 && seqSearch(aPrvDrawn, b4) >= 0 && seqSearch(aPrvDrawn, b5) >= 0 && seqSearch(aPrvDrawn, b6) >= 0) {found=1; break;}
	}

	return found;
}



int search4CombX(struct ListX *aPrvDrawn, struct ListX *drawnBalls, struct ListXY *luckyBalls, char *buf)
{
	struct ListX *lb = NULL;
	int index;
	int b1 = getKey(drawnBalls, 0);
	int b2 = getKey(drawnBalls, 1);
	int b3 = getKey(drawnBalls, 2);
	int b4 = getKey(drawnBalls, 3);
	int b5 = getKey(drawnBalls, 4);
	int b6 = getKey(drawnBalls, 5);
	int found = 0;
	char fStr[20];

	if (buf) buf[0] = '\0';

	if (seqSearch(aPrvDrawn, b1) >= 0 && seqSearch(aPrvDrawn, b2) >= 0 && seqSearch(aPrvDrawn, b3) >= 0 && seqSearch(aPrvDrawn, b4) >= 0)
	{
		if (buf) {
			sprintf(fStr, "(%2d,%2d,%2d,%2d)", b1, b2, b3, b4);
			strcat(buf, fStr);
		}
		if (luckyBalls) {
			if ((index = seqSearchXY4(luckyBalls, b1, b2, b3, b4)) < 0) {
				lb = createListX(lb, NULL, NULL, 1);
				appendItem(lb, b1);
				appendItem(lb, b2);
				appendItem(lb, b3);
				appendItem(lb, b4);
				appendList(luckyBalls, lb);
			} else {
				incValXY(luckyBalls, index);
			}
		}
		found=1;
	}
	if (seqSearch(aPrvDrawn, b1) >= 0 && seqSearch(aPrvDrawn, b2) >= 0 && seqSearch(aPrvDrawn, b3) >= 0 && seqSearch(aPrvDrawn, b5) >= 0)
	{
		if (buf) {
			if (found) strcat(buf, ", ");
			sprintf(fStr, "(%2d,%2d,%2d,%2d)", b1, b2, b3, b5);
			strcat(buf, fStr);
		}
		if (luckyBalls) {
			if ((index = seqSearchXY4(luckyBalls, b1, b2, b3, b5)) < 0) {
				lb = createListX(lb, NULL, NULL, 1);
				appendItem(lb, b1);
				appendItem(lb, b2);
				appendItem(lb, b3);
				appendItem(lb, b5);
				appendList(luckyBalls, lb);
			} else {
				incValXY(luckyBalls, index);
			}
		}
		found=1;
	}
	if (seqSearch(aPrvDrawn, b1) >= 0 && seqSearch(aPrvDrawn, b2) >= 0 && seqSearch(aPrvDrawn, b3) >= 0 && seqSearch(aPrvDrawn, b6) >= 0)
	{
		if (buf) {
			if (found) strcat(buf, ", ");
			sprintf(fStr, "(%2d,%2d,%2d,%2d)", b1, b2, b3, b6);
			strcat(buf, fStr);
		}
		if (luckyBalls) {
			if ((index = seqSearchXY4(luckyBalls, b1, b2, b3, b6)) < 0) {
				lb = createListX(lb, NULL, NULL, 1);
				appendItem(lb, b1);
				appendItem(lb, b2);
				appendItem(lb, b3);
				appendItem(lb, b6);
				appendList(luckyBalls, lb);
			} else {
				incValXY(luckyBalls, index);
			}
		}
		found=1;
	}
	if (seqSearch(aPrvDrawn, b1) >= 0 && seqSearch(aPrvDrawn, b2) >= 0 && seqSearch(aPrvDrawn, b4) >= 0 && seqSearch(aPrvDrawn, b5) >= 0)
	{
		if (buf) {
			if (found) strcat(buf, ", ");
			sprintf(fStr, "(%2d,%2d,%2d,%2d)", b1, b2, b4, b5);
			strcat(buf, fStr);
		}
		if (luckyBalls) {
			if ((index = seqSearchXY4(luckyBalls, b1, b2, b4, b5)) < 0) {
				lb = createListX(lb, NULL, NULL, 1);
				appendItem(lb, b1);
				appendItem(lb, b2);
				appendItem(lb, b4);
				appendItem(lb, b5);
				appendList(luckyBalls, lb);
			} else {
				incValXY(luckyBalls, index);
			}
		}
		found=1;
	}
	if (seqSearch(aPrvDrawn, b1) >= 0 && seqSearch(aPrvDrawn, b2) >= 0 && seqSearch(aPrvDrawn, b4) >= 0 && seqSearch(aPrvDrawn, b6) >= 0)
	{
		if (buf) {
			if (found) strcat(buf, ", ");
			sprintf(fStr, "(%2d,%2d,%2d,%2d)", b1, b2, b4, b6);
			strcat(buf, fStr);
		}
		if (luckyBalls) {
			if ((index = seqSearchXY4(luckyBalls, b1, b2, b4, b6)) < 0) {
				lb = createListX(lb, NULL, NULL, 1);
				appendItem(lb, b1);
				appendItem(lb, b2);
				appendItem(lb, b4);
				appendItem(lb, b6);
				appendList(luckyBalls, lb);
			} else {
				incValXY(luckyBalls, index);
			}
		}
		found=1;
	}
	if (seqSearch(aPrvDrawn, b1) >= 0 && seqSearch(aPrvDrawn, b2) >= 0 && seqSearch(aPrvDrawn, b5) >= 0 && seqSearch(aPrvDrawn, b6) >= 0)
	{
		if (buf) {
			if (found) strcat(buf, ", ");
			sprintf(fStr, "(%2d,%2d,%2d,%2d)", b1, b2, b5, b6);
			strcat(buf, fStr);
		}
		if (luckyBalls) {
			if ((index = seqSearchXY4(luckyBalls, b1, b2, b5, b6)) < 0) {
				lb = createListX(lb, NULL, NULL, 1);
				appendItem(lb, b1);
				appendItem(lb, b2);
				appendItem(lb, b5);
				appendItem(lb, b6);
				appendList(luckyBalls, lb);
			} else {
				incValXY(luckyBalls, index);
			}
		}
		found=1;
	}
	if (seqSearch(aPrvDrawn, b1) >= 0 && seqSearch(aPrvDrawn, b3) >= 0 && seqSearch(aPrvDrawn, b4) >= 0 && seqSearch(aPrvDrawn, b5) >= 0)
	{
		if (buf) {
			if (found) strcat(buf, ", ");
			sprintf(fStr, "(%2d,%2d,%2d,%2d)", b1, b3, b4, b5);
			strcat(buf, fStr);
		}
		if (luckyBalls) {
			if ((index = seqSearchXY4(luckyBalls, b1, b3, b4, b5)) < 0) {
				lb = createListX(lb, NULL, NULL, 1);
				appendItem(lb, b1);
				appendItem(lb, b3);
				appendItem(lb, b4);
				appendItem(lb, b5);
				appendList(luckyBalls, lb);
			} else {
				incValXY(luckyBalls, index);
			}
		}
		found=1;
	}
	if (seqSearch(aPrvDrawn, b1) >= 0 && seqSearch(aPrvDrawn, b3) >= 0 && seqSearch(aPrvDrawn, b4) >= 0 && seqSearch(aPrvDrawn, b6) >= 0)
	{
		if (buf) {
			if (found) strcat(buf, ", ");
			sprintf(fStr, "(%2d,%2d,%2d,%2d)", b1, b3, b4, b6);
			strcat(buf, fStr);
		}
		if (luckyBalls) {
			if ((index = seqSearchXY4(luckyBalls, b1, b3, b4, b6)) < 0) {
				lb = createListX(lb, NULL, NULL, 1);
				appendItem(lb, b1);
				appendItem(lb, b3);
				appendItem(lb, b4);
				appendItem(lb, b6);
				appendList(luckyBalls, lb);
			} else {
				incValXY(luckyBalls, index);
			}
		}
		found=1;
	}
	if (seqSearch(aPrvDrawn, b1) >= 0 && seqSearch(aPrvDrawn, b3) >= 0 && seqSearch(aPrvDrawn, b5) >= 0 && seqSearch(aPrvDrawn, b6) >= 0)
	{
		if (buf) {
			if (found) strcat(buf, ", ");
			sprintf(fStr, "(%2d,%2d,%2d,%2d)", b1, b3, b5, b6);
			strcat(buf, fStr);
		}
		if (luckyBalls) {
			if ((index = seqSearchXY4(luckyBalls, b1, b3, b5, b6)) < 0) {
				lb = createListX(lb, NULL, NULL, 1);
				appendItem(lb, b1);
				appendItem(lb, b3);
				appendItem(lb, b5);
				appendItem(lb, b6);
				appendList(luckyBalls, lb);
			} else {
				incValXY(luckyBalls, index);
			}
		}
		found=1;
	}
	if (seqSearch(aPrvDrawn, b1) >= 0 && seqSearch(aPrvDrawn, b4) >= 0 && seqSearch(aPrvDrawn, b5) >= 0 && seqSearch(aPrvDrawn, b6) >= 0)
	{
		if (buf) {
			if (found) strcat(buf, ", ");
			sprintf(fStr, "(%2d,%2d,%2d,%2d)", b1, b4, b5, b6);
			strcat(buf, fStr);
		}
		if (luckyBalls) {
			if ((index = seqSearchXY4(luckyBalls, b1, b4, b5, b6)) < 0) {
				lb = createListX(lb, NULL, NULL, 1);
				appendItem(lb, b1);
				appendItem(lb, b4);
				appendItem(lb, b5);
				appendItem(lb, b6);
				appendList(luckyBalls, lb);
			} else {
				incValXY(luckyBalls, index);
			}
		}
		found=1;
	}
	if (seqSearch(aPrvDrawn, b2) >= 0 && seqSearch(aPrvDrawn, b3) >= 0 && seqSearch(aPrvDrawn, b4) >= 0 && seqSearch(aPrvDrawn, b5) >= 0)
	{
		if (buf) {
			if (found) strcat(buf, ", ");
			sprintf(fStr, "(%2d,%2d,%2d,%2d)", b2, b3, b4, b5);
			strcat(buf, fStr);
		}
		if (luckyBalls) {
			if ((index = seqSearchXY4(luckyBalls, b2, b3, b4, b5)) < 0) {
				lb = createListX(lb, NULL, NULL, 1);
				appendItem(lb, b2);
				appendItem(lb, b3);
				appendItem(lb, b4);
				appendItem(lb, b5);
				appendList(luckyBalls, lb);
			} else {
				incValXY(luckyBalls, index);
			}
		}
		found=1;
	}
	if (seqSearch(aPrvDrawn, b2) >= 0 && seqSearch(aPrvDrawn, b3) >= 0 && seqSearch(aPrvDrawn, b4) >= 0 && seqSearch(aPrvDrawn, b6) >= 0)
	{
		if (buf) {
			if (found) strcat(buf, ", ");
			sprintf(fStr, "(%2d,%2d,%2d,%2d)", b2, b3, b4, b6);
			strcat(buf, fStr);
		}
		if (luckyBalls) {
			if ((index = seqSearchXY4(luckyBalls, b2, b3, b4, b6)) < 0) {
				lb = createListX(lb, NULL, NULL, 1);
				appendItem(lb, b2);
				appendItem(lb, b3);
				appendItem(lb, b4);
				appendItem(lb, b6);
				appendList(luckyBalls, lb);
			} else {
				incValXY(luckyBalls, index);
			}
		}
		found=1;
	}
	if (seqSearch(aPrvDrawn, b2) >= 0 && seqSearch(aPrvDrawn, b3) >= 0 && seqSearch(aPrvDrawn, b5) >= 0 && seqSearch(aPrvDrawn, b6) >= 0)
	{
		if (buf) {
			if (found) strcat(buf, ", ");
			sprintf(fStr, "(%2d,%2d,%2d,%2d)", b2, b3, b5, b6);
			strcat(buf, fStr);
		}
		if (luckyBalls) {
			if ((index = seqSearchXY4(luckyBalls, b2, b3, b5, b6)) < 0) {
				lb = createListX(lb, NULL, NULL, 1);
				appendItem(lb, b2);
				appendItem(lb, b3);
				appendItem(lb, b5);
				appendItem(lb, b6);
				appendList(luckyBalls, lb);
			} else {
				incValXY(luckyBalls, index);
			}
		}
		found=1;
	}
	if (seqSearch(aPrvDrawn, b2) >= 0 && seqSearch(aPrvDrawn, b4) >= 0 && seqSearch(aPrvDrawn, b5) >= 0 && seqSearch(aPrvDrawn, b6) >= 0)
	{
		if (buf) {
			if (found) strcat(buf, ", ");
			sprintf(fStr, "(%2d,%2d,%2d,%2d)", b2, b4, b5, b6);
			strcat(buf, fStr);
		}
		if (luckyBalls) {
			if ((index = seqSearchXY4(luckyBalls, b2, b4, b5, b6)) < 0) {
				lb = createListX(lb, NULL, NULL, 1);
				appendItem(lb, b2);
				appendItem(lb, b4);
				appendItem(lb, b5);
				appendItem(lb, b6);
				appendList(luckyBalls, lb);
			} else {
				incValXY(luckyBalls, index);
			}
		}
		found=1;
	}
	if (seqSearch(aPrvDrawn, b3) >= 0 && seqSearch(aPrvDrawn, b4) >= 0 && seqSearch(aPrvDrawn, b5) >= 0 && seqSearch(aPrvDrawn, b6) >= 0)
	{
		if (buf) {
			if (found) strcat(buf, ", ");
			sprintf(fStr, "(%2d,%2d,%2d,%2d)", b3, b4, b5, b6);
			strcat(buf, fStr);
		}
		if (luckyBalls) {
			if ((index = seqSearchXY4(luckyBalls, b3, b4, b5, b6)) < 0) {
				lb = createListX(lb, NULL, NULL, 1);
				appendItem(lb, b3);
				appendItem(lb, b4);
				appendItem(lb, b5);
				appendItem(lb, b6);
				appendList(luckyBalls, lb);
			} else {
				incValXY(luckyBalls, index);
			}
		}
		found=1;
	}

	return found;
}



int search3CombXY(struct ListXY *prvDrawnsList, struct ListX *drawnBalls)
{
	int i;
	int b1 = getKey(drawnBalls, 0);
	int b2 = getKey(drawnBalls, 1);
	int b3 = getKey(drawnBalls, 2);
	int b4 = getKey(drawnBalls, 3);
	int b5 = getKey(drawnBalls, 4);
	int b6 = getKey(drawnBalls, 5);
	struct ListX *aPrvDrawn = NULL;
	int found = 0;

	for (i=0; i<lengthY(prvDrawnsList); i++) 
	{
		aPrvDrawn = getListXByIndex(prvDrawnsList, i);
		if (seqSearch(aPrvDrawn, b1) >= 0 && seqSearch(aPrvDrawn, b2) >= 0 && seqSearch(aPrvDrawn, b3) >= 0) {found=1; break;}
		if (seqSearch(aPrvDrawn, b1) >= 0 && seqSearch(aPrvDrawn, b2) >= 0 && seqSearch(aPrvDrawn, b4) >= 0) {found=1; break;}
		if (seqSearch(aPrvDrawn, b1) >= 0 && seqSearch(aPrvDrawn, b2) >= 0 && seqSearch(aPrvDrawn, b5) >= 0) {found=1; break;}
		if (seqSearch(aPrvDrawn, b1) >= 0 && seqSearch(aPrvDrawn, b2) >= 0 && seqSearch(aPrvDrawn, b6) >= 0) {found=1; break;}
		if (seqSearch(aPrvDrawn, b1) >= 0 && seqSearch(aPrvDrawn, b3) >= 0 && seqSearch(aPrvDrawn, b4) >= 0) {found=1; break;}
		if (seqSearch(aPrvDrawn, b1) >= 0 && seqSearch(aPrvDrawn, b3) >= 0 && seqSearch(aPrvDrawn, b5) >= 0) {found=1; break;}
		if (seqSearch(aPrvDrawn, b1) >= 0 && seqSearch(aPrvDrawn, b3) >= 0 && seqSearch(aPrvDrawn, b6) >= 0) {found=1; break;}
		if (seqSearch(aPrvDrawn, b1) >= 0 && seqSearch(aPrvDrawn, b4) >= 0 && seqSearch(aPrvDrawn, b5) >= 0) {found=1; break;}
		if (seqSearch(aPrvDrawn, b1) >= 0 && seqSearch(aPrvDrawn, b4) >= 0 && seqSearch(aPrvDrawn, b6) >= 0) {found=1; break;}
		if (seqSearch(aPrvDrawn, b1) >= 0 && seqSearch(aPrvDrawn, b5) >= 0 && seqSearch(aPrvDrawn, b6) >= 0) {found=1; break;}
		if (seqSearch(aPrvDrawn, b2) >= 0 && seqSearch(aPrvDrawn, b3) >= 0 && seqSearch(aPrvDrawn, b4) >= 0) {found=1; break;}
		if (seqSearch(aPrvDrawn, b2) >= 0 && seqSearch(aPrvDrawn, b3) >= 0 && seqSearch(aPrvDrawn, b5) >= 0) {found=1; break;}
		if (seqSearch(aPrvDrawn, b2) >= 0 && seqSearch(aPrvDrawn, b3) >= 0 && seqSearch(aPrvDrawn, b6) >= 0) {found=1; break;}
		if (seqSearch(aPrvDrawn, b2) >= 0 && seqSearch(aPrvDrawn, b4) >= 0 && seqSearch(aPrvDrawn, b5) >= 0) {found=1; break;}
		if (seqSearch(aPrvDrawn, b2) >= 0 && seqSearch(aPrvDrawn, b4) >= 0 && seqSearch(aPrvDrawn, b6) >= 0) {found=1; break;}
		if (seqSearch(aPrvDrawn, b2) >= 0 && seqSearch(aPrvDrawn, b5) >= 0 && seqSearch(aPrvDrawn, b6) >= 0) {found=1; break;}
		if (seqSearch(aPrvDrawn, b3) >= 0 && seqSearch(aPrvDrawn, b4) >= 0 && seqSearch(aPrvDrawn, b5) >= 0) {found=1; break;}
		if (seqSearch(aPrvDrawn, b3) >= 0 && seqSearch(aPrvDrawn, b4) >= 0 && seqSearch(aPrvDrawn, b6) >= 0) {found=1; break;}
		if (seqSearch(aPrvDrawn, b3) >= 0 && seqSearch(aPrvDrawn, b5) >= 0 && seqSearch(aPrvDrawn, b6) >= 0) {found=1; break;}
		if (seqSearch(aPrvDrawn, b4) >= 0 && seqSearch(aPrvDrawn, b5) >= 0 && seqSearch(aPrvDrawn, b6) >= 0) {found=1; break;}
	}

	return found;
}



int search3CombX(struct ListX *aPrvDrawn, struct ListX *drawnBalls, struct ListXY *luckyBalls, char *buf)
{
	struct ListX *lb = NULL;
	int index;
	int b1 = getKey(drawnBalls, 0);
	int b2 = getKey(drawnBalls, 1);
	int b3 = getKey(drawnBalls, 2);
	int b4 = getKey(drawnBalls, 3);
	int b5 = getKey(drawnBalls, 4);
	int b6 = getKey(drawnBalls, 5);
	int found = 0;
	char fStr[20];

	if (buf) buf[0] = '\0';

	if (seqSearch(aPrvDrawn, b1) >= 0 && seqSearch(aPrvDrawn, b2) >= 0 && seqSearch(aPrvDrawn, b3) >= 0)
	{
		if (buf) {
			sprintf(fStr, "(%2d,%2d,%2d)", b1, b2, b3);
			strcat(buf, fStr);
		}
		if (luckyBalls) {
			if ((index = seqSearchXY3(luckyBalls, b1, b2, b3)) < 0) {
				lb = createListX(lb, NULL, NULL, 1);
				appendItem(lb, b1);
				appendItem(lb, b2);
				appendItem(lb, b3);
				appendList(luckyBalls, lb);
			} else {
				incValXY(luckyBalls, index);
			}
		}
		found=1;
	}
	if (seqSearch(aPrvDrawn, b1) >= 0 && seqSearch(aPrvDrawn, b2) >= 0 && seqSearch(aPrvDrawn, b4) >= 0)
	{
		if (buf) {
			if (found) strcat(buf, ", ");
			sprintf(fStr, "(%2d,%2d,%2d)", b1, b2, b4);
			strcat(buf, fStr);
		}
		if (luckyBalls) {
			if ((index = seqSearchXY3(luckyBalls, b1, b2, b4)) < 0) {
				lb = createListX(lb, NULL, NULL, 1);
				appendItem(lb, b1);
				appendItem(lb, b2);
				appendItem(lb, b4);
				appendList(luckyBalls, lb);
			} else {
				incValXY(luckyBalls, index);
			}
		}
		found=1;
	}
	if (seqSearch(aPrvDrawn, b1) >= 0 && seqSearch(aPrvDrawn, b2) >= 0 && seqSearch(aPrvDrawn, b5) >= 0)
	{
		if (buf) {
			if (found) strcat(buf, ", ");
			sprintf(fStr, "(%2d,%2d,%2d)", b1, b2, b5);
			strcat(buf, fStr);
		}
		if (luckyBalls) {
			if ((index = seqSearchXY3(luckyBalls, b1, b2, b5)) < 0) {
				lb = createListX(lb, NULL, NULL, 1);
				appendItem(lb, b1);
				appendItem(lb, b2);
				appendItem(lb, b5);
				appendList(luckyBalls, lb);
			} else {
				incValXY(luckyBalls, index);
			}
		}
		found=1;
	}
	if (seqSearch(aPrvDrawn, b1) >= 0 && seqSearch(aPrvDrawn, b2) >= 0 && seqSearch(aPrvDrawn, b6) >= 0)
	{
		if (buf) {
			if (found) strcat(buf, ", ");
			sprintf(fStr, "(%2d,%2d,%2d)", b1, b2, b6);
			strcat(buf, fStr);
		}
		if (luckyBalls) {
			if ((index = seqSearchXY3(luckyBalls, b1, b2, b6)) < 0) {
				lb = createListX(lb, NULL, NULL, 1);
				appendItem(lb, b1);
				appendItem(lb, b2);
				appendItem(lb, b6);
				appendList(luckyBalls, lb);
			} else {
				incValXY(luckyBalls, index);
			}
		}
		found=1;
	}
	if (seqSearch(aPrvDrawn, b1) >= 0 && seqSearch(aPrvDrawn, b3) >= 0 && seqSearch(aPrvDrawn, b4) >= 0)
	{
		if (buf) {
			if (found) strcat(buf, ", ");
			sprintf(fStr, "(%2d,%2d,%2d)", b1, b3, b4);
			strcat(buf, fStr);
		}
		if (luckyBalls) {
			if ((index = seqSearchXY3(luckyBalls, b1, b3, b4)) < 0) {
				lb = createListX(lb, NULL, NULL, 1);
				appendItem(lb, b1);
				appendItem(lb, b3);
				appendItem(lb, b4);
				appendList(luckyBalls, lb);
			} else {
				incValXY(luckyBalls, index);
			}
		}
		found=1;
	}
	if (seqSearch(aPrvDrawn, b1) >= 0 && seqSearch(aPrvDrawn, b3) >= 0 && seqSearch(aPrvDrawn, b5) >= 0)
	{
		if (buf) {
			if (found) strcat(buf, ", ");
			sprintf(fStr, "(%2d,%2d,%2d)", b1, b3, b5);
			strcat(buf, fStr);
		}
		if (luckyBalls) {
			if ((index = seqSearchXY3(luckyBalls, b1, b3, b5)) < 0) {
				lb = createListX(lb, NULL, NULL, 1);
				appendItem(lb, b1);
				appendItem(lb, b3);
				appendItem(lb, b5);
				appendList(luckyBalls, lb);
			} else {
				incValXY(luckyBalls, index);
			}
		}
		found=1;
	}
	if (seqSearch(aPrvDrawn, b1) >= 0 && seqSearch(aPrvDrawn, b3) >= 0 && seqSearch(aPrvDrawn, b6) >= 0)
	{
		if (buf) {
			if (found) strcat(buf, ", ");
			sprintf(fStr, "(%2d,%2d,%2d)", b1, b3, b6);
			strcat(buf, fStr);
		}
		if (luckyBalls) {
			if ((index = seqSearchXY3(luckyBalls, b1, b3, b6)) < 0) {
				lb = createListX(lb, NULL, NULL, 1);
				appendItem(lb, b1);
				appendItem(lb, b3);
				appendItem(lb, b6);
				appendList(luckyBalls, lb);
			} else {
				incValXY(luckyBalls, index);
			}
		}
		found=1;
	}
	if (seqSearch(aPrvDrawn, b1) >= 0 && seqSearch(aPrvDrawn, b4) >= 0 && seqSearch(aPrvDrawn, b5) >= 0)
	{
		if (buf) {
			if (found) strcat(buf, ", ");
			sprintf(fStr, "(%2d,%2d,%2d)", b1, b4, b5);
			strcat(buf, fStr);
		}
		if (luckyBalls) {
			if ((index = seqSearchXY3(luckyBalls, b1, b4, b5)) < 0) {
				lb = createListX(lb, NULL, NULL, 1);
				appendItem(lb, b1);
				appendItem(lb, b4);
				appendItem(lb, b5);
				appendList(luckyBalls, lb);
			} else {
				incValXY(luckyBalls, index);
			}
		}
		found=1;
	}
	if (seqSearch(aPrvDrawn, b1) >= 0 && seqSearch(aPrvDrawn, b4) >= 0 && seqSearch(aPrvDrawn, b6) >= 0)
	{
		if (buf) {
			if (found) strcat(buf, ", ");
			sprintf(fStr, "(%2d,%2d,%2d)", b1, b4, b6);
			strcat(buf, fStr);
		}
		if (luckyBalls) {
			if ((index = seqSearchXY3(luckyBalls, b1, b4, b6)) < 0) {
				lb = createListX(lb, NULL, NULL, 1);
				appendItem(lb, b1);
				appendItem(lb, b4);
				appendItem(lb, b6);
				appendList(luckyBalls, lb);
			} else {
				incValXY(luckyBalls, index);
			}
		}
		found=1;
	}
	if (seqSearch(aPrvDrawn, b1) >= 0 && seqSearch(aPrvDrawn, b5) >= 0 && seqSearch(aPrvDrawn, b6) >= 0)
	{
		if (buf) {
			if (found) strcat(buf, ", ");
			sprintf(fStr, "(%2d,%2d,%2d)", b1, b5, b6);
			strcat(buf, fStr);
		}
		if (luckyBalls) {
			if ((index = seqSearchXY3(luckyBalls, b1, b5, b6)) < 0) {
				lb = createListX(lb, NULL, NULL, 1);
				appendItem(lb, b1);
				appendItem(lb, b5);
				appendItem(lb, b6);
				appendList(luckyBalls, lb);
			} else {
				incValXY(luckyBalls, index);
			}
		}
		found=1;
	}
	if (seqSearch(aPrvDrawn, b2) >= 0 && seqSearch(aPrvDrawn, b3) >= 0 && seqSearch(aPrvDrawn, b4) >= 0)
	{
		if (buf) {
			if (found) strcat(buf, ", ");
			sprintf(fStr, "(%2d,%2d,%2d)", b2, b3, b4);
			strcat(buf, fStr);
		}
		if (luckyBalls) {
			if ((index = seqSearchXY3(luckyBalls, b2, b3, b4)) < 0) {
				lb = createListX(lb, NULL, NULL, 1);
				appendItem(lb, b2);
				appendItem(lb, b3);
				appendItem(lb, b4);
				appendList(luckyBalls, lb);
			} else {
				incValXY(luckyBalls, index);
			}
		}
		found=1;
	}
	if (seqSearch(aPrvDrawn, b2) >= 0 && seqSearch(aPrvDrawn, b3) >= 0 && seqSearch(aPrvDrawn, b5) >= 0)
	{
		if (buf) {
			if (found) strcat(buf, ", ");
			sprintf(fStr, "(%2d,%2d,%2d)", b2, b3, b5);
			strcat(buf, fStr);
		}
		if (luckyBalls) {
			if ((index = seqSearchXY3(luckyBalls, b2, b3, b5)) < 0) {
				lb = createListX(lb, NULL, NULL, 1);
				appendItem(lb, b2);
				appendItem(lb, b3);
				appendItem(lb, b5);
				appendList(luckyBalls, lb);
			} else {
				incValXY(luckyBalls, index);
			}
		}
		found=1;
	}
	if (seqSearch(aPrvDrawn, b2) >= 0 && seqSearch(aPrvDrawn, b3) >= 0 && seqSearch(aPrvDrawn, b6) >= 0)
	{
		if (buf) {
			if (found) strcat(buf, ", ");
			sprintf(fStr, "(%2d,%2d,%2d)", b2, b3, b6);
			strcat(buf, fStr);
		}
		if (luckyBalls) {
			if ((index = seqSearchXY3(luckyBalls, b2, b3, b6)) < 0) {
				lb = createListX(lb, NULL, NULL, 1);
				appendItem(lb, b2);
				appendItem(lb, b3);
				appendItem(lb, b6);
				appendList(luckyBalls, lb);
			} else {
				incValXY(luckyBalls, index);
			}
		}
		found=1;
	}
	if (seqSearch(aPrvDrawn, b2) >= 0 && seqSearch(aPrvDrawn, b4) >= 0 && seqSearch(aPrvDrawn, b5) >= 0)
	{
		if (buf) {
			if (found) strcat(buf, ", ");
			sprintf(fStr, "(%2d,%2d,%2d)", b2, b4, b5);
			strcat(buf, fStr);
		}
		if (luckyBalls) {
			if ((index = seqSearchXY3(luckyBalls, b2, b4, b5)) < 0) {
				lb = createListX(lb, NULL, NULL, 1);
				appendItem(lb, b2);
				appendItem(lb, b4);
				appendItem(lb, b5);
				appendList(luckyBalls, lb);
			} else {
				incValXY(luckyBalls, index);
			}
		}
		found=1;
	}
	if (seqSearch(aPrvDrawn, b2) >= 0 && seqSearch(aPrvDrawn, b4) >= 0 && seqSearch(aPrvDrawn, b6) >= 0)
	{
		if (buf) {
			if (found) strcat(buf, ", ");
			sprintf(fStr, "(%2d,%2d,%2d)", b2, b4, b6);
			strcat(buf, fStr);
		}
		if (luckyBalls) {
			if ((index = seqSearchXY3(luckyBalls, b2, b4, b6)) < 0) {
				lb = createListX(lb, NULL, NULL, 1);
				appendItem(lb, b2);
				appendItem(lb, b4);
				appendItem(lb, b6);
				appendList(luckyBalls, lb);
			} else {
				incValXY(luckyBalls, index);
			}
		}
		found=1;
	}
	if (seqSearch(aPrvDrawn, b2) >= 0 && seqSearch(aPrvDrawn, b5) >= 0 && seqSearch(aPrvDrawn, b6) >= 0)
	{
		if (buf) {
			if (found) strcat(buf, ", ");
			sprintf(fStr, "(%2d,%2d,%2d)", b2, b5, b6);
			strcat(buf, fStr);
		}
		if (luckyBalls) {
			if ((index = seqSearchXY3(luckyBalls, b2, b5, b6)) < 0) {
				lb = createListX(lb, NULL, NULL, 1);
				appendItem(lb, b2);
				appendItem(lb, b5);
				appendItem(lb, b6);
				appendList(luckyBalls, lb);
			} else {
				incValXY(luckyBalls, index);
			}
		}
		found=1;
	}
	if (seqSearch(aPrvDrawn, b3) >= 0 && seqSearch(aPrvDrawn, b4) >= 0 && seqSearch(aPrvDrawn, b5) >= 0)
	{
		if (buf) {
			if (found) strcat(buf, ", ");
			sprintf(fStr, "(%2d,%2d,%2d)", b3, b4, b5);
			strcat(buf, fStr);
		}
		if (luckyBalls) {
			if ((index = seqSearchXY3(luckyBalls, b3, b4, b5)) < 0) {
				lb = createListX(lb, NULL, NULL, 1);
				appendItem(lb, b3);
				appendItem(lb, b4);
				appendItem(lb, b5);
				appendList(luckyBalls, lb);
			} else {
				incValXY(luckyBalls, index);
			}
		}
		found=1;
	}
	if (seqSearch(aPrvDrawn, b3) >= 0 && seqSearch(aPrvDrawn, b4) >= 0 && seqSearch(aPrvDrawn, b6) >= 0)
	{
		if (buf) {
			if (found) strcat(buf, ", ");
			sprintf(fStr, "(%2d,%2d,%2d)", b3, b4, b6);
			strcat(buf, fStr);
		}
		if (luckyBalls) {
			if ((index = seqSearchXY3(luckyBalls, b3, b4, b6)) < 0) {
				lb = createListX(lb, NULL, NULL, 1);
				appendItem(lb, b3);
				appendItem(lb, b4);
				appendItem(lb, b6);
				appendList(luckyBalls, lb);
			} else {
				incValXY(luckyBalls, index);
			}
		}
		found=1;
	}
	if (seqSearch(aPrvDrawn, b3) >= 0 && seqSearch(aPrvDrawn, b5) >= 0 && seqSearch(aPrvDrawn, b6) >= 0)
	{
		if (buf) {
			if (found) strcat(buf, ", ");
			sprintf(fStr, "(%2d,%2d,%2d)", b3, b5, b6);
			strcat(buf, fStr);
		}
		if (luckyBalls) {
			if ((index = seqSearchXY3(luckyBalls, b3, b5, b6)) < 0) {
				lb = createListX(lb, NULL, NULL, 1);
				appendItem(lb, b3);
				appendItem(lb, b5);
				appendItem(lb, b6);
				appendList(luckyBalls, lb);
			} else {
				incValXY(luckyBalls, index);
			}
		}
		found=1;
	}
	if (seqSearch(aPrvDrawn, b4) >= 0 && seqSearch(aPrvDrawn, b5) >= 0 && seqSearch(aPrvDrawn, b6) >= 0)
	{
		if (buf) {
			if (found) strcat(buf, ", ");
			sprintf(fStr, "(%2d,%2d,%2d)", b4, b5, b6);
			strcat(buf, fStr);
		}
		if (luckyBalls) {
			if ((index = seqSearchXY3(luckyBalls, b4, b5, b6)) < 0) {
				lb = createListX(lb, NULL, NULL, 1);
				appendItem(lb, b4);
				appendItem(lb, b5);
				appendItem(lb, b6);
				appendList(luckyBalls, lb);
			} else {
				incValXY(luckyBalls, index);
			}
		}
		found=1;
	}

	return found;
}



int search2CombXY(struct ListXY *prvDrawnsList, struct ListX *drawnBalls)
{
	int i;
	int b1 = getKey(drawnBalls, 0);
	int b2 = getKey(drawnBalls, 1);
	int b3 = getKey(drawnBalls, 2);
	int b4 = getKey(drawnBalls, 3);
	int b5 = getKey(drawnBalls, 4);
	int b6 = getKey(drawnBalls, 5);
	struct ListX *aPrvDrawn = NULL;
	int found = 0;

	for (i=0; i<lengthY(prvDrawnsList); i++) 
	{
		aPrvDrawn = getListXByIndex(prvDrawnsList, i);
		if (seqSearch(aPrvDrawn, b1) >= 0 && seqSearch(aPrvDrawn, b2) >= 0) {found=1; break;}
		if (seqSearch(aPrvDrawn, b1) >= 0 && seqSearch(aPrvDrawn, b3) >= 0) {found=1; break;}
		if (seqSearch(aPrvDrawn, b1) >= 0 && seqSearch(aPrvDrawn, b4) >= 0) {found=1; break;}
		if (seqSearch(aPrvDrawn, b1) >= 0 && seqSearch(aPrvDrawn, b5) >= 0) {found=1; break;}
		if (seqSearch(aPrvDrawn, b1) >= 0 && seqSearch(aPrvDrawn, b6) >= 0) {found=1; break;}
		if (seqSearch(aPrvDrawn, b2) >= 0 && seqSearch(aPrvDrawn, b3) >= 0) {found=1; break;}
		if (seqSearch(aPrvDrawn, b2) >= 0 && seqSearch(aPrvDrawn, b4) >= 0) {found=1; break;}
		if (seqSearch(aPrvDrawn, b2) >= 0 && seqSearch(aPrvDrawn, b5) >= 0) {found=1; break;}
		if (seqSearch(aPrvDrawn, b2) >= 0 && seqSearch(aPrvDrawn, b6) >= 0) {found=1; break;}
		if (seqSearch(aPrvDrawn, b3) >= 0 && seqSearch(aPrvDrawn, b4) >= 0) {found=1; break;}
		if (seqSearch(aPrvDrawn, b3) >= 0 && seqSearch(aPrvDrawn, b5) >= 0) {found=1; break;}
		if (seqSearch(aPrvDrawn, b3) >= 0 && seqSearch(aPrvDrawn, b6) >= 0) {found=1; break;}
		if (seqSearch(aPrvDrawn, b4) >= 0 && seqSearch(aPrvDrawn, b5) >= 0) {found=1; break;}
		if (seqSearch(aPrvDrawn, b4) >= 0 && seqSearch(aPrvDrawn, b6) >= 0) {found=1; break;}
		if (seqSearch(aPrvDrawn, b5) >= 0 && seqSearch(aPrvDrawn, b6) >= 0) {found=1; break;}
	}

	return found;
}



int search2CombX(struct ListX *aPrvDrawn, struct ListX *drawnBalls, struct ListXY *luckyBalls, char *buf)
{
	struct ListX *lb = NULL;
	int index;
	int b1 = getKey(drawnBalls, 0);
	int b2 = getKey(drawnBalls, 1);
	int b3 = getKey(drawnBalls, 2);
	int b4 = getKey(drawnBalls, 3);
	int b5 = getKey(drawnBalls, 4);
	int b6 = getKey(drawnBalls, 5);
	int found = 0;
	char fStr[20];

	if (buf) buf[0] = '\0';

	if (seqSearch(aPrvDrawn, b1) >= 0 && seqSearch(aPrvDrawn, b2) >= 0)
	{
		if (buf) {
			sprintf(fStr, "(%2d,%2d)", b1, b2);
			strcat(buf, fStr);
		}
		if (luckyBalls) {
			if ((index = seqSearchXY2(luckyBalls, b1, b2)) < 0) {
				lb = createListX(lb, NULL, NULL, 1);
				appendItem(lb, b1);
				appendItem(lb, b2);
				appendList(luckyBalls, lb);
			} else {
				incValXY(luckyBalls, index);
			}
		}
		found=1;
	}
	if (seqSearch(aPrvDrawn, b1) >= 0 && seqSearch(aPrvDrawn, b3) >= 0)
	{
		if (buf) {
			if (found) strcat(buf, ", ");
			sprintf(fStr, "(%2d,%2d)", b1, b3);
			strcat(buf, fStr);
		}
		if (luckyBalls) {
			if ((index = seqSearchXY2(luckyBalls, b1, b3)) < 0) {
				lb = createListX(lb, NULL, NULL, 1);
				appendItem(lb, b1);
				appendItem(lb, b3);
				appendList(luckyBalls, lb);
			} else {
				incValXY(luckyBalls, index);
			}
		}
		found=1;
	}
	if (seqSearch(aPrvDrawn, b1) >= 0 && seqSearch(aPrvDrawn, b4) >= 0)
	{
		if (buf) {
			if (found) strcat(buf, ", ");
			sprintf(fStr, "(%2d,%2d)", b1, b4);
			strcat(buf, fStr);
		}
		if (luckyBalls) {
			if ((index = seqSearchXY2(luckyBalls, b1, b4)) < 0) {
				lb = createListX(lb, NULL, NULL, 1);
				appendItem(lb, b1);
				appendItem(lb, b4);
				appendList(luckyBalls, lb);
			} else {
				incValXY(luckyBalls, index);
			}
		}
		found=1;
	}
	if (seqSearch(aPrvDrawn, b1) >= 0 && seqSearch(aPrvDrawn, b5) >= 0)
	{
		if (buf) {
			if (found) strcat(buf, ", ");
			sprintf(fStr, "(%2d,%2d)", b1, b5);
			strcat(buf, fStr);
		}
		if (luckyBalls) {
			if ((index = seqSearchXY2(luckyBalls, b1, b5)) < 0) {
				lb = createListX(lb, NULL, NULL, 1);
				appendItem(lb, b1);
				appendItem(lb, b5);
				appendList(luckyBalls, lb);
			} else {
				incValXY(luckyBalls, index);
			}
		}
		found=1;
	}
	if (seqSearch(aPrvDrawn, b1) >= 0 && seqSearch(aPrvDrawn, b6) >= 0)
	{
		if (buf) {
			if (found) strcat(buf, ", ");
			sprintf(fStr, "(%2d,%2d)", b1, b6);
			strcat(buf, fStr);
		}
		if (luckyBalls) {
			if ((index = seqSearchXY2(luckyBalls, b1, b6)) < 0) {
				lb = createListX(lb, NULL, NULL, 1);
				appendItem(lb, b1);
				appendItem(lb, b6);
				appendList(luckyBalls, lb);
			} else {
				incValXY(luckyBalls, index);
			}
		}
		found=1;
	}
	if (seqSearch(aPrvDrawn, b2) >= 0 && seqSearch(aPrvDrawn, b3) >= 0)
	{
		if (buf) {
			if (found) strcat(buf, ", ");
			sprintf(fStr, "(%2d,%2d)", b2, b3);
			strcat(buf, fStr);
		}
		if (luckyBalls) {
			if ((index = seqSearchXY2(luckyBalls, b2, b3)) < 0) {
				lb = createListX(lb, NULL, NULL, 1);
				appendItem(lb, b2);
				appendItem(lb, b3);
				appendList(luckyBalls, lb);
			} else {
				incValXY(luckyBalls, index);
			}
		}
		found=1;
	}
	if (seqSearch(aPrvDrawn, b2) >= 0 && seqSearch(aPrvDrawn, b4) >= 0)
	{
		if (buf) {
			if (found) strcat(buf, ", ");
			sprintf(fStr, "(%2d,%2d)", b2, b4);
			strcat(buf, fStr);
		}
		if (luckyBalls) {
			if ((index = seqSearchXY2(luckyBalls, b2, b4)) < 0) {
				lb = createListX(lb, NULL, NULL, 1);
				appendItem(lb, b2);
				appendItem(lb, b4);
				appendList(luckyBalls, lb);
			} else {
				incValXY(luckyBalls, index);
			}
		}
		found=1;
	}
	if (seqSearch(aPrvDrawn, b2) >= 0 && seqSearch(aPrvDrawn, b5) >= 0)
	{
		if (buf) {
			if (found) strcat(buf, ", ");
			sprintf(fStr, "(%2d,%2d)", b2, b5);
			strcat(buf, fStr);
		}
		if (luckyBalls) {
			if ((index = seqSearchXY2(luckyBalls, b2, b5)) < 0) {
				lb = createListX(lb, NULL, NULL, 1);
				appendItem(lb, b2);
				appendItem(lb, b5);
				appendList(luckyBalls, lb);
			} else {
				incValXY(luckyBalls, index);
			}
		}
		found=1;
	}
	if (seqSearch(aPrvDrawn, b2) >= 0 && seqSearch(aPrvDrawn, b6) >= 0)
	{
		if (buf) {
			if (found) strcat(buf, ", ");
			sprintf(fStr, "(%2d,%2d)", b2, b6);
			strcat(buf, fStr);
		}
		if (luckyBalls) {
			if ((index = seqSearchXY2(luckyBalls, b2, b6)) < 0) {
				lb = createListX(lb, NULL, NULL, 1);
				appendItem(lb, b2);
				appendItem(lb, b6);
				appendList(luckyBalls, lb);
			} else {
				incValXY(luckyBalls, index);
			}
		}
		found=1;
	}
	if (seqSearch(aPrvDrawn, b3) >= 0 && seqSearch(aPrvDrawn, b4) >= 0)
	{
		if (buf) {
			if (found) strcat(buf, ", ");
			sprintf(fStr, "(%2d,%2d)", b3, b4);
			strcat(buf, fStr);
		}
		if (luckyBalls) {
			if ((index = seqSearchXY2(luckyBalls, b3, b4)) < 0) {
				lb = createListX(lb, NULL, NULL, 1);
				appendItem(lb, b3);
				appendItem(lb, b4);
				appendList(luckyBalls, lb);
			} else {
				incValXY(luckyBalls, index);
			}
		}
		found=1;
	}
	if (seqSearch(aPrvDrawn, b3) >= 0 && seqSearch(aPrvDrawn, b5) >= 0)
	{
		if (buf) {
			if (found) strcat(buf, ", ");
			sprintf(fStr, "(%2d,%2d)", b3, b5);
			strcat(buf, fStr);
		}
		if (luckyBalls) {
			if ((index = seqSearchXY2(luckyBalls, b3, b5)) < 0) {
				lb = createListX(lb, NULL, NULL, 1);
				appendItem(lb, b3);
				appendItem(lb, b5);
				appendList(luckyBalls, lb);
			} else {
				incValXY(luckyBalls, index);
			}
		}
		found=1;
	}
	if (seqSearch(aPrvDrawn, b3) >= 0 && seqSearch(aPrvDrawn, b6) >= 0)
	{
		if (buf) {
			if (found) strcat(buf, ", ");
			sprintf(fStr, "(%2d,%2d)", b3, b6);
			strcat(buf, fStr);
		}
		if (luckyBalls) {
			if ((index = seqSearchXY2(luckyBalls, b3, b6)) < 0) {
				lb = createListX(lb, NULL, NULL, 1);
				appendItem(lb, b3);
				appendItem(lb, b6);
				appendList(luckyBalls, lb);
			} else {
				incValXY(luckyBalls, index);
			}
		}
		found=1;
	}
	if (seqSearch(aPrvDrawn, b4) >= 0 && seqSearch(aPrvDrawn, b5) >= 0)
	{
		if (buf) {
			if (found) strcat(buf, ", ");
			sprintf(fStr, "(%2d,%2d)", b4, b5);
			strcat(buf, fStr);
		}
		if (luckyBalls) {
			if ((index = seqSearchXY2(luckyBalls, b4, b5)) < 0) {
				lb = createListX(lb, NULL, NULL, 1);
				appendItem(lb, b4);
				appendItem(lb, b5);
				appendList(luckyBalls, lb);
			} else {
				incValXY(luckyBalls, index);
			}
		}
		found=1;
	}
	if (seqSearch(aPrvDrawn, b4) >= 0 && seqSearch(aPrvDrawn, b6) >= 0)
	{
		if (buf) {
			if (found) strcat(buf, ", ");
			sprintf(fStr, "(%2d,%2d)", b4, b6);
			strcat(buf, fStr);
		}
		if (luckyBalls) {
			if ((index = seqSearchXY2(luckyBalls, b4, b6)) < 0) {
				lb = createListX(lb, NULL, NULL, 1);
				appendItem(lb, b4);
				appendItem(lb, b6);
				appendList(luckyBalls, lb);
			} else {
				incValXY(luckyBalls, index);
			}
		}
		found=1;
	}
	if (seqSearch(aPrvDrawn, b5) >= 0 && seqSearch(aPrvDrawn, b6) >= 0)
	{
		if (buf) {
			if (found) strcat(buf, ", ");
			sprintf(fStr, "(%2d,%2d)", b5, b6);
			strcat(buf, fStr);
		}
		if (luckyBalls) {
			if ((index = seqSearchXY2(luckyBalls, b5, b6)) < 0) {
				lb = createListX(lb, NULL, NULL, 1);
				appendItem(lb, b5);
				appendItem(lb, b6);
				appendList(luckyBalls, lb);
			} else {
				incValXY(luckyBalls, index);
			}
		}
		found=1;
	}

	return found;
}



int search1BallXY(struct ListXY *couponList, struct ListX *drawnBalls, int drawBall)
{
	int i, j;
	struct ListX *aPrvDrawn = NULL;
	int ball[DRAW_BALL];
	int found = 0;
	
	for (j=0; j<drawBall; j++) {
		ball[j] = getKey(drawnBalls, j);
	}
	
	for (i=0; i<lengthY(couponList); i++) {
		aPrvDrawn = getListXByIndex(couponList, i);
		for (j=0; j<drawBall; j++) {
			if (seqSearch(aPrvDrawn, ball[j]) >= 0) {found=1; break;}
		}
		if (found) break;
	}

	return found;
}



void init()
{
    int err;
    char realPath[PATH_MAX];
    
    cwd = (char *) malloc(sizeof(char)*PATH_MAX);
    fileStats = (char *) malloc(sizeof(char)*PATH_MAX);
    outputFile = (char *) malloc(sizeof(char)*PATH_MAX);

    fileStats[0] = '\0';
    outputFile[0] = '\0';

#if defined(__linux__) || defined(__unix__) || defined(__APPLE__) || defined(SOLARIS) || defined(WIN32)
    err = get_app_path(realPath, PATH_MAX);
    if (err) puts("App path lookup failed!");
    else {
        cwd = dirname(realPath);

		strcat(fileStats, cwd);
    	strcat(fileStats, PATH_SEPARATOR);

    	strcat(outputFile, cwd);
    	strcat(outputFile, PATH_SEPARATOR);
    }
#endif

    strcat(fileStats, FILESTATS);
    strcat(outputFile, OUTPUTFILE);

    winningDrawnBallsList = createListXY(winningDrawnBallsList);
	superStarDrawnBallsList = createListXY(superStarDrawnBallsList);

	getDrawnBallsList(winningDrawnBallsList, superStarDrawnBallsList, fileStats);
    calcMatchCombCount();

	clearScreen();
}



struct ListXY * getLuckyBalls(struct ListXY *luckyBalls, int comb)
{
	int i, j;
	struct ListX *aPrvDrawn1 = NULL;
	struct ListX *aPrvDrawn2 = NULL;

    if (comb == 2 || comb == 3 || comb == 4) {
    	for (i=0; i<lengthY(winningDrawnBallsList); i++) {
	    	aPrvDrawn1 = getListXByIndex(winningDrawnBallsList, i);
		    for (j=i+1; j<lengthY(winningDrawnBallsList); j++) {
			    aPrvDrawn2 = getListXByIndex(winningDrawnBallsList, j);
                if (comb == 2) {
                    search2CombX(aPrvDrawn2, aPrvDrawn1, luckyBalls, NULL);
                }
                else if (comb == 3) {
                    search3CombX(aPrvDrawn2, aPrvDrawn1, luckyBalls, NULL);
                }
                else if (comb == 4) {
                    search4CombX(aPrvDrawn2, aPrvDrawn1, luckyBalls, NULL);
                }
		    }
	    }
    }

    return luckyBalls;
}



void calcMatchCombCount()
{
	int i, j;
	struct ListX *aPrvDrawn1 = NULL;
	struct ListX *aPrvDrawn2 = NULL;

    printf("Initializing... Please wait.\n");

   	for (i=0; i<lengthY(winningDrawnBallsList); i++) {
    	aPrvDrawn1 = getListXByIndex(winningDrawnBallsList, i);
	    for (j=i+1; j<lengthY(winningDrawnBallsList); j++) {
		    aPrvDrawn2 = getListXByIndex(winningDrawnBallsList, j);
   			match2comb += search2CombX(aPrvDrawn2, aPrvDrawn1, NULL, NULL);
    		match3comb += search3CombX(aPrvDrawn2, aPrvDrawn1, NULL, NULL);
	    	match4comb += search4CombX(aPrvDrawn2, aPrvDrawn1, NULL, NULL);
		    match5comb += search5CombX(aPrvDrawn2, aPrvDrawn1, NULL);
   			match6comb += search6CombX(aPrvDrawn2, aPrvDrawn1);
    	}
    }
}



void calcCombMatch(int comb, FILE *fp)
{
	int i, j;
    int found;
	struct ListX *aPrvDrawn1 = NULL;
	struct ListX *aPrvDrawn2 = NULL;

	char buf[100];
	char date1[11], date2[11];
	char lastDate[11];
	int d1, m1, y1;
	int d2, m2, y2;
	char *ds1, *ms1, *ys1;
	char *ds2, *ms2, *ys2;

    if (comb == 2 || comb == 3 || comb == 4 || comb == 5 || comb == 6) 
    {
        printf("Matched combinations of numbers from previous draws:\n\n");
        if (fp != NULL) fprintf(fp, "Matched combinations of numbers from previous draws:\n\n");
        if (comb == 2) {
        	printf("Matched 2 combinations: %u\n\n", match2comb);
        	if (fp != NULL) fprintf(fp, "Matched 2 combinations: %u\n\n", match2comb);
        }
        else if (comb == 3) {
    	    printf("Matched 3 combinations: %u\n\n", match3comb);
    	    if (fp != NULL) fprintf(fp, "Matched 3 combinations: %u\n\n", match3comb);
        }
        else if (comb == 4) {
    	    printf("Matched 4 combinations: %u\n\n", match4comb);
    	    if (fp != NULL) fprintf(fp, "Matched 4 combinations: %u\n\n", match4comb);
        }
        else if (comb == 5) {
    	    printf("Matched 5 combinations: %u\n\n", match5comb);
    	    if (fp != NULL) fprintf(fp, "Matched 5 combinations: %u\n\n", match5comb);
        }
        else if (comb == 6) {
    	    printf("Matched 6 combinations: %u\n\n", match6comb);
    	    if (fp != NULL) fprintf(fp, "Matched 6 combinations: %u\n\n", match6comb);
        }

    	for (i=0; i<lengthY(winningDrawnBallsList); i++) {
	    	aPrvDrawn1 = getListXByIndex(winningDrawnBallsList, i);
		    for (j=i+1; j<lengthY(winningDrawnBallsList); j++) {
			    aPrvDrawn2 = getListXByIndex(winningDrawnBallsList, j);
                if (comb == 2) {
                    found = search2CombX(aPrvDrawn2, aPrvDrawn1, NULL, buf);
                }
                else if (comb == 3) {
                    found = search3CombX(aPrvDrawn2, aPrvDrawn1, NULL, buf);
                }
                else if (comb == 4) {
                    found = search4CombX(aPrvDrawn2, aPrvDrawn1, NULL, buf);
                }
                else if (comb == 5) {
                    found = search5CombX(aPrvDrawn2, aPrvDrawn1, NULL);
                }
                else if (comb == 6) {
                    found = search6CombX(aPrvDrawn2, aPrvDrawn1);
                }
    			if (found) {
	    			strcpy(date1, aPrvDrawn1->date);
		    		if (strcmp(lastDate, date1) != 0) {
			    		if (comb >= 4) printf("\n-------------------------------------------------------------------------------------------------");
			    		if (fp != NULL) fprintf(fp, "\n-------------------------------------------------------------------------------------------------");
				    	if (comb >= 4) printf("\n\n%s : ", date1);
				    	if (fp != NULL) fprintf(fp, "\n\n%s : ", date1);

    					if (comb >= 4) printListXByKey(aPrvDrawn1, 0, fp);
                        else  printListXByKey(aPrvDrawn1, 2, fp);

	    				ds1 = strtok(date1, ".");
		    			ms1 = strtok(NULL, ".");
			    		ys1 = strtok(NULL, ".");
				    	d1 = atoi(ds1);
    					m1 = atoi(ms1);
	    				y1 = atoi(ys1);
		    			strcpy(date1, aPrvDrawn1->date);
			    	}
				    else {
					    if (comb >= 4) printf("                               ");
					    if (fp != NULL) fprintf(fp, "                               ");
	    			}
		    		if (comb >= 4) printf("    ");
		    		if (fp != NULL) fprintf(fp, "    ");
			    	strcpy(date2, aPrvDrawn2->date);
				    if (comb >= 4) printf("%s : ", date2);
				    if (fp != NULL) fprintf(fp, "%s : ", date2);

   					if (comb >= 4) printListXByKey(aPrvDrawn2, 0, fp);
                    else  printListXByKey(aPrvDrawn2, 2, fp);

	    			ds2 = strtok(date2, ".");
		    		ms2 = strtok(NULL, ".");
			    	ys2 = strtok(NULL, ".");
				    d2 = atoi(ds2);
    				m2 = atoi(ms2);
	    			y2 = atoi(ys2);
		    		if (comb >= 4) printf("   %4d days", dateDiff(d2, m2, y2, d1, m1, y1));
		    		if (fp != NULL) fprintf(fp, "   %4d days", dateDiff(d2, m2, y2, d1, m1, y1));
			    	if (comb != 5) {
                        if (comb >= 4) printf("   %s", buf);
                        if (fp != NULL) fprintf(fp, "   %s", buf);
                    }
				    if (comb >= 4) printf("\n");
				    if (fp != NULL) fprintf(fp, "\n");
    				strcpy(lastDate, date1);
	    		}
		    }
	    }
    	if (comb >= 4) printf("\n-------------------------------------------------------------------------------------------------\n\n");
    	if (fp != NULL) fprintf(fp, "\n-------------------------------------------------------------------------------------------------\n\n");
    }
}



void drawBallsByLucky(struct ListXY *coupon, int drawCount, int totalDrawCount)
{
	int i, j, k;
    int index;
    int ind1, ind2, ind3;
    int ball1, ball2, ball3;
    char label[30];
	struct ListXY *luckyBalls2 = NULL;
	struct ListXY *luckyBalls3 = NULL;
	struct ListXY *luckyBalls2Sorted = NULL;
	struct ListXY *luckyBalls3Sorted = NULL;
	struct ListX *luckyRow2 = NULL;
	struct ListX *luckyRow3 = NULL;
	struct ListX *drawRow = NULL;

	struct ListX *winningBallStats = NULL;
	struct ListX *superStarBallStats = NULL;

	struct ListX *drawnBallsNorm = NULL;

	strcpy(label, "(normal distribution)");
    drawnBallsNorm = createListX(drawnBallsNorm, NULL, label, 0);

	winningBallStats = createListX(winningBallStats, NULL, NULL, 0);
	superStarBallStats = createListX(superStarBallStats, NULL, NULL, 0);

	getDrawnBallsStats(winningBallStats, superStarBallStats);

    luckyBalls2 = createListXY(luckyBalls2);
    luckyBalls2 = getLuckyBalls(luckyBalls2, 2);

    luckyBalls2Sorted = createListXY(luckyBalls2Sorted);
    luckyBalls2Sorted = getLuckyBalls(luckyBalls2Sorted, 2);

    luckyBalls3 = createListXY(luckyBalls3);
    luckyBalls3 = getLuckyBalls(luckyBalls3, 3);

    luckyBalls3Sorted = createListXY(luckyBalls3Sorted);
    luckyBalls3Sorted = getLuckyBalls(luckyBalls3Sorted, 3);

    bubbleSortYByVal(luckyBalls2Sorted, -1);
    bubbleSortYByVal(luckyBalls3Sorted, -1);

    for (i=0; i<drawCount;) 
    {
        if (i == 0) strcpy(label, "(lucky 3)");
        else if (i == 1) strcpy(label, "(2 of lucky 3)");
        else strcpy(label, "(lucky 2)");

        if (i == 0) k = 3;
        else k = 2;
   	    drawRow = createListX(drawRow, NULL, label, 0);
        if (i < 2) {
            luckyRow3 = getListXByIndex(luckyBalls3Sorted, rand()%10);
            ind1 = rand()%3;
            if (ind1 == 0) {
                ind2 = rand()%2 +1;
                if (ind2 == 1) ind3 = 2;
                else ind3 = 1;
            }
            else if (ind1 == 1) {
                ind2 = 0;
                ind3 = 2;
            }
            else {
                ind2 = rand()%2;
                if (ind2 == 0) ind3 = 1;
                else ind3 = 0;
            }
            ball1 = getKey(luckyRow3, ind1);
            ball2 = getKey(luckyRow3, ind2);
            ball3 = getKey(luckyRow3, ind3);
            appendItem(drawRow, ball1);
            if (i == 0) appendItem(drawRow, ball2);
            appendItem(drawRow, ball3);
        } else {
            if (i < 2) {
                luckyRow2 = getListXByIndex(luckyBalls2Sorted, rand()%10);
            } else {
                do {
                    index = rand() % lengthY(luckyBalls2);
                    luckyRow2 = getListXByIndex(luckyBalls2, index);
                } while (luckyRow2->val < ceil(180/TOTAL_BALL)+1);
                //} while (0);
            }
            ind1 = rand()%2;
            if (ind1 == 0) ind2 = 1;
            else ind2 = 0;
            ball1 = getKey(luckyRow2, ind1);
            ball2 = getKey(luckyRow2, ind2);
            appendItem(drawRow, ball1);
            appendItem(drawRow, ball2);
        }

        for (j=0; j<DRAW_BALL-k; j++)
        {
            do {
                do {
                    index = rand() % lengthY(luckyBalls2);
                    luckyRow2 = getListXByIndex(luckyBalls2, index);
                } while ((ind1 = seqSearch(luckyRow2, ball2)) < 0 || luckyRow2->val < ceil(180/TOTAL_BALL)+1);
                //} while ((ind1 = seqSearch(luckyRow2, ball2)) < 0 );
                if (ind1 == 0) ind2 = 1;
                else ind2 = 0;
                ball2 = getKey(luckyRow2, ind2);
            } while (seqSearch(drawRow, ball2) >= 0);
            appendItem(drawRow, ball2);
        }
        
        if (lengthY(coupon) < ceil(totalDrawCount/3)) {
			if (search2CombXY(coupon, drawRow)) {
    	        removeAllX(drawRow);
        	    free(drawRow);
            	continue;
        	}
        	else i++;
        } else if (lengthY(coupon) >= ceil(totalDrawCount/3) && lengthY(coupon) < 2*ceil(totalDrawCount/3)) {
			if (search3CombXY(coupon, drawRow)) {
    	        removeAllX(drawRow);
        	    free(drawRow);
            	continue;
        	}
        	else i++;
		} else {
			if (search4CombXY(coupon, drawRow)) {
    	        removeAllX(drawRow);
        	    free(drawRow);
            	continue;
        	}
        	else i++;
		}
		
        bubbleSortXByKey(drawRow);
        appendList(coupon, drawRow);
    }
}



void drawBalls(struct ListXY *coupon, int drawCount, int drawByDate)
{
	int i;
    int found = 0;
	int matchComb = 0;
	int elimComb = 0;
	int autoCalc = 1;
    int noMatch, noElim;
    char label[30];

	struct ListX *drawnBallsNorm = NULL;
	struct ListX *drawnBallsLeft = NULL;
	struct ListX *drawnBallsBlend1 = NULL;
	struct ListX *drawnBallsBlend2 = NULL;
	struct ListX *drawnBallsSide = NULL;
	struct ListX *drawnBallsRand = NULL;

	struct ListXY *drawnBallsDate = NULL;
	struct ListX *drawnBallsDate1 = NULL;
	struct ListX *drawnBallsDate2 = NULL;

	struct ListX *winningBallStats = NULL;
	struct ListX *superStarBallStats = NULL;

	winningBallStats = createListX(winningBallStats, NULL, NULL, 0);
	superStarBallStats = createListX(superStarBallStats, NULL, NULL, 0);

	getDrawnBallsStats(winningBallStats, superStarBallStats);

	strcpy(label, "(normal distribution)");
    drawnBallsNorm = createListX(drawnBallsNorm, NULL, label, 0);
	strcpy(label, "(left stacked)");
	drawnBallsLeft = createListX(drawnBallsLeft, NULL, label, 0);
	strcpy(label, "(blend 1)");
	drawnBallsBlend1 = createListX(drawnBallsBlend1, NULL, label, 0);
	strcpy(label, "(blend 2)");
	drawnBallsBlend2 = createListX(drawnBallsBlend2, NULL, label, 0);
	strcpy(label, "(side stacked)");
	drawnBallsSide = createListX(drawnBallsSide, NULL, label, 0);
	strcpy(label, "(random)");
	drawnBallsRand = createListX(drawnBallsRand, NULL, label, 0);

	if (drawByDate) {
		drawnBallsDate = drawBallByDate(winningBallStats);
		drawnBallsDate1 = getListXByIndex(drawnBallsDate, 0);
		drawnBallsDate2 = getListXByIndex(drawnBallsDate, 1);

    	if (drawCount) {
    		appendList(coupon, drawnBallsDate1);
	        drawCount--;
    	}

    	if (drawCount) {
    		appendList(coupon, drawnBallsDate2);
	        drawCount--;
    	}
	}

	/* normal distribution */
	if (autoCalc) {
		matchComb = 0;
        elimComb = 2;
	}
	
	for (i=0; i<40 && drawCount; i++) 
    {
		drawnBallsNorm = drawBallByNorm(drawnBallsNorm, winningBallStats, TOTAL_BALL, DRAW_BALL, matchComb, elimComb);
        if (matchComb) noMatch = !findComb(drawnBallsNorm, matchComb);
		else noMatch = 0;

        if (elimComb) noElim = findComb(drawnBallsNorm, elimComb);
		else noElim = 0;

        if (!(noMatch || noElim)) {
            if (i < 10) {
        		found = search1BallXY(coupon, drawnBallsNorm, DRAW_BALL);
            } else if (i < 20) {
    			found = search2CombXY(coupon, drawnBallsNorm);
            } else if (i < 30) {
    			found = search3CombXY(coupon, drawnBallsNorm);
            } else {
    			found = search4CombXY(coupon, drawnBallsNorm);
            }
	    	if(!found) break;
        }

        if (autoCalc) {
			if (i > 9 && i <= 19) {matchComb = 0; elimComb = 3;}
			else if (i > 19 && i <= 29) {matchComb = 3; elimComb = 4;}
			else {matchComb = 0; elimComb = 0;}
		}
	}

    if (drawCount) {
        appendList(coupon, drawnBallsNorm);
        drawCount--;
    }


	/* Blend 1 */
	if (autoCalc) {
		matchComb = 0;
        elimComb = 2;
	}

	for (i=0; i<40 && drawCount; i++) 
    {
		drawnBallsBlend1 = drawBallByBlend1(drawnBallsBlend1, winningBallStats, TOTAL_BALL, DRAW_BALL, matchComb, elimComb);

        if (matchComb) noMatch = !findComb(drawnBallsBlend1, matchComb);
		else noMatch = 0;

        if (elimComb) noElim = findComb(drawnBallsBlend1, elimComb);
		else noElim = 0;

        if (!(noMatch || noElim)) {
            if (i < 10) {
        		found = search1BallXY(coupon, drawnBallsBlend1, DRAW_BALL);
            } else if (i < 20) {
    			found = search2CombXY(coupon, drawnBallsBlend1);
            } else if (i < 30) {
    			found = search3CombXY(coupon, drawnBallsBlend1);
            } else {
    			found = search4CombXY(coupon, drawnBallsBlend1);
            }
	    	if(!found) break;
        }

        if (autoCalc) {
			if (i > 9 && i <= 19) {matchComb = 0; elimComb = 3;}
			else if (i > 19 && i <= 29) {matchComb = 3; elimComb = 4;}
			else {matchComb = 0; elimComb = 0;}
		}
	}

    if (drawCount) {
       	appendList(coupon, drawnBallsBlend1);
        drawCount--;
    }


	/* Blend 2 */
	if (autoCalc) {
		matchComb = 0;
        elimComb = 2;
	}

	for (i=0; i<40 && drawCount; i++) 
    {
		drawnBallsBlend2 = drawBallByBlend2(drawnBallsBlend2, winningBallStats, TOTAL_BALL, DRAW_BALL, matchComb, elimComb);

        if (matchComb) noMatch = !findComb(drawnBallsBlend2, matchComb);
		else noMatch = 0;

        if (elimComb) noElim = findComb(drawnBallsBlend2, elimComb);
		else noElim = 0;

        if (!(noMatch || noElim)) {
            if (i < 10) {
        		found = search1BallXY(coupon, drawnBallsBlend2, DRAW_BALL);
            } else if (i < 20) {
    			found = search2CombXY(coupon, drawnBallsBlend2);
            } else if (i < 30) {
    			found = search3CombXY(coupon, drawnBallsBlend2);
            } else {
    			found = search4CombXY(coupon, drawnBallsBlend2);
            }
	    	if(!found) break;
        }

        if (autoCalc) {
			if (i > 9 && i <= 19) {matchComb = 0; elimComb = 3;}
			else if (i > 19 && i <= 29) {matchComb = 3; elimComb = 4;}
			else {matchComb = 0; elimComb = 0;}
		}
	}

    if (drawCount) {
       	appendList(coupon, drawnBallsBlend2);
        drawCount--;
    }


	/* Left Stacked */
	if (autoCalc) {
		matchComb = 0;
        elimComb = 2;
	}

	for (i=0; i<40 && drawCount; i++) 
    {
		drawnBallsLeft = drawBallByLeft(drawnBallsLeft, winningBallStats, TOTAL_BALL, DRAW_BALL, matchComb, elimComb);

        if (matchComb) noMatch = !findComb(drawnBallsLeft, matchComb);
		else noMatch = 0;

        if (elimComb) noElim = findComb(drawnBallsLeft, elimComb);
		else noElim = 0;

        if (!(noMatch || noElim)) {
            if (i < 10) {
        		found = search1BallXY(coupon, drawnBallsLeft, DRAW_BALL);
            } else if (i < 20) {
    			found = search2CombXY(coupon, drawnBallsLeft);
            } else if (i < 30) {
    			found = search3CombXY(coupon, drawnBallsLeft);
            } else {
    			found = search4CombXY(coupon, drawnBallsLeft);
            }
	    	if(!found) break;
        }

        if (autoCalc) {
			if (i > 9 && i <= 19) {matchComb = 0; elimComb = 3;}
			else if (i > 19 && i <= 29) {matchComb = 3; elimComb = 4;}
			else {matchComb = 0; elimComb = 0;}
		}
	}

    if (drawCount) {
       	appendList(coupon, drawnBallsLeft);
        drawCount--;
    }


	/* Side Stacked */
	if (autoCalc) {
		matchComb = 0;
        elimComb = 2;
	}

	for (i=0; i<40 && drawCount; i++) 
    {
		drawnBallsSide = drawBallBySide(drawnBallsSide, winningBallStats, TOTAL_BALL, DRAW_BALL, matchComb, elimComb);

        if (matchComb) noMatch = !findComb(drawnBallsSide, matchComb);
		else noMatch = 0;

        if (elimComb) noElim = findComb(drawnBallsSide, elimComb);
		else noElim = 0;

        if (!(noMatch || noElim)) {
            if (i < 10) {
        		found = search1BallXY(coupon, drawnBallsSide, DRAW_BALL);
            } else if (i < 20) {
    			found = search2CombXY(coupon, drawnBallsSide);
            } else if (i < 30) {
    			found = search3CombXY(coupon, drawnBallsSide);
            } else {
    			found = search4CombXY(coupon, drawnBallsSide);
            }
	    	if(!found) break;
        }

        if (autoCalc) {
			if (i > 9 && i <= 19) {matchComb = 0; elimComb = 3;}
			else if (i > 19 && i <= 29) {matchComb = 3; elimComb = 4;}
			else {matchComb = 0; elimComb = 0;}
		}
	}

    if (drawCount) {
       	appendList(coupon, drawnBallsSide);
        drawCount--;
    }


	/* Random */
	if (autoCalc) {
		matchComb = 0;
        elimComb = 2;
	}

	for (i=0; i<40 && drawCount; i++) 
    {
		drawnBallsRand = drawBallByRand(drawnBallsRand, winningBallStats, TOTAL_BALL, DRAW_BALL, matchComb, elimComb);

        if (matchComb) noMatch = !findComb(drawnBallsRand, matchComb);
		else noMatch = 0;

        if (elimComb) noElim = findComb(drawnBallsRand, elimComb);
		else noElim = 0;

        if (!(noMatch || noElim)) {
            if (i < 10) {
        		found = search1BallXY(coupon, drawnBallsRand, DRAW_BALL);
            } else if (i < 20) {
    			found = search2CombXY(coupon, drawnBallsRand);
            } else if (i < 30) {
    			found = search3CombXY(coupon, drawnBallsRand);
            } else {
    			found = search4CombXY(coupon, drawnBallsRand);
            }
	    	if(!found) break;
        }

        if (autoCalc) {
			if (i > 9 && i <= 19) {matchComb = 0; elimComb = 3;}
			else if (i > 19 && i <= 29) {matchComb = 3; elimComb = 4;}
			else {matchComb = 0; elimComb = 0;}
		}
	}

    if (drawCount) {
       	appendList(coupon, drawnBallsRand);
        drawCount--;
    }
}



void drawBallsSS(struct ListXY *coupon, int drawCount)
{
	int i, j;
    int found = 0;
    char label[30];

	struct ListX *drawnBalls = NULL;

	struct ListX *winningBallStats = NULL;
	struct ListX *superStarBallStats = NULL;

	winningBallStats = createListX(winningBallStats, NULL, NULL, 0);
	superStarBallStats = createListX(superStarBallStats, NULL, NULL, 0);

	getDrawnBallsStats(winningBallStats, superStarBallStats);

	strcpy(label, "super star");

	for (i=0; i<drawCount; i++) 
    {
		drawnBalls = createListX(drawnBalls, NULL, label, 0);
		for (j=0; j<6; j++) 
    	{
			drawnBalls = drawBallByNorm(drawnBalls, superStarBallStats, TOTAL_BALL_SS, 1, 0, 0);
   			found = search1BallXY(coupon, drawnBalls, 1);
    		if(!found) break;
		}

		if (found) {
			for (j=0; j<6; j++) 
    		{
				drawnBalls = drawBallByRand(drawnBalls, superStarBallStats, TOTAL_BALL_SS, 1, 0, 0);
   				found = search1BallXY(coupon, drawnBalls, 1);
    			if(!found) break;
			}
		}

		if (found) {
			for (j=0; j<6; j++) 
    		{
				drawnBalls = drawBallByBlend1(drawnBalls, superStarBallStats, TOTAL_BALL_SS, 1, 0, 0);
   				found = search1BallXY(coupon, drawnBalls, 1);
    			if(!found) break;
			}
		}

		if (found) {
			for (j=0; j<6; j++) 
    		{
				drawnBalls = drawBallByBlend2(drawnBalls, superStarBallStats, TOTAL_BALL_SS, 1, 0, 0);
   				found = search1BallXY(coupon, drawnBalls, 1);
    			if(!found) break;
			}
		}

		if (found) {
			for (j=0; j<6; j++) 
    		{
			    drawnBalls = drawBallByNorm(drawnBalls, superStarBallStats, TOTAL_BALL_SS, 1, 0, 0);
   			    found = search1BallXY(coupon, drawnBalls, 1);
    		    if(!found) break;
			}
		}

       	appendList(coupon, drawnBalls);
	}
}



void getDrawnBallsList(struct ListXY * winningDrawnBallsList, struct ListXY * superStarDrawnBallsList, char *fileName)
{
	int i;
	int d1, m1, y1, joker, sstar;
	int n1, n2, n3, n4, n5, n6;

	int eof = 0;
	FILE *fp;

	struct ListX *drawList = NULL;
	char s1[3], s2[3], s3[5];
	char tmp[2];
	char date[11];

	if ((fp = fopen(fileName, "r")) == NULL) {
		printf("%s file not found!\n", fileName);
		return;
	}

	for (i=0; eof != EOF; i++)
	{
		#ifdef SAYISALLOTTO
			eof = fscanf(fp, "%d.%d.%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", &d1, &m1, &y1, &n1, &n2, &n3, &n4, &n5, &n6, &joker, &sstar);
		#else
			eof = fscanf(fp, "%d.%d.%d\t%d\t%d\t%d\t%d\t%d\t%d\n", &d1, &m1, &y1, &n1, &n2, &n3, &n4, &n5, &n6);
		#endif

		if (eof == EOF) break;
			
		sprintf(s1, "%d", d1);
		sprintf(s2, "%d", m1);
		sprintf(s3, "%d", y1);

		date[0] = '\0';

		if (strlen(s1)==1) {
			tmp[0] = s1[0];
			s1[0] = '0';
			s1[1] = tmp[0];
			s1[2] = '\0';
		}
		if (strlen(s2)==1) {
			tmp[0] = s2[0];
			s2[0] = '0';
			s2[1] = tmp[0];
			s2[2] = '\0';
		}

		sprintf(date, "%s.%s.%s", s1, s2, s3);

		drawList = createListX(drawList, date, NULL, 0);

		appendItem(drawList, n1);
		appendItem(drawList, n2);
		appendItem(drawList, n3);
		appendItem(drawList, n4);
		appendItem(drawList, n5);
		appendItem(drawList, n6);

		appendList(winningDrawnBallsList, drawList);

		#ifdef SAYISALLOTTO
			drawList = createListX(drawList, date, NULL, 0);
			appendItem(drawList, sstar);
			appendList(superStarDrawnBallsList, drawList);
		#endif
	}

	fclose(fp);
}



int dateDiff(int d1, int m1, int y1, int d2, int m2, int y2)
{
	int x1, x2;

	m1 = (m1 + 9) % 12;
	y1 = y1 - m1 / 10;
	x1 = 365*y1 + y1/4 - y1/100 + y1/400 + (m1*306 + 5)/10 + (d1-1);

	m2 = (m2 + 9) % 12;
	y2 = y2 - m2 / 10;
	x2 = 365*y2 + y2/4 - y2/100 + y2/400 + (m2*306 + 5)/10 + (d2-1);

	return (x2-x1);
}



struct ListXY * drawBallByDate(struct ListX *ballStats)
{
	int x, j, k;
    int d1=12, m1=7, y1=2021;
	int d2, m2, y2;
	time_t rawtime;
	struct tm *timeInfo;
	unsigned long num;
	char snum[9], snum2[9], snum3[9], snum4[13];
	char s1[3], s2[3], s3[3], s4[3];
	char s5[3], s6[3], s7[3], s8[3];
	int n1, n2, n3, n4, n5, n6;
	int e1, e2, e3, e4, e5, e6;
	char se1[3], se2[3], se3[3], se4[3], se5[3], se6[3];
	char tmp[1], tmp2[3];
    char label[30];

	unsigned int drawball;

	struct ListX *globe = NULL;
	struct ListX *drawnBallsDate1 = NULL;
	struct ListX *drawnBallsDate2 = NULL;
	struct ListXY *drawnBallsList = NULL;

	globe = createListX(globe, NULL, NULL, 0);
	strcpy(label, "(date 1)");
	drawnBallsDate1 = createListX(drawnBallsDate1, NULL, label, 0);
	strcpy(label, "(date 2)");
	drawnBallsDate2 = createListX(drawnBallsDate2, NULL, label, 0);
	drawnBallsList = createListXY(drawnBallsList);

	bubbleSortXByVal(ballStats, 1);

	/* Fill inside the globe with balls */
	if (rand() % 2) // (blend1)
	{ 
		for (x=0, j=TOTAL_BALL-1, k=0; k<TOTAL_BALL; k++)
		{
			if (k%2) {
				appendItem(globe, getKey(ballStats, j));
				j--;
			}
			else {
				insertItem(globe, getKey(ballStats, x));
				x++;
			}
		}
	}
	else // (blend2)
	{ 
		for (x=0, j=TOTAL_BALL-1, k=0; k<TOTAL_BALL; k++)
		{
			if (k%2) {
				addItemByIndex(globe, TOTAL_BALL-j, getKey(ballStats, j));
				j--;
			}
			else {
				addItemByIndex(globe, x, getKey(ballStats, x));
				x++;
			}
		}
	}


	time(&rawtime);
	timeInfo = localtime(&rawtime);

	d2 = timeInfo->tm_mday;
	m2 = timeInfo->tm_mon +1;
	y2 = timeInfo->tm_year +1900;

	num = 4532632 + (unsigned long) ceil(dateDiff(d1, m1, y1, d2, m2, y2)*106.5);
	sprintf(snum, "%ld", num);

	if (strlen(snum) < 8) {
		snum2[0] = '0';
		snum2[1] = snum[0];
		snum2[2] = snum[1];
		snum2[3] = snum[2];
		snum2[4] = snum[3];
		snum2[5] = snum[4];
		snum2[6] = snum[5];
		snum2[7] = snum[6];
		snum2[8] = '\0';
	} else {
		snum2[0] = snum[0];
		snum2[1] = snum[1];
		snum2[2] = snum[2];
		snum2[3] = snum[3];
		snum2[4] = snum[4];
		snum2[5] = snum[5];
		snum2[6] = snum[6];
		snum2[7] = snum[7];
		snum2[8] = '\0';
	}
	snum3[0] = snum2[7];
	snum3[1] = snum2[0];
	snum3[2] = snum2[1];
	snum3[3] = snum2[2];
	snum3[4] = snum2[3];
	snum3[5] = snum2[4];
	snum3[6] = snum2[5];
	snum3[7] = snum2[6];
	snum3[8] = '\0';

	s1[0] = snum2[0];
	s1[1] = snum2[1];
	s1[2] = '\0';
	s2[0] = snum2[2];
	s2[1] = snum2[3];
	s2[2] = '\0';
	s3[0] = snum2[4];
	s3[1] = snum2[5];
	s3[2] = '\0';
	s4[0] = snum2[6];
	s4[1] = snum2[7];
	s4[2] = '\0';

	s5[0] = snum3[0];
	s5[1] = snum3[1];
	s5[2] = '\0';
	s6[0] = snum3[2];
	s6[1] = snum3[3];
	s6[2] = '\0';
	s7[0] = snum3[4];
	s7[1] = snum3[5];
	s7[2] = '\0';
	s8[0] = snum3[6];
	s8[1] = snum3[7];
	s8[2] = '\0';

	n1 = (atoi(s2)+atoi(s5)) % TOTAL_BALL;
	if (n1 == 0) n1 = TOTAL_BALL;
	n2 = (atoi(s1)+atoi(s6)) % TOTAL_BALL;
	if (n2 == 0) n2 = TOTAL_BALL;
	n3 = (atoi(s3)+atoi(s6)) % TOTAL_BALL;
	if (n3 == 0) n3 = TOTAL_BALL;
	n4 = (atoi(s2)+atoi(s7)) % TOTAL_BALL;
	if (n4 == 0) n4 = TOTAL_BALL;
	n5 = (atoi(s4)+atoi(s7)) % TOTAL_BALL;
	if (n5 == 0) n5 = TOTAL_BALL;
	n6 = (atoi(s3)+atoi(s8)) % TOTAL_BALL;
	if (n6 == 0) n6 = TOTAL_BALL;

	sprintf(se1, "%d", n1);
	sprintf(se2, "%d", n2);
	sprintf(se3, "%d", n3);
	sprintf(se4, "%d", n4);
	sprintf(se5, "%d", n5);
	sprintf(se6, "%d", n6);

	snum4[0] = '\0';

	if (strlen(se1)==1) {
		tmp[0] = se1[0];
		se1[0] = '0';
		se1[1] = tmp[0];
		se1[2] = '\0';
	}
	if (strlen(se2)==1) {
		tmp[0] = se2[0];
		se2[0] = '0';
		se2[1] = tmp[0];
		se2[2] = '\0';
	}
	if (strlen(se3)==1) {
		tmp[0] = se3[0];
		se3[0] = '0';
		se3[1] = tmp[0];
		se3[2] = '\0';
	}
	if (strlen(se4)==1) {
		tmp[0] = se4[0];
		se4[0] = '0';
		se4[1] = tmp[0];
		se4[2] = '\0';
	}
	if (strlen(se5)==1) {
		tmp[0] = se5[0];
		se5[0] = '0';
		se5[1] = tmp[0];
		se5[2] = '\0';
	}
	if (strlen(se6)==1) {
		tmp[0] = se6[0];
		se6[0] = '0';
		se6[1] = tmp[0];
		se6[2] = '\0';
	}

	strcat(snum4, se1);
	strcat(snum4, se2);
	strcat(snum4, se3);
	strcat(snum4, se4);
	strcat(snum4, se5);
	strcat(snum4, se6);

	tmp2[2] = '\0';

	tmp2[0] = snum4[11];
	tmp2[1] = snum4[0];
	e1 = atoi(tmp2) % TOTAL_BALL;
	if (e1 == 0) e1 = TOTAL_BALL;

	tmp2[0] = snum4[1];
	tmp2[1] = snum4[2];
	e2 = atoi(tmp2) % TOTAL_BALL;
	if (e2 == 0) e2 = TOTAL_BALL;

	tmp2[0] = snum4[3];
	tmp2[1] = snum4[4];
	e3 = atoi(tmp2) % TOTAL_BALL;
	if (e3 == 0) e3 = TOTAL_BALL;

	tmp2[0] = snum4[5];
	tmp2[1] = snum4[6];
	e4 = atoi(tmp2) % TOTAL_BALL;
	if (e4 == 0) e4 = TOTAL_BALL;

	tmp2[0] = snum4[7];
	tmp2[1] = snum4[8];
	e5 = atoi(tmp2) % TOTAL_BALL;
	if (e5 == 0) e5 = TOTAL_BALL;

	tmp2[0] = snum4[9];
	tmp2[1] = snum4[10];
	e6 = atoi(tmp2) % TOTAL_BALL;
	if (e6 == 0) e6 = TOTAL_BALL;

	appendItem(drawnBallsDate1, n1);

    if (seqSearch(drawnBallsDate1, n2) >= 0) {
        do {
	        drawball = getKey(globe, gaussIndex(TOTAL_BALL)-1);
        } while(seqSearch(drawnBallsDate1, drawball) >= 0);
		n2 = drawball;
    }
	appendItem(drawnBallsDate1, n2);

    if (seqSearch(drawnBallsDate1, n3) >= 0) {
        do {
	        drawball = getKey(globe, gaussIndex(TOTAL_BALL)-1);
        } while(seqSearch(drawnBallsDate1, drawball) >= 0);
		n3 = drawball;
    }
	appendItem(drawnBallsDate1, n3);

    if (seqSearch(drawnBallsDate1, n4) >= 0) {
        do {
	        drawball = getKey(globe, gaussIndex(TOTAL_BALL)-1);
        } while(seqSearch(drawnBallsDate1, drawball) >= 0);
		n4 = drawball;
    }
	appendItem(drawnBallsDate1, n4);

    if (seqSearch(drawnBallsDate1, n5) >= 0) {
        do {
	        drawball = getKey(globe, gaussIndex(TOTAL_BALL)-1);
        } while(seqSearch(drawnBallsDate1, drawball) >= 0);
		n5 = drawball;
    }
	appendItem(drawnBallsDate1, n5);

    if (seqSearch(drawnBallsDate1, n6) >= 0) {
        do {
	        drawball = getKey(globe, gaussIndex(TOTAL_BALL)-1);
        } while(seqSearch(drawnBallsDate1, drawball) >= 0);
		n6 = drawball;
    }
	appendItem(drawnBallsDate1, n6);

	bubbleSortXByKey(drawnBallsDate1);

	appendItem(drawnBallsDate2, e1);

    if (seqSearch(drawnBallsDate2, e2) >= 0) {
        do {
	        drawball = getKey(globe, gaussIndex(TOTAL_BALL)-1);
        } while(seqSearch(drawnBallsDate2, drawball) >= 0);
		e2 = drawball;
    }
	appendItem(drawnBallsDate2, e2);

    if (seqSearch(drawnBallsDate2, e3) >= 0) {
        do {
	        drawball = getKey(globe, gaussIndex(TOTAL_BALL)-1);
        } while(seqSearch(drawnBallsDate2, drawball) >= 0);
		e3 = drawball;
    }
	appendItem(drawnBallsDate2, e3);

    if (seqSearch(drawnBallsDate2, e4) >= 0) {
        do {
	        drawball = getKey(globe, gaussIndex(TOTAL_BALL)-1);
        } while(seqSearch(drawnBallsDate2, drawball) >= 0);
		e4 = drawball;
    }
	appendItem(drawnBallsDate2, e4);

    if (seqSearch(drawnBallsDate2, e5) >= 0) {
        do {
	        drawball = getKey(globe, gaussIndex(TOTAL_BALL)-1);
        } while(seqSearch(drawnBallsDate2, drawball) >= 0);
		e5 = drawball;
    }
	appendItem(drawnBallsDate2, e5);

    if (seqSearch(drawnBallsDate2, e6) >= 0) {
        do {
	        drawball = getKey(globe, gaussIndex(TOTAL_BALL)-1);
        } while(seqSearch(drawnBallsDate2, drawball) >= 0);
		e6 = drawball;
    }
	appendItem(drawnBallsDate2, e6);

	bubbleSortXByKey(drawnBallsDate2);

	appendList(drawnBallsList, drawnBallsDate1);
	appendList(drawnBallsList, drawnBallsDate2);

    removeAllX(globe);
    free(globe);

	return drawnBallsList;
}



struct ListX * drawBallByRand(struct ListX *drawnBallsRand, struct ListX *ballStats, int totalBall, int drawBall, int matchComb, int elimComb)
{
	int i, j, k, x, y, z;
	int index;
	unsigned int flyball, drawball;
	unsigned int shuffleGlobe;
    int noMatch, noElim;

	struct ListX *globe = NULL;

	globe = createListX(globe, NULL, NULL, 0);

	bubbleSortXByVal(ballStats, 1);

	/* Fill inside the globe with balls */
	if (rand() % 2) // (blend1)
	{ 
		for (x=0, j=totalBall-1, k=0; k<totalBall; k++)
		{
			if (k%2) {
				appendItem(globe, getKey(ballStats, j));
				j--;
			}
			else {
				insertItem(globe, getKey(ballStats, x));
				x++;
			}
		}
	}
	else // (blend2)
	{ 
		for (x=0, j=totalBall-1, k=0; k<totalBall; k++)
		{
			if (k%2) {
				addItemByIndex(globe, totalBall-j, getKey(ballStats, j));
				j--;
			}
			else {
				addItemByIndex(globe, x, getKey(ballStats, x));
				x++;
			}
		}
	}

	for (i=0; i < drawBall*3; i++)
	{
		removeAllX(drawnBallsRand);

		for (j=0; j<drawBall; j++)
		{
			/* shuffle globe */
			shuffleGlobe = ceil((totalBall * totalBall * totalBall * (rand()%3+1)) / ceil(totalBall/3));

			for (k=0; k<shuffleGlobe; k++)
			{
				index = rand() % (totalBall-j);
				if (index == ceil(totalBall/2) -1 || index == ceil(totalBall/2))
					continue;
				else if (index < ceil(totalBall/2)-1) {
					flyball = removeItemByIndex(globe, index);
					insertItem(globe, flyball);
				}
				else {
					flyball = removeItemByIndex(globe, index);
					appendItem(globe, flyball);
				}
			}

			/* draw a ball */
			drawball = removeItemByIndex(globe, ceil((totalBall-j)/2));
			appendItem(drawnBallsRand, drawball);
		}

        for (x=0, y=drawBall-1, z=0; z<drawBall; z++) 
        {
    	    if (z%2) {
	    	    appendItem(globe, getKey(drawnBallsRand, y));
		        y--;
		    }
   		    else {
    		    insertItem(globe, getKey(drawnBallsRand, x));
	    	    x++;
	        }
        }

		noMatch = 0;
        if (matchComb) {
            if (!findComb(drawnBallsRand, matchComb)) noMatch = 1;
        }

		noElim = 0;
		if (elimComb) {
            if (findComb(drawnBallsRand, elimComb)) noElim = 1;
        }

        if (noMatch || noElim) {
        } else break;
	}

	bubbleSortXByKey(drawnBallsRand);

    removeAllX(globe);
    free(globe);

	return drawnBallsRand;
}



struct ListX * drawBallByLeft(struct ListX *drawnBallsLeft, struct ListX *ballStats, int totalBall, int drawBall, int matchComb, int elimComb)
{
	int i, k;
	int noMatch, noElim;
    unsigned int drawball;
	struct ListX *globe = NULL;

	globe = createListX(globe, NULL, NULL, 0);

	bubbleSortXByVal(ballStats, 1);

   	for (i=0; i<totalBall; i++)
    {
	    appendItem(globe, getKey(ballStats, i));
    }

	for (k=0; k<totalBall; k++)
	{
		removeAllX(drawnBallsLeft);

		for (i=0; i<drawBall; i++)
		{
			drawball = getKey(globe, gaussIndex(totalBall)-1);
        	if (seqSearch(drawnBallsLeft, drawball) >= 0) {
		        do {
			        drawball = getKey(globe, gaussIndex(totalBall)-1);
		        } while(seqSearch(drawnBallsLeft, drawball) >= 0);
	        }
			appendItem(drawnBallsLeft, drawball);
		}

		noMatch = 0;
        if (matchComb) {
            if (!findComb(drawnBallsLeft, matchComb)) noMatch = 1;
        }

		noElim = 0;
		if (elimComb) {
            if (findComb(drawnBallsLeft, elimComb)) noElim = 1;
        }

        if (noMatch || noElim) {
        } else break;
	}

	bubbleSortXByKey(drawnBallsLeft);

    removeAllX(globe);
    free(globe);

	return drawnBallsLeft;
}



struct ListX * drawBallByBlend1(struct ListX *drawnBallsBlend1, struct ListX *ballStats, int totalBall, int drawBall, int matchComb, int elimComb)
{
	int i, j, k;
    int noMatch, noElim;
	unsigned int drawball;
	struct ListX *globe = NULL;

	globe = createListX(globe, NULL, NULL, 0);

	bubbleSortXByVal(ballStats, 1);

   	for (i=0, j=totalBall-1, k=0; k<totalBall; k++)
    {
	    if (k%2) {
		    appendItem(globe, getKey(ballStats, j));
		    j--;
	    }
	    else {
		    insertItem(globe, getKey(ballStats, i));
		    i++;
	    }
    }

	for (k=0; k<totalBall; k++)
	{
		removeAllX(drawnBallsBlend1);

		for (i=0; i<drawBall; i++)
		{
			drawball = getKey(globe, gaussIndex(totalBall)-1);
        	if (seqSearch(drawnBallsBlend1, drawball) >= 0) {
		        do {
			        drawball = getKey(globe, gaussIndex(totalBall)-1);
		        } while(seqSearch(drawnBallsBlend1, drawball) >= 0);
	        }
			appendItem(drawnBallsBlend1, drawball);
		}

		noMatch = 0;
        if (matchComb) {
            if (!findComb(drawnBallsBlend1, matchComb)) noMatch = 1;
        }

		noElim = 0;
		if (elimComb) {
            if (findComb(drawnBallsBlend1, elimComb)) noElim = 1;
        }

        if (noMatch || noElim) {
        } else break;
	}

	bubbleSortXByKey(drawnBallsBlend1);

    removeAllX(globe);
    free(globe);

	return drawnBallsBlend1;
}



struct ListX * drawBallByBlend2(struct ListX *drawnBallsBlend2, struct ListX *ballStats, int totalBall, int drawBall, int matchComb, int elimComb)
{
	int i, j, k;
    int noMatch, noElim;
	unsigned int drawball;
	struct ListX *globe = NULL;

	globe = createListX(globe, NULL, NULL, 0);

	bubbleSortXByVal(ballStats, 1);

   	for (i=0, j=totalBall-1, k=0; k<totalBall; k++)
    {
	    if (k%2) {
		    addItemByIndex(globe, totalBall-j, getKey(ballStats, j));
   			j--;
    	}
	    else {
		    addItemByIndex(globe, i, getKey(ballStats, i));
		    i++;
	    }
    }

	for (k=0; k<totalBall; k++)
	{
		removeAllX(drawnBallsBlend2);

		for (i=0; i<drawBall; i++)
		{
			drawball = getKey(globe, gaussIndex(totalBall)-1);
        	if (seqSearch(drawnBallsBlend2, drawball) >= 0) {
		        do {
			        drawball = getKey(globe, gaussIndex(totalBall)-1);
		        } while(seqSearch(drawnBallsBlend2, drawball) >= 0);
	        }
			appendItem(drawnBallsBlend2, drawball);
		}

		noMatch = 0;
        if (matchComb) {
            if (!findComb(drawnBallsBlend2, matchComb)) noMatch = 1;
        }

		noElim = 0;
		if (elimComb) {
            if (findComb(drawnBallsBlend2, elimComb)) noElim = 1;
        }

        if (noMatch || noElim) {
        } else break;
	}

	bubbleSortXByKey(drawnBallsBlend2);

    removeAllX(globe);
    free(globe);

	return drawnBallsBlend2;
}



struct ListX * drawBallBySide(struct ListX *drawnBallsSide, struct ListX *ballStats, int totalBall, int drawBall, int matchComb, int elimComb)
{
	int i, k;
    int noMatch, noElim;
	unsigned int drawball;
	struct ListX *globe = NULL;

	globe = createListX(globe, NULL, NULL, 0);

	bubbleSortXByVal(ballStats, 1);

	for (k=0; k<totalBall; k++)
	{
		if (k%2) {
			insertItem(globe, getKey(ballStats, k));
		}
		else {
			appendItem(globe, getKey(ballStats, k));
		}
	}

	for (k=0; k<totalBall; k++)
	{
		removeAllX(drawnBallsSide);

		for (i=0; i<drawBall; i++)
		{
			drawball = getKey(globe, gaussIndex(totalBall)-1);
        	if (seqSearch(drawnBallsSide, drawball) >= 0) {
		        do {
			        drawball = getKey(globe, gaussIndex(totalBall)-1);
		        } while(seqSearch(drawnBallsSide, drawball) >= 0);
	        }
			appendItem(drawnBallsSide, drawball);
		}

		noMatch = 0;
        if (matchComb) {
            if (!findComb(drawnBallsSide, matchComb)) noMatch = 1;
        }

		noElim = 0;
		if (elimComb) {
            if (findComb(drawnBallsSide, elimComb)) noElim = 1;
        }

        if (noMatch || noElim) {
        } else break;
	}

	bubbleSortXByKey(drawnBallsSide);

    removeAllX(globe);
    free(globe);

	return drawnBallsSide;
}



struct ListX * drawBallByNorm(struct ListX *drawnBallsNorm, struct ListX *ballStats, int totalBall, int drawBall, int matchComb, int elimComb)
{
	int i, k;
    int noMatch, noElim;
	unsigned int drawball;
	struct ListX *globe = NULL;

	globe = createListX(globe, NULL, NULL, 0);

	bubbleSortXByVal(ballStats, -1);

   	for (k=0; k<totalBall; k++)
    {
	    if (k%2) {
		    insertItem(globe, getKey(ballStats, k));
	    }
	    else {
		    appendItem(globe, getKey(ballStats, k));
	    }
    }

	for (k=0; k<totalBall; k++)
	{
		removeAllX(drawnBallsNorm);

		for (i=0; i<drawBall; i++)
		{
            drawball = getKey(globe, gaussIndex(totalBall)-1);
        	if (seqSearch(drawnBallsNorm, drawball) >= 0) {
		        do {
			        drawball = getKey(globe, gaussIndex(totalBall)-1);
		        } while(seqSearch(drawnBallsNorm, drawball) >= 0);
	        }
			appendItem(drawnBallsNorm, drawball);
		}

		noMatch = 0;
        if (matchComb) {
            if (!findComb(drawnBallsNorm, matchComb)) noMatch = 1;
        }

		noElim = 0;
		if (elimComb) {
            if (findComb(drawnBallsNorm, elimComb)) noElim = 1;
        }

        if (noMatch || noElim) {
        } else break;
	}

	bubbleSortXByKey(drawnBallsNorm);

    removeAllX(globe);
    free(globe);

	return drawnBallsNorm;
}



void getDrawnBallsStats(struct ListX *winningBallStats, struct ListX *superStarBallStats)
{
	int i;
	int n1, n2, n3, n4, n5, n6;
	int superStar;
	int index;

	struct ListX *aPrvDrawn = NULL;

	for (i=0; i<TOTAL_BALL; i++)
	{
		insertItem(winningBallStats, i+1);
	}

	for (i=0; i<TOTAL_BALL; i++)
	{
		chgVal(winningBallStats, i, 0);
	}

	for (i=0; i<lengthY(winningDrawnBallsList); i++) {
		aPrvDrawn = getListXByIndex(winningDrawnBallsList, i);
		n1 = getKey(aPrvDrawn, 0);
		n2 = getKey(aPrvDrawn, 1);
		n3 = getKey(aPrvDrawn, 2);
		n4 = getKey(aPrvDrawn, 3);
		n5 = getKey(aPrvDrawn, 4);
		n6 = getKey(aPrvDrawn, 5);
		if ((index = seqSearch(winningBallStats,n1)) != -1) incVal(winningBallStats, index);
		if ((index = seqSearch(winningBallStats,n2)) != -1) incVal(winningBallStats, index);
		if ((index = seqSearch(winningBallStats,n3)) != -1) incVal(winningBallStats, index);
		if ((index = seqSearch(winningBallStats,n4)) != -1) incVal(winningBallStats, index);
		if ((index = seqSearch(winningBallStats,n5)) != -1) incVal(winningBallStats, index);
		if ((index = seqSearch(winningBallStats,n6)) != -1) incVal(winningBallStats, index);
	}

	for (i=0; i<TOTAL_BALL_SS; i++)
	{
		insertItem(superStarBallStats, i+1);
	}

	for (i=0; i<TOTAL_BALL_SS; i++)
	{
		chgVal(superStarBallStats, i, 0);
	}

	for (i=0; i<lengthY(superStarDrawnBallsList); i++) {
		aPrvDrawn = getListXByIndex(superStarDrawnBallsList, i);
		superStar = getKey(aPrvDrawn, 0);
		if ((index = seqSearch(superStarBallStats,superStar)) != -1) incVal(superStarBallStats, index);
	}
}



void clearScreen()
{
    #if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
        system("clear");
    #else
        system("cls");
    #endif
}



int get_app_path (char *pname, size_t pathsize)
{
    long result;
    
#if defined(__linux__)
    pathsize --;
    result = readlink("/proc/self/exe", pname, pathsize);
    if (result > 0)
    {
        pname[result] = 0;
        if ((access(pname, 0) == 0))
            return 0;
    }
#elif WIN32
    result = GetModuleFileName(NULL, pname, pathsize);
    if (result > 0)
    {
        int len = strlen(pname);
        int idx;
        for (idx = 0; idx < len; idx++)
        {
            if (pname[idx] == '\\') pname[idx] = '/';
        }
        if ((access(pname, 0) == 0))
            return 0;
    }
#elif SOLARIS
    char *p = getexecname();
    if (p)
    {
        if (p[0] == '/')
        {
            strncpy(pname, p, pathsize);
            if ((access(pname, 0) == 0))
                return 0;
        }
        else
        {
            getcwd(pname, pathsize);
            result = strlen(pname);
            strncat(pname, "/", (pathsize - result));
            result ++;
            strncat(pname, p, (pathsize - result));
            
            if ((access(pname, 0) == 0))
                return 0;
        }
    }
#elif __APPLE__
    /*
     extern int _NSGetExecutablePath(char *buf, uint32_t *bufsize);
     
     _NSGetExecutablePath copies the path of the executable
     into the buffer and returns 0 if the path was successfully
     copied in the provided buffer. If the buffer is not large
     enough, -1 is returned and the expected buffer size is
     copied in *bufsize. Note that _NSGetExecutablePath will
     return "a path" to the executable not a "real path" to the
     executable. That is the path may be a symbolic link and
     not the real file. And with deep directories the total
     bufsize needed could be more than MAXPATHLEN.
     */
    int status = -1;
    uint32_t ul_tmp = pathsize;
    char *given_path = malloc(MAXPATHLEN * 2);
    if (!given_path) return status;
    
    pathsize = MAXPATHLEN * 2;
    result = _NSGetExecutablePath(given_path, &ul_tmp);
    if (result == 0)
    {
        if (realpath(given_path, pname) != NULL)
        {
            if ((access(pname, 0) == 0))
                status = 0;
        }
    }
    free (given_path);
    return status;
#endif
    
    return -1; /* Path Lookup Failed */
}



int main(void)
{
	int keyb = 0, keyb2;

	struct ListX *winningBallStats = NULL;
	struct ListX *superStarBallStats = NULL;
	struct ListXY *coupon = NULL;
	struct ListXY *coupon_ss = NULL;

	struct ListXY *luckyBalls2 = NULL;
	struct ListXY *luckyBalls3 = NULL;
	struct ListXY *luckyBalls4 = NULL;

	FILE *fp;

	srand((unsigned) time(NULL));

	init();

	winningBallStats = createListX(winningBallStats, NULL, NULL, 0);
	superStarBallStats = createListX(superStarBallStats, NULL, NULL, 0);

	coupon = createListXY(coupon);
	coupon_ss = createListXY(coupon_ss);

	luckyBalls2 = createListXY(luckyBalls2);
	luckyBalls3 = createListXY(luckyBalls3);
	luckyBalls4 = createListXY(luckyBalls4);

	getDrawnBallsStats(winningBallStats, superStarBallStats);
	bubbleSortXByVal(winningBallStats, -1);
	bubbleSortXByVal(superStarBallStats, -1);

mainMenu:
	#ifdef SAYISALLOTTO
		printf("Sayisal Lotto 1.0");
	#else
		printf("Super Lotto 1.0");
	#endif
	printf(" Copyright ibrahim Tipirdamaz (c) 2022\n\n");
	printf("Which number drawn how many times? : \n\n");
	printBallStats(winningBallStats);
	printf("\n\n");

	#ifdef SAYISALLOTTO
		printf("Number of draws of SuperStar balls : \n\n");
		printBallStats(superStarBallStats);
		printf("\n\n");
	#endif

   	printf("1- Draw Ball\n");

   	printf("2- Matched 2 combinations: %u\n", match2comb);
    printf("3- Matched 3 combinations: %u\n", match3comb);
    printf("4- Matched 4 combinations: %u\n", match4comb);
    printf("5- Matched 5 combinations: %u\n", match5comb);

   	printf("6- Numbers that love each other (2 numbers)\n");
   	printf("7- Numbers that love each other (3 numbers)\n");
   	printf("8- Numbers that love each other (4 numbers)\n");
   	printf("9- Exit\n");
	printf("\nPlease make your selection: ");

keybCommand:
	do {
		scanf("%d", &keyb);
	} while(!(keyb >= 0 && keyb < 10));

	clearScreen();
    
	if (keyb == 0) {
        goto mainMenu;
    } 
    else if (keyb == 9) {
        goto exitProgram;
	}
   
   	if ((fp = fopen(outputFile, "w")) == NULL) {
    	printf("Can't open file %s\n", OUTPUTFILE);
	    return 0;
    }

    if (keyb == 1) {
    	printf("\nInput draw count (between 1-50) : ");
    	do {
	    	scanf("%d", &keyb2);
	    } while(!(keyb2 > 0 && keyb2 < 51));
        printf("\n");

        if (keyb2 < 9) {
	        drawBalls(coupon, keyb2, 1);
        }
        else if (keyb2 < 12) {
	        drawBalls(coupon, 8, 1);
    	    drawBallsByLucky(coupon, keyb2-8, keyb2);
        }
        else if (keyb2 < 18) {
	        drawBalls(coupon, 8, 1);
    	    drawBallsByLucky(coupon, 3, keyb2);
	        drawBalls(coupon, keyb2-11, 0);
        }
        else if (keyb2 < 21) {
	        drawBalls(coupon, 8, 1);
    	    drawBallsByLucky(coupon, 3, keyb2);
	        drawBalls(coupon, 6, 0);
    	    drawBallsByLucky(coupon, keyb2-17, keyb2);
        }
        else if (keyb2 < 27) {
	        drawBalls(coupon, 8, 1);
    	    drawBallsByLucky(coupon, 3, keyb2);
	        drawBalls(coupon, 6, 0);
    	    drawBallsByLucky(coupon, 3, keyb2);
	        drawBalls(coupon, keyb2-20, 0);
        }
        else if (keyb2 < 30) {
	        drawBalls(coupon, 8, 1);
    	    drawBallsByLucky(coupon, 3, keyb2);
	        drawBalls(coupon, 6, 0);
    	    drawBallsByLucky(coupon, 3, keyb2);
	        drawBalls(coupon, 6, 0);
    	    drawBallsByLucky(coupon, keyb2-26, keyb2);
        }
        else if (keyb2 < 36) {
	        drawBalls(coupon, 8, 1);
    	    drawBallsByLucky(coupon, 3, keyb2);
	        drawBalls(coupon, 6, 0);
    	    drawBallsByLucky(coupon, 3, keyb2);
	        drawBalls(coupon, 6, 0);
    	    drawBallsByLucky(coupon, 3, keyb2);
	        drawBalls(coupon, keyb2-29, 0);
        }
        else if (keyb2 < 39) {
	        drawBalls(coupon, 8, 1);
    	    drawBallsByLucky(coupon, 3, keyb2);
	        drawBalls(coupon, 6, 0);
    	    drawBallsByLucky(coupon, 3, keyb2);
	        drawBalls(coupon, 6, 0);
    	    drawBallsByLucky(coupon, 3, keyb2);
	        drawBalls(coupon, 6, 0);
    	    drawBallsByLucky(coupon, keyb2-35, keyb2);
        }
        else if (keyb2 < 45) {
	        drawBalls(coupon, 8, 1);
    	    drawBallsByLucky(coupon, 3, keyb2);
	        drawBalls(coupon, 6, 0);
    	    drawBallsByLucky(coupon, 3, keyb2);
	        drawBalls(coupon, 6, 0);
    	    drawBallsByLucky(coupon, 3, keyb2);
	        drawBalls(coupon, 6, 0);
    	    drawBallsByLucky(coupon, 3, keyb2);
	        drawBalls(coupon, keyb2-38, 0);
        }
        else if (keyb2 < 48) {
	        drawBalls(coupon, 8, 1);
    	    drawBallsByLucky(coupon, 3, keyb2);
	        drawBalls(coupon, 6, 0);
    	    drawBallsByLucky(coupon, 3, keyb2);
	        drawBalls(coupon, 6, 0);
    	    drawBallsByLucky(coupon, 3, keyb2);
	        drawBalls(coupon, 6, 0);
    	    drawBallsByLucky(coupon, 3, keyb2);
	        drawBalls(coupon, 6, 0);
    	    drawBallsByLucky(coupon, keyb2-44, keyb2);
        }
        else if (keyb2 < 51) {
	        drawBalls(coupon, 8, 1);
    	    drawBallsByLucky(coupon, 3, keyb2);
	        drawBalls(coupon, 6, 0);
    	    drawBallsByLucky(coupon, 3, keyb2);
	        drawBalls(coupon, 6, 0);
    	    drawBallsByLucky(coupon, 3, keyb2);
	        drawBalls(coupon, 6, 0);
    	    drawBallsByLucky(coupon, 3, keyb2);
	        drawBalls(coupon, 6, 0);
    	    drawBallsByLucky(coupon, 3, keyb2);
	        drawBalls(coupon, keyb2-47, 0);
        }

		#ifdef SAYISALLOTTO
			drawBallsSS(coupon_ss, keyb2);
	   	    printListXYWithSSByKey(coupon, coupon_ss, fp);
	        removeAllXY(coupon_ss);
		#else
			  printListXYByKey(coupon, fp);
		#endif

        removeAllXY(coupon);

	} else if (keyb == 2) {
    	printf("Calculation results are writing to %s file...\n", OUTPUTFILE);
		  calcCombMatch(2, fp);
	} else if (keyb == 3) {
		printf("Calculation results are writing to %s file...\n", OUTPUTFILE);
		calcCombMatch(3, fp);
	} else if (keyb == 4) {
		calcCombMatch(4, fp);
	} else if (keyb == 5) {
    	calcCombMatch(5, fp);
	} else if (keyb == 6) {
        luckyBalls2 = getLuckyBalls(luckyBalls2, 2);
       	printf("Numbers that love each other (2 numbers):\n\n");
       	fprintf(fp, "Numbers that love each other (2 numbers):\n\n");
        bubbleSortYByVal(luckyBalls2, -1);
        printLuckyBalls(luckyBalls2, fp);
        removeAllXY(luckyBalls2);
	} else if (keyb == 7) {
        luckyBalls3 = getLuckyBalls(luckyBalls3, 3);
       	printf("Numbers that love each other (3 numbers):\n\n");
       	fprintf(fp, "Numbers that love each other (3 numbers):\n\n");
        bubbleSortYByVal(luckyBalls3, -1);
		  printLuckyBalls(luckyBalls3, fp);
        removeAllXY(luckyBalls3);
	} else if (keyb == 8) {
        luckyBalls4 = getLuckyBalls(luckyBalls4, 4);
       	printf("Numbers that love each other (4 numbers):\n\n");
       	fprintf(fp, "Numbers that love each other (4 numbers):\n\n");
		  bubbleSortYByVal(luckyBalls4, -1);
		  printLuckyBalls(luckyBalls4, fp);
        removeAllXY(luckyBalls4);
	} 
    
		printf("\nThe results are written to %s file.\n", OUTPUTFILE);

    fclose(fp);
    
	printf("\n\n");
   	printf("0- Main Menu\n");
    if (keyb == 1) printf("1- Draw Again\n");
   	printf("9- Exit\n");
	printf("\nPlease make your selection: ");
	 goto keybCommand;

exitProgram:

	return 0;
}
