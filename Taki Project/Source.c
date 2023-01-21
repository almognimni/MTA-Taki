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
#define CLOCKWISE 1
#define COUNTERCLOCKWISE 0
#define STARTING_CARDS 4

//Error codes//
//1 - Dynamic memory allocation failed

typedef struct Card
{
	/*The numbers 1 - 9 represent the basic cards respectively
	  The numbers 10 - 14 represent: "plus(10), stop(11), change direction(12), TAKI(13) and change color(14)*/
	int cardNum;

	/*The colors represented from 1 - 4 as Yellow Red Blue Green respectively
	  while 0 means no color card */ 
	int cardColor;
	
} Card;

typedef struct Player
{
	char name[MAX_NAME_LEN];
	Card* cards;
	int numOfCards;
	int maxCards;
} Player;

//Function decelerations//
int scanPlayers(Player** players);
void handCards(Player* players, int numOfPlayers);
void printCard(Card card);
void printCards(Player player);
Card genRndCard();
Card changeColor();
Card taki(Player* player, int cardPosition);
Card turn(Player* player, Card cardInPlay, int* playDirection, int* currentPlayerNum);
void removeCard(Player* player, int cardPosition);
Card stop(int* currentPlayerNum, int color);
Card changeDirection(int* gameDirection, int color);
Card plus(int* currentPlayer, int color);
void drawCard(Player* player);

void exampl(int** outputInt, int* outputSize)
{
	int* num = NULL;
	num = (int*)malloc(10 * sizeof(int));
	num[1] = 5;
	*outputInt = num;
	*outputSize = 10;

}

int main()
{
	srand(time(NULL));
	int numOfPlayers;
	Player* players = NULL;
	int playDirection = CLOCKWISE;
	int currentPlayerNum = 0; //Starting at 0
	bool ongoing = true;

	//Welcome massage
	printf("************  Welcome to TAKI game !!! ***********\n");

	//Scan the number of players and their names
	numOfPlayers = scanPlayers(&players);

	//Hand 4 cards for each player
	handCards(players, numOfPlayers);

	//Pick a starting card - If the card is a special card, pick again
	Card cardInPlay;
	do
	{
		cardInPlay = genRndCard();
	} while (cardInPlay.cardNum > 9);

	printf("Upper card:\n");
	printCard(cardInPlay);

	while (ongoing)
	{
		cardInPlay = turn(&players[currentPlayerNum], cardInPlay, &playDirection, &currentPlayerNum);
		//At the end of each turn check if that player won
		if (players[currentPlayerNum].numOfCards == 0)
		{
			//If the last card placed was a plus card, gives another card to the player who placed it
			if (cardInPlay.cardNum == PLUS)
			{
				players[currentPlayerNum].cards[0] = genRndCard();
				players[currentPlayerNum].numOfCards++;
			}

			//If the last card placed was a stop card and there were 2 players in game
			//gives another card to the player who placed it
			if (numOfPlayers == 2 && cardInPlay.cardNum == STOP)
			{
				players[currentPlayerNum].cards[0] = genRndCard();
				players[currentPlayerNum].numOfCards++;
			}
			else // There is a winner
			{
				ongoing = false;
				break;
			}
		}
		
		switch (playDirection)
		{
		case CLOCKWISE:
			currentPlayerNum++;
			break;
		case COUNTERCLOCKWISE:
			currentPlayerNum--;
			break;
		}

		//Keeps the rotation in the range of the number of players
		if (currentPlayerNum >= numOfPlayers)
		{
			currentPlayerNum -= numOfPlayers;
		}
		else if (currentPlayerNum < 0)
		{
			currentPlayerNum += numOfPlayers;
		}

		printf("\nUpper card:\n");
		printCard(cardInPlay);
	}

	//When the while loop ends, it means there is a winner
	printf("The winner is: %s", players[currentPlayerNum].name); //TODO

	//Statistics** TODO

	//printf("The cards of player 1 are:\n");
	//for (int i = 0; i < players[0].numOfCards; i++)
	//printCard(players[0].cards[i]);

	
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
		{
			printf("Invalid choice, please enter a positive number.\n");
		}
			

	} while (numOfPlayers <= 0);

	*players = (Player*)malloc(numOfPlayers * sizeof(Player));
	if (*players == NULL)
	{
		printf("Memory allocation failed!");
		exit(1);
	}
	


	//Getting the name of each player
	//and setting the amount of cards for each player to 0
	//and initializing the "winner" flag to false
	for (int i = 0; i < numOfPlayers; i++)
	{
		printf("Please enter the first name of player #%d:\n", i + 1);
		scanf("%s", (*players + i)->name);
		(*players + i)->numOfCards = 0;
	}
	return numOfPlayers;
}

