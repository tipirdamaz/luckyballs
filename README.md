![alt text](https://github.com/tipirdamaz/luckyballs/blob/main/screenshots/preview.jpg)

# luckyballs
LuckyBalls - Lotto Statistics and Ball Draw Algorithms In C


WARNING! THIS PROGRAM DOES NOT GUARANTEE YOU WINNING THE LOTTO.

This program provides statistics such as how many times the numbers in the previous draws came out, how many times the numbers that came out together in the draws came out together and how many days apart.

It allows you to draw a lotto by using the information of the past draws.

This program offers you useful 1 and 2 dimensional data structures created with linked list. It offers the ability to search and sort within data structures.


PACKAGE CONTENTS

      source            : C source files

         lotto.c        : Turkey Sayisal Lotto and Super Lotto
         powerball.c    : American PowerBall Lotto


      DOS               : For DOS operating system. 

         POWERBLL.EXE   : MS DOS executable files.
         SAYISL16.EXE   : These files were compiled with the 16-bit Borland Turbo C++ compiler on the Windows XP operating system.
         SUPER16.EXE

         powerbll.txt
         sayisal.txt
         super.txt


      powerball.txt        : TXT files are statistics files containing the results of previous draws.
      sayisal.txt          : These files must be located in the same directory as the executable files.
      super.txt

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


COMPILER

      gcc compiler


PLATFORM

      Source files were compiled with gcc compiler and successfuly tested on

      Windows 10
      macOS Mojave 10.14.6
      centOS 7 Linux



HOW TO COMPILE FILES?

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



SCREENSHOTS

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
