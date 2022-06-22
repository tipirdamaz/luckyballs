![alt text](https://github.com/tipirdamaz/luckyballs/blob/main/screenshots/preview.jpg)

# LuckyBalls

Lotto Statistics and Ball Draw Algorithms In C


# WARNING! THIS PROGRAM DOES NOT GUARANTEE YOU WINNING THE LOTTO.

This program provides statistics such as how many times the numbers in the previous draws came out, how many times the numbers that came out together in the draws came out together and how many days apart.

It allows you to draw a lotto by using the information of the past draws.

This program offers you useful 1 and 2 dimensional data structures created with linked list. It offers the ability to search and sort within data structures.


# PACKAGE CONTENTS

      source            : C source files

         lotto.c        : Turkey Sayisal Lotto and Super Lotto
         powerball.c    : American PowerBall Lotto


      dist

         powerball-win32.exe  : Windows 32 bit and 64 bit executable files.
         powerball-win64.exe  : These files were compiled with the gcc compiler on the windows 10 operating system.
         sayisal-win32.exe
         sayisal-win64.exe
         super-win32.exe
         super-win64.exe

         powerball-mac        : macOS executable files.
         sayisal-mac          : These files were compiled with the gcc compiler on the macOS Mojave 10.14.6 operating system.
         super-mac

         powerball-linux      : Linux executable files.
         sayisal-linux        : These files were compiled with the gcc compiler on the centOS 7 Linux operating system.
         super-linux

         powerball.txt        : TXT files are statistics files containing the results of previous draws.
         sayisal.txt          : These files must be located in the same directory as the executable files.
         super.txt

	 
      DOS               : For DOS operating system. 

         POWERBLL.EXE   : MS DOS executable files.
         SAYISL16.EXE   : These files were compiled with the 16-bit Borland Turbo C++ compiler on the Windows XP operating system.
         SUPER16.EXE

         powerbll.txt
         sayisal.txt
         super.txt



# COMPILER

      gcc compiler


# PLATFORM

      Source files were compiled with gcc compiler and successfuly tested on

      Windows 10
      macOS Mojave 10.14.6
      centOS 7 Linux



# HOW TO COMPILE FILES?

   SAYISAL AND SUPER LOTTO
   
   The lotto.c file can be compiled for both sayisal and super lotto.

      To compile for the sayisal lotto:

         #define SAYISALLOTTO 1 in the lotto.c file

         and compile with gcc with the following command:

         gcc lotto.c -o outputfile -lm

      To compile for the super lotto:

         //#define SAYISALLOTTO 1 in the lotto.c file

         and compile with gcc with the following command:

         gcc lotto.c -o outputfile -lm

   POWERBALL
   
   To compile the powerball.c file

      gcc powerball.c -o outputfile -lm



# SCREENSHOTS

![alt text](https://github.com/tipirdamaz/luckyballs/blob/main/screenshots/01.png)

![alt text](https://github.com/tipirdamaz/luckyballs/blob/main/screenshots/02.png)

![alt text](https://github.com/tipirdamaz/luckyballs/blob/main/screenshots/03.png)

![alt text](https://github.com/tipirdamaz/luckyballs/blob/main/screenshots/04.png)

![alt text](https://github.com/tipirdamaz/luckyballs/blob/main/screenshots/05.png)

![alt text](https://github.com/tipirdamaz/luckyballs/blob/main/screenshots/06.png)

![alt text](https://github.com/tipirdamaz/luckyballs/blob/main/screenshots/07.png)

![alt text](https://github.com/tipirdamaz/luckyballs/blob/main/screenshots/08.png)

![alt text](https://github.com/tipirdamaz/luckyballs/blob/main/screenshots/09.png)

![alt text](https://github.com/tipirdamaz/luckyballs/blob/main/screenshots/10.png)

![alt text](https://github.com/tipirdamaz/luckyballs/blob/main/screenshots/11.png)

![alt text](https://github.com/tipirdamaz/luckyballs/blob/main/screenshots/12.png)

![alt text](https://github.com/tipirdamaz/luckyballs/blob/main/screenshots/13.png)

![alt text](https://github.com/tipirdamaz/luckyballs/blob/main/screenshots/14.png)


# DATA STRUCTURES

```
struct Item {			/* List item (ball) */
	struct Item *next;	// pointer to next item (ball) in list (draw)
	int key;		// ball number
	int val;		// other info such as how many times the ball has been drawn so far
};



struct ListX {			/* List (globe, drawn balls, statistics file row etc) */
	char *date;		// old drawn date (for statistics file rows)
	char *label;		// list label (for new drawns)
	int val;		// how many times the numbers that love each other drawn together (for lucky numbers)
	struct Item *head;	// first item in the list
	struct ListX *next;	// next list (next row, if two dimensions)
};



struct ListXY {			/* 2 dimensions List. Multi draw (coupon or drawn balls from file has been drawn so far) */
	struct ListX *list; 	// list
};
```



# FUNCTION DEFINITIONS


```
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
```



```
/** 
 * Create Empty 2 dimensions List
 * 
 * @param {struct ListXY *}    : refers to a 2 dimensions ball list
 * @return {struct ListXY *}   : refers to the ball list (memory allocated)
 */
struct ListXY *createListXY(struct ListXY *pl);
```



```
/** 
 * Add an item (ball) to the beginning of the list
 * 
 * @param {struct ListX *}     : refers to a ball list
 * @param {Integer} key        : the key to which the ball will be added
 */
void insertItem(struct ListX *pl, int key);
```



```
/** 
 * Add an item (ball) to the end of the list
 * 
 * @param {struct ListX *}     : refers to a ball list
 * @param {Integer} key        : the key to which the ball will be added
 */
void appendItem(struct ListX *pl, int key);
```



```
/** 
 * Add a list to the end of the 2 dimensions list
 * 
 * @param {struct ListXY *}    : refers to a 2 dimensions ball list (multiple draws)
 * @param {struct ListX *}     : refers to a ball list
 */
void appendList(struct ListXY *pl, struct ListX *plNext);
```



```
/** 
 * Removes the specified item (ball) by key (ball number) from the list
 * 
 * @param {struct ListX *}     : refers to a ball list
 * @param {Integer} key        : the key to which the ball will be removed
 */
void removeItemByKey(struct ListX *pl, int key);
```



```
/** 
 * Removes item and returns the key (ball number) at the specified index from the list
 * 
 * @param {struct ListX *}     : refers to a ball list
 * @param {Integer} ind        : the index to which the ball will be removed
 * @return {Integer}           : returns key (ball number)
 */
int removeItemByIndex(struct ListX *pl, int ind);
```



```
/** 
 * If the specified key is not in the list, it adds to the specified index and returns 1
 * 
 * @param {struct ListX *}     : refers to a ball list
 * @param {Integer} ind        : the index to which the ball will be added
 * @param {Integer} key        : key (ball number)
 * @return {Integer}           : If the key is in the list, it does not add it to the list and return 0
 */
int addItemByIndex(struct ListX *pl, int ind, int key);
```



```
/** 
 * Remove all items in the list
 * 
 * @param {struct ListX *}     : refers to a ball list
 */
void removeAllX(struct ListX *pl);
```



```
/** 
 * Remove all list in the 2 dimensions list
 * 
 * @param {struct ListXY *}    : refers to a 2 dimensions ball list (multiple draws)
 */
void removeAllXY(struct ListXY *pl);
```



```
/** 
 * Return the number of items in the list
 * 
 * @param {struct ListX *}     : refers to a ball list
 * @return {Integer}           : Return the number of items in the list
 */
int length(struct ListX *pl);
```



```
/** 
 * Return the number of list in the 2 dimensions list
 * 
 * @param {struct ListXY *}    : refers to a 2 dimensions ball list (multiple draws)
 * @return {Integer}           : Return the number of list in the 2 dimensions list
 */
int lengthY(struct ListXY *pl);
```



```
/** 
 * Find the pointer of the item (ball) at the end of the list
 * 
 * @param {struct ListX *}    : refers to a ball list
 * @return {struct Item *}    : returns pointer of the item at the end of the list
 */
struct Item *atEnd(struct ListX *pl);
```



```
/** 
 * Find the pointer of the list at the end of the 2 dimensions list
 * 
 * @param {struct ListXY *}    : refers to a 2 dimensions ball list (multiple draws)
 * @return {struct ListX *}    : returns pointer of the list at the end of the 2 dimensions list
 */
struct ListX *atEndY(struct ListXY *pl);
```



```
/** 
 * Searches for a key (ball number) from the list.
 * 
 * @param {struct ListX *}     : refers to a ball list
 * @param {Integer} key        : key (ball number) to be search
 * @return {Integer}           : It returns the index of the number if it finds it, or -1 if it doesn't.
 */
int seqSearch(struct ListX *pl, int key);
```



```
/**
 * Searches for a key (ball number) from the 2 dimensions list.
 * 
 * @param {struct ListXY *}    : refers to a 2 dimensions ball list
 * @param {Integer} key        : key (ball number) to be search
 * @return {Integer}           : It returns the index of the number if it finds it, or -1 if it doesn't.
 *                               This is index of the first list containing the key in the 2 dimensions list
 */
int seqSearchXY1(struct ListXY *pl, int key);
```



```
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
```



```
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
```



```
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
```



```
/** 
 * Assigns the specified value to the item at the specified index
 * 
 * @param {struct ListX *}     : refers to a ball list (ballStats)
 * @param {Integer} ind        : index of an item (ball) in the ball list
 * @param {Integer} val        : Item value
 */
void chgVal(struct ListX *pl, int ind, int val);
```



```
/** 
 * Increments the value (the number of times the ball has drawn so far) of the item at the specified index by 1
 * 
 * @param {struct ListX *}     : refers to a ball list (ballStats)
 * @param {Integer} ind        : index of an item (ball) in the ball list
 */
void incVal(struct ListX *pl, int ind);
```



```
/** 
 * Increments the value (how many times the numbers that love each other drawn together) of the list at the specified index by 1
 * 
 * @param {struct ListXY *}    : refers to a 2 dimensions ball list (luckyBalls)
 * @param {Integer} ind        : index of a list in the 2 dimensions ball list
 */
void incValXY(struct ListXY *pl, int ind);
```



```
/** 
 * Returns the value of the item at the specified index
 * 
 * @param {struct ListX *}     : refers to a ball list
 * @param {Integer} ind        : index of an item (ball) in the ball list
 * @return {Integer}           : Returns the value of the item
 */
int getVal(struct ListX *pl, int ind);
```



```
/** 
 * Returns the value of the list at the specified index
 * 
 * @param {struct ListXY *}    : refers to a 2 dimensions ball list (luckyBalls)
 * @param {Integer} ind        : index of a list in the 2 dimensions ball list
 * @return {Integer}           : Returns the value of the list at the specified index
 */
int getValXY(struct ListXY *pl, int ind);
```



```
/** 
 * Returns the key (ball number) of the item at the specified index
 * 
 * @param {struct ListX *}     : refers to a ball list
 * @param {Integer} ind        : index of an item (ball) in the ball list
 * @return {Integer}           : Returns the key (ball number)
 */
int getKey(struct ListX *pl, int ind);
```



```
/** 
 * Returns the list in the 2 dimensions list at the specified index
 * 
 * @param {struct ListXY *}    : refers to a 2 dimensions ball list (multiple draws)
 * @param {Integer} ind        : index of 1 dimension list (a draw) in the 2 dimensions list (multi draws)
 * @return {struct ListX *}    : refers to a ball list at the specified index
 */
struct ListX * getListXByIndex(struct ListXY *pl, int ind);
```



```
/** 
 * Bubble sort by val the items (balls) in the ball list
 * 
 * @param {struct ListX *}    : refers to a ball list
 * @param {Integer} inc       : If inc=1, sort from smallest to greager. If inc=-1, sort from greater to smallest
 */
void bubbleSortXByVal(struct ListX *pl, int inc);
```



```
/** 
 * Bubble sort by val the lists in the 2 dimensions list
 * 
 * @param {struct ListXY *}   : refers to a 2 dimensions ball list
 * @param {Integer} inc       : If inc=1, sort from smallest to greager. If inc=-1, sort from greater to smallest
 */
void bubbleSortYByVal(struct ListXY *pl, int inc);
```



```
/** 
 * Bubble sort by key (ball number) from smallest to greager the items (ball) in the ball list
 * 
 * @param {struct ListX *}    : refers to a ball list
 */
void bubbleSortXByKey(struct ListX *pl);
```



```
/** 
 * Function to swap data of two item (ball) a and b
 * 
 * @param {struct Item *} a   : refers to a ball
 * @param {struct Item *} b   : refers to a ball
 */
void swapX(struct Item *a, struct Item *b);
```



```
/** 
 * Function to swap data of two list a and b
 * 
 * @param {struct ListX *} a   : refers to a ball list
 * @param {struct ListX *} b   : refers to a ball list
 */
void swapY(struct ListX *a, struct ListX *b);
```



```
/** 
 * Print keys (ball numbers) of the items in the list
 * 
 * @param {struct ListX *}    : refers to a ball list
 * @param {Integer} printTo   : 0: print to screen and output file. 1: print to screen only. 2: print to output file only.
 * @param {FILE *} fp         : refers to output file. If fp != NULL print to output file
 */
void printListXByKey(struct ListX *pl, int printTo, FILE *fp);
```



```
/** 
 * Print keys of the items (row by row) in the 2 dimensions list
 * 
 * @param {struct ListXY *}    : refers to 2 dimensions ball list
 */
void printListXYByKey(struct ListXY *pl);
```



```
/** 
 * print ListXY With PowerBall ByKey
 * Print keys of the items (row by row) in the 2 dimensions lists
 * 
 * @param {struct ListXY *}    : refers to 2 dimensions ball list (winning numbers)
 * @param {struct ListXY *}    : refers to 2 dimensions ball list (powerBall)
 * @param {FILE *} fp          : refers to output file. If fp != NULL print to output file
 */
void printListXYWithPBByKey(struct ListXY *pl1, struct ListXY *pl2, FILE *fp);
```



```
/** 
 * How many times the balls has been drawn so far
 * 
 * @param {struct ListX *} winningBallStats      : How many times were the winning numbers drawn in the previous draws?
 * @param {struct ListX *} powerBallStats    : How many times were the powerBall numbers drawn in the previous draws?
 */
void getDrawnBallsStats(struct ListX *winningBallStats, struct ListX *powerBallStats);
```



```
/** 
 * Print key-val pair of the items in the list by ball statistics (How many times the balls has been drawn so far)
 * 
 * @param {struct ListX *} ballStats      : refers to balls and the number of times each ball was drawn in previous draws.
 */
void printBallStats(struct ListX *ballStats);
```



```
/**
 * print numbers (double combinations or triple combinations) that love each other (lucky balls)

 * @param {struct ListXY *}     : refers to 2 dimensions ball list (2*y or 3*y)
 * @param {FILE *} fp           : refers to output file. If fp != NULL print to output file
*/
void printLuckyBalls(struct ListXY *pl, FILE *fp);
```



```
/**
 * Return number of term

 * @param {Integer}     : sum of numbers from 1 to n
 * @return {Integer}    : n
*/
int numberOfTerm(int sum);
```



```
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
```



```
/** 
 * Draw based on today's date
 * 
 * @param {struct ListX *} ballStats      : refers to balls and the number of times each ball was drawn in previous draws.
 * @return {struct ListXY *}              : Returns 2 drawn list.
 */
struct ListXY * drawBallByDate(struct ListX *ballStats);
```



```
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
```



```
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
```



```
/** 
 * Blend 1
 * Dividing the list of numbers (ballStats) in the middle, inverting the left and right parts and combining them.
 * The numbers that came out in the previous draws (ballStats) are arranged at the base of the
 * pascal triangle and the ball is dropped on (with gaussIndex function) it and the ball hit is drawn.
 * 
 * @param {struct ListX *} drawnBallsBlend1	: refers to the balls to be drawn. 
 * @param {struct ListX *} ballStats		: refers to balls and the number of times each ball was drawn in previous draws.
 * @param {Integer} totalBall			: Total ball count in the globe
 * @param {Integer} drawBall     		: Number of balls to be drawn
 * @param {Integer} matchComb			: Number of combinations in which the drawn numbers must match any of the previous draws.
 * @param {Integer} elimComb			: If a combination of the drawn numbers matched with any of the previous draws, specified by the
 * 						  elimComb parameter, the draw is renewed.
 * @return {struct ListX *} drawnBallsBlend1	: Returns new drawn balls.
 */
struct ListX * drawBallByBlend1(struct ListX *drawnBallsBlend1, struct ListX *ballStats, int totalBall, int drawBall, int matchComb, int elimComb);
```



```
/** 
 * Blend 2
 * The balls are taken from the left and right of the ballStats and placed from the middle of the globe 
 * (The base of the Pascal's triangle) to the edges
 * 
 * @param {struct ListX *} drawnBallsBlend2	: refers to the balls to be drawn. 
 * @param {struct ListX *} ballStats		: refers to balls and the number of times each ball was drawn in previous draws.
 * @param {Integer} totalBall			: Total ball count in the globe
 * @param {Integer} drawBall        	 	: Number of balls to be drawn
 * @param {Integer} matchComb			: Number of combinations in which the drawn numbers must match any of the previous draws.
 * @param {Integer} elimComb			: If a combination of the drawn numbers matched with any of the previous draws, specified by the
 * 						  elimComb parameter, the draw is renewed.
 * @return {struct ListX *} drawnBallsBlend2	: Returns new drawn balls.
 */
struct ListX * drawBallByBlend2(struct ListX *drawnBallsBlend2, struct ListX *ballStats, int totalBall, int drawBall, int matchComb, int elimComb);
```



```
/** 
 * The balls that drawn the least are placed in the middle of the globe, and the balls that drawn the most are placed on the edges.
 * 
 * @param {struct ListX *} drawnBallsSide	: refers to the balls to be drawn. 
 * @param {struct ListX *} ballStats		: refers to balls and the number of times each ball was drawn in previous draws.
 * @param {Integer} totalBall			: Total ball count in the globe
 * @param {Integer} drawBall        	  	: Number of balls to be drawn
 * @param {Integer} matchComb			: Number of combinations in which the drawn numbers must match any of the previous draws.
 * @param {Integer} elimComb			: If a combination of the drawn numbers matched with any of the previous draws, specified by the
 * 						  elimComb parameter, the draw is renewed.
 * @return {struct ListX *} drawnBallsSide	: Returns new drawn balls.
 */
struct ListX * drawBallBySide(struct ListX *drawnBallsSide, struct ListX *ballStats, int totalBall, int drawBall, int matchComb, int elimComb);
```



```
/** 
 * The most drawn balls are placed in the center of the globe,
 * the least drawn balls are placed at the edges (normal distribution)
 * 
 * @param {struct ListX *} drawnBallsNorm	: refers to the balls to be drawn. 
 * @param {struct ListX *} ballStats		: refers to balls and the number of times each ball was drawn in previous draws.
 * @param {Integer} totalBall			: Total ball count in the globe
 * @param {Integer} drawBall			: Number of balls to be drawn
 * @param {Integer} matchComb			: Number of combinations in which the drawn numbers must match any of the previous draws.
 * @param {Integer} elimComb			: If a combination of the drawn numbers matched with any of the previous draws, specified by the
 *                                	          elimComb parameter, the draw is renewed.
 * 						  For example, a previous draw is [1, 2, 4, 6, 8, 9]
 * 						  the new draw (drawnBallsNorm) is [1, 2, 5, 7, 9, 10]
 * 						  If matchComb = 2 and elimComb = 0, draw is OK, because (1,2) (1,9) or (2,9) matched.
 * 						  If matchComb = 2 and elimComb = 3, draw is renewed, because (1,2,9) eliminated. 
 * @return {struct ListX *} drawnBallsNorm	: Returns new drawn balls.
 */
struct ListX * drawBallByNorm(struct ListX *drawnBallsNorm, struct ListX *ballStats, int totalBall, int drawBall, int matchComb, int elimComb);
```



```
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
```



```
/** 
 * The search5CombXY function searches for 5 combinations in all previous draws. 
 * 
 * @param {struct ListXY *} prvDrawnsList : refers to the balls has been drawn so far. 
 * @param {struct ListX *} drawnBalls     : refers to balls drawn in a new draw. 
 * @return {Integer}                      : Returns 1 if found, 0 if not. 
 */
int search5CombXY(struct ListXY *prvDrawnsList, struct ListX *drawnBalls);
```



```
/** 
 * The search5CombX function searches for 5 combinations in a previous draw. 
 * 
 * @param {struct ListX *} aPrvDrawn      : refers to a drawn balls in the previous draws.
 * @param {struct ListX *} drawnBalls     : refers to balls drawn in a new draw. 
 * @return {Integer}                      : Returns 1 if found, 0 if not. 
 */
int search5CombX(struct ListX *aPrvDrawn, struct ListX *drawnBalls);
```



```
/** 
 * The search4CombXY function searches for 4 combinations in all previous draws. 
 * 
 * @param {struct ListXY *} prvDrawnsList : refers to the balls has been drawn so far. 
 * @param {struct ListX *} drawnBalls     : refers to balls drawn in a new draw. 
 * @return {Integer}                      : Returns 1 if found, 0 if not. 
 */
int search4CombXY(struct ListXY *prvDrawnsList, struct ListX *drawnBalls);
```



```
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
```



```
/** 
 * The search3CombXY function searches for 3 combinations in all previous draws. 
 * 
 * @param {struct ListXY *} prvDrawnsList : refers to the balls has been drawn so far. 
 * @param {struct ListX *} drawnBalls     : refers to balls drawn in a new draw. 
 * @return {Integer}                      : Returns 1 if found, 0 if not. 
 */
int search3CombXY(struct ListXY *prvDrawnsList, struct ListX *drawnBalls);
```



```
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
```



```
/** 
 * The search2CombXY function searches for 2 combinations in all previous draws. 
 * 
 * @param {struct ListXY *} prvDrawnsList : refers to the balls has been drawn so far. 
 * @param {struct ListX *} drawnBalls     : refers to balls drawn in a new draw. 
 * @return {Integer}                      : Returns 1 if found, 0 if not. 
 */
int search2CombXY(struct ListXY *prvDrawnsList, struct ListX *drawnBalls);
```



```
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
```



```
/** 
 * The search1BallXY function searches for 1 ball in a draws list. 
 * 
 * @param {struct ListXY *} couponList    : refers to the balls has been new drawns. 
 * @param {struct ListX *} drawnBalls     : refers to balls drawn in a new draw. 
 * @param {Integer} drawBall     		  : DRAW_BALL for normal ball list or 1 for powerBall list (coupon column count)
 * @return {Integer}                      : Returns 1 if found, 0 if not. 
 */
int search1BallXY(struct ListXY *couponList, struct ListX *drawnBalls, int drawBall);
```



```
/**
 * Calculate matching combinations count of numbers from previous draws
 * it prints the number of matching double, triple, quartet, quintuple, and six combinations.
*/
void calcMatchCombCount();
```



```
/**
 * Calculate matching combinations of numbers from previous draws
 * 
 * @param {Integer} comb    : If 2, it prints matching double combinations along with their dates
 *                            If 3, it prints matching triple combinations along with their dates
 *                            If 4, it prints matching quartet combinations along with their dates
 *                            If 5, it prints matching quintuple combinations along with their dates
 * @param {FILE *} fp       : refers to output file. If fp != NULL print to output file
*/
void calcCombMatch(int comb, FILE *fp);
```



```
/**
 * Get the numbers that love each other drawn together (lucky numbers)
 * 
 * @param {struct ListXY *} luckyBalls    : refers to the lucky balls list
 * @param {Integer} comb                  : double, triple or quartet (2, 3 or 4) combinations
 * @return {struct ListXY *} luckyBalls   : refers to the lucky balls list
*/
struct ListXY * getLuckyBalls(struct ListXY *luckyBalls, int comb);
```



```
/**
 * Draw balls by lucky numbers (the numbers that love each other drawn together)
 * 
 * @param {struct ListXY *} coupon    : refers to the 2 dimensions balls list (coupon)
 * @param {Integer} drawCount         : how many draws will be made
 * @param {Integer} totalDrawCount    : 
*/
void drawBallsByLucky(struct ListXY *coupon, int drawCount, int totalDrawCount);
```



```
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
```



```
/**
 * Draw powerBall
 * 
 * @param {struct ListXY *} coupon    : refers to the 2 dimensions balls list (coupon)
 * @param {Integer} drawCount         : how many draws will be made
*/
void drawBallsPB(struct ListXY *coupon, int drawCount);
```



```
/** 
 * Get drawn balls from file has been drawn so far
 * 
 * @param {struct ListXY *}      : refer to 2 dimensions list of winning numbers
 * @param {struct ListXY *}      : refer to 2 dimensions list of powerBall numbers
 * @param {char *}               : Drawn list file
 */
void getDrawnBallsList(struct ListXY * winningDrawnBallsList, struct ListXY * powerBallDrawnBallsList, char *fileName);
```


# Donate
### BTC : 1CASunNSibqCsR5Y6kniSW2t22Rt5Bwtgu
![alt text](https://github.com/tipirdamaz/migallery/blob/main/donate/btc.png)
### ETH : 0x3ea58816d52d3a18a447b41a9e65aefb32700551
![alt text](https://github.com/tipirdamaz/migallery/blob/main/donate/eth.png)
### USDT : 0x3ea58816d52d3a18a447b41a9e65aefb32700551
![alt text](https://github.com/tipirdamaz/migallery/blob/main/donate/usdt.png)
