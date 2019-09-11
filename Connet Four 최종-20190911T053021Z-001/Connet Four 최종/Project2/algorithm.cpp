#include <stdio.h>
#include <stdlib.h>
#include <time.h> // �ӽ������� ���� �Լ�
#include <windows.h>
#include "Structure.h" // vector include ����, node ����ü ����, removeNode, recoverNode �Լ�

using namespace std; // vector ������ �ʿ��� �κ�

vector<node> OWin;   // O �¸� �׷�� vector
vector<node> XWin;   // X �¸� �׷�� vector
vector<node> OTemp;   // O �������� �׷�� vector
vector<node> XTemp;   // X �������� �׷�� vector

#define BOARD_ROWS 6
#define BOARD_COLS 7

extern int checkFirst(int);
void FindCurrentBoardStatus(); // ���� ���� ���¿� ���� OWin, XWin, OTemp, XTemp�� �����ϴ� �Լ�
int FindScore(char);
int FindBoardHeight(int);
int FindHeuristic(int, bool, int, char, int, int, int);
int FindMaxChoice(int, char);
extern char board[][BOARD_COLS];

int FindNextMove(int level) {
	char stone = (checkFirst(0) == 0) ? 'X' : 'O'; // checkFirst���� 0�̸� Player First, 1�̸� AI First
	FindCurrentBoardStatus();
	int column;

	time_t startTime = clock();
	column = FindMaxChoice(level, stone);
	time_t endTime = clock();
	printf("Execution Time : %.3lf", (float)(endTime - startTime) / (CLOCKS_PER_SEC));
	srand(time(NULL));

	return column;
}

void init() { // �ڵ� ���۽ÿ� init �Լ��� 1�� ����, ��� ������ Square�� Group�� ���Ϳ� ����
	for (int i = 0; i < BOARD_ROWS; i++) {
		for (int j = 0; j < BOARD_COLS - 3; j++) {
			int num[4][2] = { { i, j },{ i, j + 1 },{ i, j + 2 },{ i, j + 3 } }; // ����
			node n;
			for (int p = 0; p < 4; p++) {
				for (int q = 0; q < 2; q++) {
					n.pos[p][q] = num[p][q]; // node n�� ���� ��� pos�� ���� �ְ�
				}
			}
			OWin.push_back(n); // OWin�� push
			XWin.push_back(n); // XWin�� push
		}
	}
	for (int i = 0; i < BOARD_ROWS - 3; i++) {
		for (int j = 0; j < BOARD_COLS; j++) {
			int num[4][2] = { { i, j },{ i + 1, j },{ i + 2, j },{ i + 3, j } }; // ����
			node n;
			for (int p = 0; p < 4; p++) {
				for (int q = 0; q < 2; q++) {
					n.pos[p][q] = num[p][q];
				}
			}
			OWin.push_back(n);
			XWin.push_back(n);
		}
	}
	for (int i = 0; i < BOARD_ROWS - 3; i++) {
		for (int j = 0; j < BOARD_COLS - 3; j++) {
			int num[4][2] = { { i, j },{ i + 1, j + 1 },{ i + 2, j + 2 },{ i + 3, j + 3 } }; // 45�� �밢��
			node n;
			for (int p = 0; p < 4; p++) {
				for (int q = 0; q < 2; q++) {
					n.pos[p][q] = num[p][q];
				}
			}
			OWin.push_back(n);
			XWin.push_back(n);
		}
	}
	for (int i = BOARD_ROWS - 1; i >= 3; i--) {
		for (int j = 0; j < BOARD_COLS - 3; j++) {
			int num[4][2] = { { i, j },{ i - 1, j + 1 },{ i - 2, j + 2 },{ i - 3, j + 3 } }; // 135�� �밢��
			node n;
			for (int p = 0; p < 4; p++) {
				for (int q = 0; q < 2; q++) {
					n.pos[p][q] = num[p][q];
				}
			}
			OWin.push_back(n);
			XWin.push_back(n);
		}
	}
}
void FindCurrentBoardStatus() {
	recoverNode(&OWin, &OTemp); // OWin ���󺹱�
	recoverNode(&XWin, &XTemp); // XWin ���󺹱�
	for (int i = 0; i < BOARD_ROWS; i++) {
		for (int j = 0; j < BOARD_COLS; j++) { // ���带 ���鼭
			if (board[i][j] == 'X') removeNode(&OWin, &OTemp, i, j); // x�� ��ǥ (i, j)�� �ִ�? OWin���� node ����
			else if (board[i][j] == 'O') removeNode(&XWin, &XTemp, i, j); // �ݴ�
		}
	}

	//printf("O left moving : %d\nX left moving : %d\n", OWin.size(), XWin.size());
}
int FindScore(char stone) { // �� �Լ�
	if (stone == 'O') {
		for (node n : XWin) {
			if (board[n.pos[0][0]][n.pos[0][1]] == 'X' &&
				board[n.pos[1][0]][n.pos[1][1]] == 'X' &&
				board[n.pos[2][0]][n.pos[2][1]] == 'X' &&
				board[n.pos[3][0]][n.pos[3][1]] == 'X') {
				return -1000;
			}
		}
		for (node n : OWin) {
			if (board[n.pos[0][0]][n.pos[0][1]] == 'O' &&
				board[n.pos[1][0]][n.pos[1][1]] == 'O' &&
				board[n.pos[2][0]][n.pos[2][1]] == 'O' &&
				board[n.pos[3][0]][n.pos[3][1]] == 'O') {
				return 1000;
			}
		}
	}
	else {
		for (node n : OWin) {
			if (board[n.pos[0][0]][n.pos[0][1]] == 'O' &&
				board[n.pos[1][0]][n.pos[1][1]] == 'O' &&
				board[n.pos[2][0]][n.pos[2][1]] == 'O' &&
				board[n.pos[3][0]][n.pos[3][1]] == 'O') {
				return -1000;
			}
		}

		for (node n : XWin) {
			if (board[n.pos[0][0]][n.pos[0][1]] == 'X' &&
				board[n.pos[1][0]][n.pos[1][1]] == 'X' &&
				board[n.pos[2][0]][n.pos[2][1]] == 'X' &&
				board[n.pos[3][0]][n.pos[3][1]] == 'X') {
				return 1000;
			}
		}
	}



	float Oscore = 0, Xscore = 0;


	int evaluationTable[BOARD_ROWS][BOARD_COLS] = { { 3, 4, 5, 7, 5, 4, 3 },
	{ 4, 6, 8, 10, 8, 6, 4 },
	{ 5, 8, 11, 13, 11, 8, 5 },
	{ 5, 8, 11, 13, 11, 8, 5 },
	{ 4, 6, 8, 10, 8, 6, 4 },
	{ 3, 4, 5, 7, 5, 4, 3 } };

	for (int i = 0; i < BOARD_ROWS; i++) {
		for (int j = 0; j < BOARD_COLS; j++) {
			switch (board[i][j]) {
			case 'O':
				Oscore += evaluationTable[i][j];
				break;
			case 'X':
				Xscore += evaluationTable[i][j];
				break;
			default:
				break;
			}
		}
	}

	return (stone == 'O') ? (int)(Oscore - Xscore) : (int)(Xscore - Oscore);
}
int FindBoardHeight(int col) { // parameter col�� ���� �ְ����� ã�´�.
	int row;
	for (row = 0; row < BOARD_ROWS; row++) {
		if (board[row][col] == ' ') {
			return row;
		}
	}
	return row;
}
int FindHeuristic(int turn, bool minmax, int choice, char stone, int alpha, int beta, int level) { // turn n���� �޸���ƽ�ϰ� ���� �Ǵ��ؼ� �� ��ȯ
	if (FindBoardHeight(choice) == 6) return -2000; // �� ���ִ� column �Ǵ� ����

	board[FindBoardHeight(choice)][choice] = stone; // ���� �ѹ� �ֺ���.
													

	int score;

	if (turn == 1) { // 1���� ���������� �� �� ��ȯ
		score = minmax ? FindScore(stone) : -FindScore(stone);
	}
	else { // 2�� �̻� ����������
		int temp = FindScore(stone);
		if (temp == 1000 || temp == -1000) { // critical�ϴ�? �ٷ� return
			board[FindBoardHeight(choice) - 1][choice] = ' ';
			return minmax ? temp + turn : -(temp + turn);
		}
		if (minmax) { // minmax�� ���� ��
			score = 1000 + level;
			for (int i = 0; i < BOARD_COLS; i++) {
				int temp = FindHeuristic(turn - 1, !minmax, i, (stone == 'O') ? 'X' : 'O', alpha, beta, level);
				if (temp != -2000) {
					score = min(score, temp);
					if (score <= alpha) {
						board[FindBoardHeight(choice) - 1][choice] = ' ';
						return score;
					}
					beta = min(beta, score);
				}
			}
		}
		else {
			score = -1000 - level; // ���� ���� ���� score�� ���� �� max�� ���Ƿ� ���� ���� ���� �ϴ� �ִ´�.
			for (int i = 0; i < BOARD_COLS; i++) {
				int temp = FindHeuristic(turn - 1, !minmax, i, (stone == 'O') ? 'X' : 'O', alpha, beta, level);
				if (temp != -2000) {
					score = max(score, temp);
					if (score >= beta) {
						board[FindBoardHeight(choice) - 1][choice] = ' ';
						return score;
					}
					alpha = max(alpha, score);
				}
			}
		}
	}

	board[FindBoardHeight(choice) - 1][choice] = ' '; // �Ʊ� �ֺô� �� ����
													  //printf("turn : %d column : %d, score = %d\n", turn, choice, score);
	return score; // ���� ����
}

