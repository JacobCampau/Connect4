// Name: Andrew Hsu, Jacob Campau, Casanova Segura
// Date: 12/04/2023
// Purpose: Final Group Project

#include <stdio.h>
#include <stdlib.h>

#define MAX_STR 50
#define MAX_ROWS 6
#define MAX_COLUMNS 7
#define SAVE_FILE "save_file.txt"
#define NUM_PLAYERS 2

int saveGame(int rows, int cols, char boardArray[][cols], int turnNum, int strMax, char names[][strMax]);
int loadGame(int rows, int cols, char boardArray[][cols], int strMax, char names[][strMax]);

void displayBoard(int rows, int cols, char boardArray[][cols]);
void clearBoard(int rows, int cols, char boardArray[][cols]);
void displayPlayers(int strMax, char names[][strMax], int turnNum);

int getRow(int row, int col, char boardArray[][col], int choice);
int checkRow(int col, char boardArray[][col], int pieceRow);
int checkCol(int row, int col, char boardArray[][col], int pieceCol);
int checkDiagonal(int row, int col, char boardArray[][col], int pieceRow, int pieceCol);

int playGame(int row, int col, int turnNum, char boardArray[][col], int maxStr, char names[][maxStr], int* winNum, int* choice);
void exitScreen(int row, int col, int choice, char boardArray[][col], int maxStr, char names[][maxStr], int turnNum);

int main(){
	int menuChoice, turn = 1, gameChoice, r, winner;
	char board[MAX_ROWS][MAX_COLUMNS], plyrNames[NUM_PLAYERS][MAX_STR];
	char temp1, temp2;
	
	system("clear");
	
	do{	
		//MENU
		printf("CONNECT 4\n");
		printf("1. Play a game\n");
		printf("2. Load an existing game\n");
		printf("0. EXIT\n");
		
		//PLAYER CHOICE
		printf("Option: ");
		scanf("%d", &menuChoice);
		printf("\n");
		
		switch(menuChoice){
			case 0:
				break;
			case 1: 
				//PLAYER NAMES
				printf("Player 1 Name (X): ");
				scanf("%c", &temp1);
				fgets(plyrNames[0], MAX_STR, stdin);
				
				printf("Player 2 Name (O): ");
				fgets(plyrNames[1], MAX_STR, stdin);
				printf("\n");
				
				clearBoard(MAX_ROWS, MAX_COLUMNS, board);
				
				//GAME
				turn = 1;
				winner = 0;
				
				turn = playGame(MAX_ROWS, MAX_COLUMNS, turn, board, MAX_STR, plyrNames, &winner, &gameChoice);
				
				//END OF GAME
				system("clear");
				displayBoard(MAX_ROWS, MAX_COLUMNS, board);
				
				if(winner != 0 && gameChoice != 0){
					printf("Winner: %s\n", plyrNames[winner-1]);
				}else if(gameChoice != 0){
					printf("No Winner, stalemate.\n");
				}
				
				exitScreen(MAX_ROWS, MAX_COLUMNS, gameChoice, board, MAX_STR, plyrNames, turn);
				
				break;
			case 2: 
				turn = loadGame(MAX_ROWS, MAX_COLUMNS, board, MAX_STR, plyrNames);
				
				//GAME
				if(turn < 44){
					turn = playGame(MAX_ROWS, MAX_COLUMNS, turn, board, MAX_STR, plyrNames, &winner, &gameChoice);
					
					//END OF GAME
					system("clear");
					displayBoard(MAX_ROWS, MAX_COLUMNS, board);
					
					if(winner != 0 && gameChoice != 0){
						printf("Winner: %s\n", plyrNames[winner-1]);
						
						clearBoard(MAX_ROWS, MAX_COLUMNS, board);
						saveGame(MAX_ROWS, MAX_COLUMNS, board, turn, MAX_STR, plyrNames);
					}else if(gameChoice != 0){
						printf("No Winner, stalemate.\n");
					}
					
					exitScreen(MAX_ROWS, MAX_COLUMNS, gameChoice, board, MAX_STR, plyrNames, turn);
				}else{
					printf("No Game To Load\n");
				}
				
				break;
			default:
				printf("Please enter a valid option.\n");
		}
	}
	while(menuChoice != 0);
	
	return 0;
}