Card genRndCard()
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

Card changeColor()
{
	Card returnCard;
	returnCard.cardNum = CHANGE_COLOR;
	printf("Please enter your color choice: \n");
	printf("1 - Yellow\n2 - Red\n3 - Blue\n4 - Green\n");
	int colorChoice;
	do
	{
		scanf("%d", &colorChoice);
		switch (colorChoice)
		{
		case 1:
			returnCard.cardColor = YELLOW;
			break;
		case 2:
			returnCard.cardColor = RED;
			break;
		case 3:
			returnCard.cardColor = BLUE;
			break;
		case 4:
			returnCard.cardColor = GREEN;
			break;
		default:
			returnCard.cardColor = NO_COLOR;
			printf("Invalid choice! Please pick a number between 1-4.\n");
		}
	} while (returnCard.cardColor == NO_COLOR);

	return returnCard;

}

//Returns the last card placed
Card taki(Player* player, int cardPosition)
{
	int choice = 0;
	
	//Initialize the last card placed as the current taki card
	Card lastPlacedCard;
	lastPlacedCard = player->cards[cardPosition];
	removeCard(player, cardPosition);

	while (player->numOfCards > 0)
	{
		printf("Upper card:\n");
		printCard(lastPlacedCard);
		printf("Your remaining cards:\n");
		printCards(*player);

		printf("Please enter 0 if you want to finish your turn\nor 1-%d if you want to put one of your cards in the middle:\n", player->numOfCards);
		scanf("%d", &choice);
		cardPosition = choice - 1;

		if (choice = 0)
		{
			return lastPlacedCard;
		}
		else if (player->cards[cardPosition].cardNum == CHANGE_COLOR)
		{
			lastPlacedCard = changeColor();
			removeCard(player, cardPosition);
			return lastPlacedCard;
		}
		else if (player->cards[cardPosition].cardColor = lastPlacedCard.cardColor)
		{
			lastPlacedCard = player->cards[cardPosition];
			removeCard(player, cardPosition);
		}
		else
		{
			printf("Invalid card! Try again.");
		}
	}
	return lastPlacedCard;
	
}