int FindMaxChoice(int turn, char stone) { // �޸���ƽ�ϰ� ���� 7�� �������� �ְ����� ã�� �� col�� ��ȯ�ϴ� �Լ�
	int score[BOARD_COLS];
	for (int i = 0; i < BOARD_COLS; i++) {
		score[i] = FindHeuristic(turn, true, i, stone, -1000 - turn, 1000 + turn, turn); // �� ó���� AI�� �δ� max�̹Ƿ� true, turn�� stone�� parameter�̰� i�� for������ 0~6���� ������
		printf("Score of Column %d : %d\n", i + 1, score[i]);
	}

	int max = 0;
	for (int i = 0; i < BOARD_COLS; i++) { // �ְ����� ã��
		if (score[i] > score[max]) max = i;
		else if (score[i] == score[max]) {
			if (abs(3 - max) >= abs(3 - i)) max = i;
		}
	}

	return max; // column ����
}
int HintChoice(int turn, char stone) {
	int score[BOARD_COLS];
	for (int i = 0; i < BOARD_COLS; i++) {
		score[i] = FindHeuristic(turn, true, i, stone, -1000 - turn, 1000 + turn, turn); // �� ó���� AI�� �δ� max�̹Ƿ� true, turn�� stone�� parameter�̰� i�� for������ 0~6���� ������
	}

	int max = 0;
	for (int i = 0; i < BOARD_COLS; i++) { // �ְ����� ã��
		if (score[i] > score[max]) max = i;
		else if (score[i] == score[max]) {
			if (abs(3 - max) >= abs(3 - i)) max = i;
		}
	}

	return max; // column ����
}