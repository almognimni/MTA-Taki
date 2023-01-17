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
#define YELLOW 1
#define RED 2
#define BLUE 3
#define GREEN 4

//Error codes//
//1 - Dynamic memory allocation failed

typedef struct Card
{
	/*The numbers 1 - 9 represent the basic cards respectively
	  The numbers 10 - 14 represent: "plus(10), stop(11), change direction(12), TAKI(13) and change color(14)*/
	int cardNum;

	/*The colors represented from 1 - 4 as Yellow Red Blue Green respectivly
	  while 0 means no color card */ 
	int cardColor;
	
} Card;

typedef struct Player
{
	char name[MAX_NAME_LEN];
	Card* cards;
	int numOfCards;
} Player;

//Function decelerations//
int scanPlayers(Player** players);
void handCards(Player* players, int numOfPlayers);
void printCard(Card card);
Card pickRandomCard();

int main()
{
	srand(time(NULL));
	int numOfPlayers;
	int currentCardInPlay;
	Player* players = NULL;
	//Welcome massage
	printf("************  Welcome to TAKI game !!! ***********\n");
	//Scan the number of players and their names
	numOfPlayers = scanPlayers(&players);
	//Hand 4 cards for each player
	handCards(players, numOfPlayers);
	

	printf("The cards of player 1 are:\n");
	for (int i = 0; i < players[0].numOfCards; i++)
	printCard(players[0].cards[i]);

	
}

//Function implementation

//Gets the *address* of the array of players 
//Scans the number of players (positive integer) while checking the input, 
//Creates a dynamic array of "Player" structs, scan and updates the players names
//and returns the amount of players
int scanPlayers(Player** players)
{
	int numOfPlayers;
	printf("Please enter the number of players:\n");
	do
	{
		scanf("%d", &numOfPlayers);
		if (numOfPlayers <= 0)
			printf("Invalid choice, please enter a positive number.\n");
	} while (numOfPlayers <= 0);

	*players = (Player*)malloc(numOfPlayers * sizeof(Player));
	if (*players == NULL)
	{
		printf("Memory allocation failed!");
		exit(1);
	}
	


	//Getting the name of each player
	//and setting the amount of cards for each player to 0
	for (int i = 0; i < numOfPlayers; i++)
	{
		printf("Please enter the first name of player #%d:\n", i + 1);
		scanf("%s", (*players + i)->name);
		(*players + i)->numOfCards = 0;
	}
	return numOfPlayers;
}

Card pickRandomCard()
{
	Card card;
	card.cardNum = (rand() % 14 + 1);
	if (card.cardNum == 14)
		card.cardColor = 0;
	else
	{
		card.cardColor = (rand() % 4 + 1);
	}
	return card;
}

//Gets a pointer to dynamic array of players and the number of players.
//Updates the array to include a dynamic array of 4 cards as a start - the cards are chosen randomly.
void handCards(Player* players, int numOfPlayers)
{
	for (int i = 0; i < numOfPlayers; i++)
	{
		(players + i)->cards = NULL;
		(players + i)->cards = (Card*)malloc(4 * sizeof(Card));
		if ((players + i)->cards == NULL)
		{
			printf("Memory allocation failed");
			exit(1);
		}
		for (int j = 0; j < 4; j++)
		{
			(players + i)->cards[j] = pickRandomCard();
			(players + i)->numOfCards++;
		}
	}
}

//Gets a card name as a string and color as a character and prints the card
void printCard(Card card)
{
	char printName[6] = { 0 };
	char printColor;
	//A switch for replacing the card's name integer with the appropriate text
	switch (card.cardNum)
	{
	case PLUS:
		strcpy(printName, "+");
		break;
	case STOP:
		strcpy(printName, "STOP");
		break;
	case CHANGE_DIRECTION:
		strcpy(printName, "<->");
		break;
	case TAKI:
		strcpy(printName, "TAKI");
		break;
	case CHANGE_COLOR:
		strcpy(printName, "COLOR");
	default:
		//char printNum[] = { card.cardNum - '0' };
		printName[0] = card.cardNum + '0';
		printName[1] = '\0';
		break;
	}

	//A switch for replacing the card's color integer with the appropriate text
	switch (card.cardColor)
	{
	case YELLOW:
		printColor = 'Y';
		break;
	case RED:
		printColor = 'R';
		break;
	case BLUE:
		printColor = 'B';
		break;
	case GREEN:
		printColor = 'G';
		break;
	default:
		printColor = ' ';
		break;
	}
	printf("*********\n");
	printf("*       *\n");
	printf("*%*s%*s*\n", (unsigned int)(4 + strlen(printName) / 2), printName ,(unsigned int) (3 - strlen(printName) / 2), "");
	if (printColor != 0)
		printf("*   %c   *\n", printColor);
	printf("*       *\n");
	printf("*********\n\n");
}