//Gets a pointer to dynamic array of players and the number of players.
//Updates the array to include a dynamic array of 4 cards as a start - the cards are chosen randomly.
void handCards(Player* players, int numOfPlayers)
{
	for (int i = 0; i < numOfPlayers; i++)
	{
		players[i].cards = NULL;
		players[i].cards = (Card*)malloc(STARTING_CARDS * sizeof(Card));
		if (players[i].cards == NULL)
		{
			printf("Memory allocation failed");
			exit(1);
		}
		for (int j = 0; j < STARTING_CARDS; j++)
		{
			players[i].cards[j] = genRndCard();
			players[i].numOfCards++;
		}
		players[i].maxCards = players[i].numOfCards;
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
		break;
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

void printCards(Player player)
{
	for (int i = 0; i < player.numOfCards; i++)
	{
		int cardNumber = i + 1;
		printf("Card #%d:\n", cardNumber);
		printCard(player.cards[i]);
	}
		
}

//Get the current player struct and the struct of the current card in play, a pointer for the current game direction and a pointer to the number of the current player
Card turn(Player* player, Card cardInPlay, int* playDirection, int* currentPlayerNum)
{
	//Print the player's name and it's cards
	printf("%s's turn:\n\n" , player->name);

	printCards(*player);

	int playerChoice;
	int cardPosition;
	bool valid;
	Card returnCard = { 0 };
	do
	{
		valid = true;
		printf("Please enter 0 if you want to take a card from the deck\n");
		printf("or 1 - %d if you want to put one of your cards in the middle:\n", player->numOfCards);
		scanf("%d", &playerChoice);
		cardPosition = playerChoice - 1;

		if (playerChoice == 0)
		{
			drawCard(player);
			return cardInPlay;
		}
		
		else if (playerChoice > player->numOfCards ||
				player->cards[cardPosition].cardColor != cardInPlay.cardColor &&
				player->cards[cardPosition].cardNum != cardInPlay.cardNum &&
				player->cards[cardPosition].cardColor != 0)
		{
			printf("Invalid card! Try again\n");
			valid = false;
		}
	} while (!valid);

	//All other valid options mean the player has chosen a valid card

	//Checks if the chosen card is a special card
	if (player->cards[cardPosition].cardNum > 9)
	{
		if (player->cards[cardPosition].cardNum == CHANGE_COLOR)
		{
			returnCard = changeColor();
		}

		//Special case for the taki card (has a switch for calling other cards functions)
		else if (player->cards[cardPosition].cardNum == TAKI)
		{
			returnCard = taki(player, cardPosition);
			//If the last card placed in a taki is special, applies it's effects
			//The effects of change color are embedded in the taki function itself
			if (returnCard.cardNum > 9)
			{
				switch (returnCard.cardNum) //TODO FINISH CASE
				{
				case PLUS:
					returnCard = plus(currentPlayerNum, returnCard.cardColor);
					break;
				case STOP:
					returnCard = stop(currentPlayerNum, returnCard.cardColor);
					break;
				case CHANGE_DIRECTION:
					returnCard = changeDirection(playDirection, returnCard.cardColor);
					break;
				}
			}
		}
		//End of taki case

		else if (player->cards[cardPosition].cardNum == PLUS)
		{
				returnCard = plus(currentPlayerNum, player->cards[cardPosition].cardColor);
		}

		else if (player->cards[cardPosition].cardNum == STOP)
		{
			returnCard = stop(currentPlayerNum, player->cards[cardPosition].cardColor);
		}

		else if (player->cards[cardPosition].cardNum == CHANGE_DIRECTION)
		{
			returnCard = changeDirection(playDirection, player->cards[cardPosition].cardColor);
	
		}
	}
	//End of special cards cases

	//If the card is a regular card
	else
	{
		returnCard = player->cards[cardPosition];
	}
	
	//If a valid card was chosen, removes the chosen card from the player and set it as the card in play
	removeCard(player, cardPosition);
	return returnCard;
}

void removeCard(Player* player, int cardPosition)
{
	int i = cardPosition;
	for (i; i < player->numOfCards - 1; i++)
	{
		player->cards[i] = player->cards[i + 1];
	}
	player->numOfCards--;

}

Card stop(int* currentPlayerNum, int color)
{
	Card returnCard;
	returnCard.cardNum = STOP;
	returnCard.cardColor = color;

	//Apply the card's effect
	int updatedPlayerNum = *currentPlayerNum;
	updatedPlayerNum++;
	*currentPlayerNum = updatedPlayerNum;
	
	return returnCard;
}

Card changeDirection(int* gameDirection, int color)
{
	Card returnCard;
	returnCard.cardNum = CHANGE_DIRECTION;
	returnCard.cardColor = color;

	//Apply the card's effect
	if (*gameDirection == CLOCKWISE)
	{
		*gameDirection = COUNTERCLOCKWISE;
	}
	else
	{
		*gameDirection = CLOCKWISE;
	}

	return returnCard;
}

Card plus(int* currentPlayer, int color)
{
	Card returnCard;
	returnCard.cardNum = PLUS;
	returnCard.cardColor = color;

	//Apply the card's effect
	int updatedPlayerNum = *currentPlayer;
	updatedPlayerNum--;
	*currentPlayer = updatedPlayerNum;

	return returnCard;
}

void drawCard(Player* player)
{
	//Check if there is a need to allocate more memory to draw another card
	if (player->numOfCards == player->maxCards)
	{
		//If so, reallocate more
		player->cards = (Card*)realloc(player->cards, (player->maxCards * 2) * sizeof(Card));
		//Check if memory reallocation failed
		if (player->cards == NULL)
		{
			exit(1);
		}

		//Update the tracker for the physical size
		player->maxCards *= 2;
	}

	player->cards[player->numOfCards] = genRndCard();
	player->numOfCards++;
}