int saveGame(int rows, int cols, char boardArray[][cols], int turnNum, int strMax, char names[][strMax]){
	FILE* fp;
	int r, c, n;
	
	fp = fopen(SAVE_FILE, "w");
	if(fp == NULL){
		printf("File did not open.\n");
		return 0; //ends function here if error
	}
	
	for(r = 0; r < rows; r++){
		for(c = 0; c < cols; c++){
			fprintf(fp, "(%c)", boardArray[r][c]);
		}
	}
	fprintf(fp, "\n");
	
	fprintf(fp, "%s", names[0]);
	fprintf(fp, "%s", names[1]);
	fprintf(fp, "Turn: %d", turnNum);
	
	fclose(fp);
}

int loadGame(int rows, int cols, char boardArray[][cols], int strMax, char names[][strMax]){
	FILE* fp;
	int r, c, turnNum;
	char junk[2];
	
	fp = fopen(SAVE_FILE, "r");
	if(fp == NULL){
		printf("File did not open.\n");
		return 0; //ends function here if error
	}
	
	for(r = 0; r < rows; r++){
		for(c = 0; c < cols; c++){
			fscanf(fp, "(%c)", &boardArray[r][c]);
		}
	}
	
	fscanf(fp, "%c", junk); //left over null character from scanf in the game, this removes it
	
	fgets(names[0], strMax, fp);
	fgets(names[1], strMax, fp);
	
	fscanf(fp, "Turn: %d", &turnNum);
	return turnNum;
	
	fclose(fp);
}

void displayBoard(int rows, int cols, char boardArray[][cols]){
	int i, j, k;
	
	for(k = 0; k < cols; k++){
		printf(" %d ", k+1);
	}
	
	for(i = 0; i < rows; i++){
		printf("\n");
		for(j = 0; j < cols; j++){
			printf("(%c)", boardArray[i][j]);
		}
	}
	printf("\n\n");
}

void clearBoard(int rows, int cols, char boardArray[][cols]){
	int i, j;
	
	for(i = 0; i < rows; i++){
		for(j = 0; j < cols; j++){
			boardArray[i][j] = ' ';
		}
	}
}

void displayPlayers(int strMax, char names[][strMax], int turnNum){
	if(turnNum%2 != 0){
		printf("Player up (X): %s\n", names[0]);
	}else{
		printf("Player up (O): %s\n", names[1]);
	}
	printf("Turn: %d\n\n", turnNum);
}

int getRow(int row, int col, char boardArray[][col], int choice){
	int r;
	
	for(r = (row-1); r >= 0; r--){
		if(boardArray[r][choice-1] == ' '){
			return r;
		}
	}
}

int checkRow(int col, char boardArray[][col], int pieceRow){
	int c, connectX = 0, connectO = 0;
	
	for(c = 0; c < col; c++){
		if(boardArray[pieceRow][c] == 'X'){
			connectX++;
			connectO = 0;
		}else if(boardArray[pieceRow][c] == 'O'){
			connectX = 0;
			connectO++;
		}else{
			connectX = 0;
			connectO = 0;
		}
		
		if(connectX == 4){
			return 1;
		}else if(connectO == 4){
			return 2;
		}
	}
	return 0;
}

int checkCol(int row, int col, char boardArray[][col], int pieceCol){
	int r, connectX = 0, connectO = 0;
	
	for(r = 0; r < row; r++){
		if(boardArray[r][pieceCol] == 'X'){
			connectX++;
			connectO = 0;
		}else if(boardArray[r][pieceCol] == 'O'){
			connectX = 0;
			connectO++;
		}else{
			connectX = 0;
			connectO = 0;
		}
		
		if(connectX == 4){
			return 1;
		}else if(connectO == 4){
			return 2;
		}
	}
	return 0;
}

