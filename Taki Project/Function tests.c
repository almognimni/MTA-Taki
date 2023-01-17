#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NAME_LEN 21
#define PLUS 10
#define STOP 11
#define CHANGE_DIRECTION 12
#define TAKI 13
#define CHANGE_COLOR 14
#define NO_COLOR 0
#define RED 1
#define BLUE 2
#define GREEN 3
#define YELLOW 4

//Functions
void printCard(char cardNum[], char cardColor)
{
	printf("*********\n");
	printf("*       *\n");
	printf("*%*s%*s*\n", (unsigned int) (4 + strlen(cardNum) / 2), cardNum, (unsigned int) (3 - strlen(cardNum) / 2), "");
	if (cardColor != 0)
		printf("*   %c   *\n", cardColor);
	printf("*       *\n");
	printf("*********\n");
}


int main()
{
	char card1[] = "plus";
	char color = 'g';
	printCard(card1, color);
}