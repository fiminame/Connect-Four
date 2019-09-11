#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <windows.h>


#pragma warning(disable:4996)
#define BOARD_ROWS 6
#define BOARD_COLS 7

void printBoard(char(*board)[BOARD_COLS]);
int AItakeTurn(char(*board)[BOARD_COLS], int player, const char *PIECES, int level);
int PVPtakeTurn(char(*board)[BOARD_COLS], int player, const char *PIECES);
int checkWin(char(*board)[BOARD_COLS]);
int horizontalCheck(char(*board)[BOARD_COLS]);
int verticalCheck(char(*board)[BOARD_COLS]);
int diagonalCheck(char(*board)[BOARD_COLS]);
int checkFirst(int first);

int checkMode(int mode);
extern int FindNextMove(int level);
extern void FindCurrentBoardStatus();
//extern void init();
extern int HintChoice(int turn, char stone);

char board[BOARD_ROWS][BOARD_COLS];

int main() {
	//init();

	const char *PIECES = "OX";

	memset(board, ' ', BOARD_ROWS * BOARD_COLS);

	int turn, done = 0;
	int mode = 0;
	int first = 0;
	int level = 0;

	printf("1. PvP mode\n2. AI mode\n������ ��带 �����ϰ� ���͸� �����ּ���! : ");
	scanf("%d", &mode);
	system("cls");

	if (mode == 1) {
		for (turn = 0; turn < BOARD_ROWS * BOARD_COLS && !done; turn++) {
			printBoard(board);
			while (!PVPtakeTurn(board, turn % 2, PIECES)) {
				printBoard(board);
				puts("**column full**\n");
			}
			done = checkWin(board);
		}
		printBoard(board);

		if (turn == BOARD_ROWS * BOARD_COLS && !done) {
			puts("It's a tie!");
		}
		else {
			turn--;
			if (turn % 2 == checkFirst(0)) printf("Player %d�� �̰���ϴ�!\n", turn % 2 + 1);
			else printf("player %d�� �̰���ϴ�!\n", turn % 2 + 1);
		}

		return 0;

	}
	else
	{
		printf("***���̵� ����***\n");
		printf("1. ���̵� - ��\n");
		printf("2. ���̵� - ��\n");
		printf("3. ���̵� - ��\n");
		printf("���̵��� �����ϰ� ���͸� �����ּ���! :");
		scanf("%d", &level);
		system("cls");

		if (level == 1) level = 12;
		else if (level == 2) level = 4;
		else level = 2;

		printf("1. Player ����\n2. AI ����\n������ ��带 �����ϰ� ���͸� �����ּ���! : ");
		scanf("%d", &first);
		checkFirst(first);
		for (turn = 0; turn < BOARD_ROWS * BOARD_COLS && !done; turn++) {
			printBoard(board);
			while (!AItakeTurn(board, turn % 2, PIECES, level)) {
				printBoard(board);
				puts("**Column full!**\n");
			}
			done = checkWin(board);
		}
		printBoard(board);

		if (turn == BOARD_ROWS * BOARD_COLS && !done) {
			puts("It's a tie!");
		}
		else {
			turn--;
			if (turn % 2 == checkFirst(0)) printf("Player (%c) win", PIECES[turn % 2]);
			else printf("AI (%c) wins!\n", PIECES[turn % 2]);
		}

		return 0;


	}




}
void printBoard(char(*board)[BOARD_COLS]) {
	int row, col;

	//system("cls");
	puts("\n    ****Connect Four****\n");
	for (row = BOARD_ROWS - 1; row >= 0; row--) {
		for (col = 0; col < BOARD_COLS; col++) {
			if (board[row][col] == 'O') {
				printf("| ");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
				printf("%c ", board[row][col]);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			}
			else if (board[row][col] == 'X') {
				printf("| ");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
				printf("%c ", board[row][col]);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			}
			else if (board[row][col] == ' ') {
				printf("| %c ", board[row][col]);
			}
				

		}
		puts("|");
		puts("-----------------------------");

	}
	puts("  1   2   3   4   5   6   7\n");
}
int PVPtakeTurn(char(*board)[BOARD_COLS], int player, const char *PIECES) {
	int row = 0, col = 0;
	if (player == 0) {
		printf("Player 1 turn! : ");
		while (1) {
			if (1 != scanf("%d", &col) || col < 1 || col > 7) {
				while (getchar() != '\n');
				puts("Number out of bounds! Try again.");
			}
			else {
				break;
			}
		}
		col--;
		for (row = 0; row < BOARD_ROWS; row++) {
			if (board[row][col] == ' ') {
				board[row][col] = PIECES[player];
				return 1;
			}
		}
	}
	else {
		printf("Player 2 turn! : ");
		while (1) {
			if (1 != scanf("%d", &col) || col < 1 || col > 7) {
				while (getchar() != '\n');
				puts("Number out of bounds! Try again.");
			}
			else {
				break;
			}
		}
		col--;
		for (row = 0; row < BOARD_ROWS; row++) {
			if (board[row][col] == ' ') {
				board[row][col] = PIECES[player];
				return 1;
			}
		}
	}

}
int AItakeTurn(char(*board)[BOARD_COLS], int player, const char *PIECES, int level) {
	int row, col = 0;

	if (player == checkFirst(0)) {
		printf("Player ����! ��Ʈ�� ���ʹٸ� 0�� �Է��ϼ��� :");

		while (1) {
			if (1 != scanf("%d", &col) || col < 0 || col > 7) {
				while (getchar() != '\n');
				puts("Number out of bounds! Try again.");
			}
			else {
				break;
			}
		}

		if (col == 0) {
			char stone = (checkFirst(0) == 0) ? 'X' : 'O';
			int hint = HintChoice(6, stone);
			printf("%d���� �����ϴ� ���� �����ϴ�!", hint + 1);
			printf("\n�����ϼ��� :");
			while (1) {
				if (1 != scanf("%d", &col) || col < 1 || col > 7) {
					while (getchar() != '\n');
					puts("Number out of bounds! Try again.");
				}
				else {
					break;
				}
			}
			col--;
			for (row = 0; row < BOARD_ROWS; row++) {
				if (board[row][col] == ' ') {
					board[row][col] = PIECES[player];
					return 1;
				}
			}


		}
		else {
			col--;
			for (row = 0; row < BOARD_ROWS; row++) {
				if (board[row][col] == ' ') {
					board[row][col] = PIECES[player];
					return 1;
				}
			}
		}
	}
	else {
		printf("AI Turn!\n");

		col = FindNextMove(level);

		for (row = 0; row < BOARD_ROWS; row++) {
			if (board[row][col] == ' ') {
				board[row][col] = PIECES[player];
				return 1;
			}
		}
	}
	return 0;

}
int checkWin(char(*board)[BOARD_COLS]) {
	return (horizontalCheck(board) || verticalCheck(board) || diagonalCheck(board));
}
int horizontalCheck(char(*board)[BOARD_COLS]) {
	for (int row = 0; row < BOARD_ROWS; row++) {
		for (int col = 0; col < BOARD_COLS - 3; col++) {
			char temp = board[row][col];
			if ((temp != ' ') &&
				(board[row][col + 1] == temp) &&
				(board[row][col + 2] == temp) &&
				(board[row][col + 3] == temp))
			{
				return 1;
			}
		}
	}
	return 0;

}
int verticalCheck(char(*board)[BOARD_COLS]) {
	for (int row = 0; row < BOARD_ROWS - 3; row++) {
		for (int col = 0; col < BOARD_COLS; col++) {
			char temp = board[row][col];
			if ((temp != ' ') &&
				(board[row + 1][col] == temp) &&
				(board[row + 2][col] == temp) &&
				(board[row + 3][col] == temp))
			{
				return 1;
			}
		}
	}
	return 0;

}
int diagonalCheck(char(*board)[BOARD_COLS]) {
	for (int row = 0; row < BOARD_ROWS - 3; row++) {
		for (int col = 0; col < BOARD_COLS - 3; col++) {
			char temp = board[row][col];
			if ((temp != ' ') &&
				(board[row + 1][col + 1] == temp) &&
				(board[row + 2][col + 2] == temp) &&
				(board[row + 3][col + 3] == temp))
			{
				return 1;
			}
		}
	}

	for (int row = 0; row < BOARD_ROWS - 3; row++) {
		for (int col = 3; col < BOARD_COLS; col++) {
			char temp = board[row][col];
			if ((temp != ' ') &&
				(board[row + 1][col - 1] == temp) &&
				(board[row + 2][col - 2] == temp) &&
				(board[row + 3][col - 3] == temp))
			{
				return 1;
			}
		}
	}
	return 0;

}
int checkFirst(int first) { // Player First == 0, AI First == 1
	static int n = -1;
	if (n == -1) n = first - 1;
	return n;
}
int checkMode(int mode) {
	static int n = -1;
	if (n == -1) n = mode - 1;
	return n;
}