int checkDiagonal(int row, int col, char boardArray[][col], int pieceRow, int pieceCol){
	int connect = 1, change;
	char compare = boardArray[pieceRow][pieceCol];
	
	//top left
	for(change = 1; change < 4; change++){
		if((pieceRow - change) < (row) && (pieceCol - change) < (col)){	
			if(boardArray[pieceRow - change][pieceCol - change] == compare){
				connect++;
			}else{
				break;
			}
		}
	}
	
	//bottom right
	for(change = 1; change < 4; change++){
		if((pieceRow + change) < (row) && (pieceCol + change) < (col)){	
			if(boardArray[pieceRow + change][pieceCol + change] == compare){
				connect++;
			}else{
				break;
			}
		}
	}
	
	if(connect >= 4){
		if(compare == 'X'){
			return 1;
		}else{
			return 2;
		}
	}
	
	//reset before checking other diagonal
	connect = 1;
	
	//top right
	for(change = 1; change < 4; change++){
		if((pieceRow - change) < (row) && (pieceCol + change) < (col)){	
			if(boardArray[pieceRow - change][pieceCol + change] == compare){
				connect++;
			}else{
				break;
			}
		}
	}
	
	//bottom left
	for(change = 1; change < 4; change++){
		if((pieceRow + change) < (row) && (pieceCol - change) < (col)){
			if(boardArray[pieceRow + change][pieceCol - change] == compare){
				connect++;
			}else{
				break;
			}
		}
	}
	
	if(connect >= 4){
		if(compare == 'X'){
			return 1;
		}else{
			return 2;
		}
	}
	
	return 0;
}

int playGame(int row, int col, int turnNum, char boardArray[][col], int maxStr, char names[][maxStr], int* winNum, int* choice){
	int r;
	
	do{
		system("clear");
		
		displayBoard(row, col, boardArray);
		displayPlayers(maxStr, names, turnNum);
		
		printf("Choose a column (0 to EXIT): ");
		scanf("%d", choice);
		
		if(*choice != 0){
			while((boardArray[0][*choice-1] != ' ') || *choice > col){
				printf("\nPlease select an empty column\n\n");
				printf("Choose a row: ");
				scanf("%d", choice);
				printf("\n");
			}
		
			r = getRow(row, col, boardArray, *choice);
				
			if(turnNum%2 != 0){
				boardArray[r][*choice-1] = 'X';
			}else{
				boardArray[r][*choice-1] = 'O';
			}
			
			if (checkRow(col, boardArray, r) != 0){
				*winNum = checkRow(col, boardArray, r);
			}else if (checkCol(row, col, boardArray, *choice - 1) != 0){
				*winNum = checkCol(row, col, boardArray, *choice - 1);
			}else if (checkDiagonal(row, col, boardArray, r, *choice - 1) != 0){
				*winNum = checkDiagonal(row, col, boardArray, r, *choice - 1);
			}
			
			if(*winNum != 0){
				return turnNum;
			}
			
			turnNum++;
		}
	}while((turnNum < 44) && *choice != 0);
	
	return turnNum;
}

void exitScreen(int row, int col, int choice, char boardArray[][col], int maxStr, char names[][maxStr], int turnNum){
	int exitChoice = 0;
	
	if(choice == 0){
		printf("CHOOSE EXIT OPTION\n");
		printf("1. EXIT\n");
		printf("2. EXIT AND SAVE\n");
		printf("Option: ");
		scanf("%d", &exitChoice);
		
		while (exitChoice != 1 && exitChoice != 2){
			printf("Please Enter a Valid Option\n\n");
			
			printf("CHOOSE EXIT OPTION\n");
			printf("1. EXIT\n");
			printf("2. EXIT AND SAVE\n");
			printf("Option: ");
			scanf("%d", &exitChoice);
		}
		
		if(exitChoice == 2){
			saveGame(row, col, boardArray, turnNum, maxStr, names);
			printf("\n= = SAVING GAME = =\n\n");
		}else{
			printf("\n");
		}
	}
}
