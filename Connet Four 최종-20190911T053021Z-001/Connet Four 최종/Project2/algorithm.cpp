#include <stdio.h>
#include <stdlib.h>
#include <time.h> // 임시적으로 만든 함수
#include <windows.h>
#include "Structure.h" // vector include 선언, node 구조체 정보, removeNode, recoverNode 함수

using namespace std; // vector 쓰려면 필요한 부분

vector<node> OWin;   // O 승리 그룹들 vector
vector<node> XWin;   // X 승리 그룹들 vector
vector<node> OTemp;   // O 쓰레기통 그룹들 vector
vector<node> XTemp;   // X 쓰레기통 그룹들 vector

#define BOARD_ROWS 6
#define BOARD_COLS 7

extern int checkFirst(int);
void FindCurrentBoardStatus(); // 현재 보드 상태에 맞춰 OWin, XWin, OTemp, XTemp를 변경하는 함수
int FindScore(char);
int FindBoardHeight(int);
int FindHeuristic(int, bool, int, char, int, int, int);
int FindMaxChoice(int, char);
extern char board[][BOARD_COLS];

int FindNextMove(int level) {
	char stone = (checkFirst(0) == 0) ? 'X' : 'O'; // checkFirst값이 0이면 Player First, 1이면 AI First
	FindCurrentBoardStatus();
	int column;

	time_t startTime = clock();
	column = FindMaxChoice(level, stone);
	time_t endTime = clock();
	printf("Execution Time : %.3lf", (float)(endTime - startTime) / (CLOCKS_PER_SEC));
	srand(time(NULL));

	return column;
}

void init() { // 코드 시작시에 init 함수가 1번 실행, 모든 가능한 Square의 Group을 벡터에 생성
	for (int i = 0; i < BOARD_ROWS; i++) {
		for (int j = 0; j < BOARD_COLS - 3; j++) {
			int num[4][2] = { { i, j },{ i, j + 1 },{ i, j + 2 },{ i, j + 3 } }; // 가로
			node n;
			for (int p = 0; p < 4; p++) {
				for (int q = 0; q < 2; q++) {
					n.pos[p][q] = num[p][q]; // node n을 만들어서 모든 pos에 값을 넣고
				}
			}
			OWin.push_back(n); // OWin에 push
			XWin.push_back(n); // XWin에 push
		}
	}
	for (int i = 0; i < BOARD_ROWS - 3; i++) {
		for (int j = 0; j < BOARD_COLS; j++) {
			int num[4][2] = { { i, j },{ i + 1, j },{ i + 2, j },{ i + 3, j } }; // 세로
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
			int num[4][2] = { { i, j },{ i + 1, j + 1 },{ i + 2, j + 2 },{ i + 3, j + 3 } }; // 45도 대각선
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
			int num[4][2] = { { i, j },{ i - 1, j + 1 },{ i - 2, j + 2 },{ i - 3, j + 3 } }; // 135도 대각선
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
	recoverNode(&OWin, &OTemp); // OWin 원상복구
	recoverNode(&XWin, &XTemp); // XWin 원상복구
	for (int i = 0; i < BOARD_ROWS; i++) {
		for (int j = 0; j < BOARD_COLS; j++) { // 보드를 돌면서
			if (board[i][j] == 'X') removeNode(&OWin, &OTemp, i, j); // x가 좌표 (i, j)에 있다? OWin에서 node 제거
			else if (board[i][j] == 'O') removeNode(&XWin, &XTemp, i, j); // 반대
		}
	}

	//printf("O left moving : %d\nX left moving : %d\n", OWin.size(), XWin.size());
}
int FindScore(char stone) { // 평가 함수
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
int FindBoardHeight(int col) { // parameter col의 현재 최고점을 찾는다.
	int row;
	for (row = 0; row < BOARD_ROWS; row++) {
		if (board[row][col] == ' ') {
			return row;
		}
	}
	return row;
}
int FindHeuristic(int turn, bool minmax, int choice, char stone, int alpha, int beta, int level) { // turn n에서 휴리스틱하게 점수 판단해서 값 반환
	if (FindBoardHeight(choice) == 6) return -2000; // 꽉 차있는 column 판단 점수

	board[FindBoardHeight(choice)][choice] = stone; // 돌을 한번 둬보자.
													

	int score;

	if (turn == 1) { // 1턴이 남아있으면 평가 후 반환
		score = minmax ? FindScore(stone) : -FindScore(stone);
	}
	else { // 2턴 이상 남아있으면
		int temp = FindScore(stone);
		if (temp == 1000 || temp == -1000) { // critical하다? 바로 return
			board[FindBoardHeight(choice) - 1][choice] = ' ';
			return minmax ? temp + turn : -(temp + turn);
		}
		if (minmax) { // minmax를 따진 후
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
			score = -1000 - level; // 가장 높은 값을 score에 넣을 때 max를 쓰므로 가장 작을 값을 일단 넣는다.
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

	board[FindBoardHeight(choice) - 1][choice] = ' '; // 아까 둬봤던 돌 제거
													  //printf("turn : %d column : %d, score = %d\n", turn, choice, score);
	return score; // 점수 리턴
}

int FindMaxChoice(int turn, char stone) { // 휴리스틱하게 구한 7개 점수에서 최고점을 찾아 그 col을 반환하는 함수
	int score[BOARD_COLS];
	for (int i = 0; i < BOARD_COLS; i++) {
		score[i] = FindHeuristic(turn, true, i, stone, -1000 - turn, 1000 + turn, turn); // 맨 처음은 AI가 두는 max이므로 true, turn과 stone은 parameter이고 i는 for문으로 0~6까지 돌려봄
		printf("Score of Column %d : %d\n", i + 1, score[i]);
	}

	int max = 0;
	for (int i = 0; i < BOARD_COLS; i++) { // 최고점수 찾기
		if (score[i] > score[max]) max = i;
		else if (score[i] == score[max]) {
			if (abs(3 - max) >= abs(3 - i)) max = i;
		}
	}

	return max; // column 리턴
}
int HintChoice(int turn, char stone) {
	int score[BOARD_COLS];
	for (int i = 0; i < BOARD_COLS; i++) {
		score[i] = FindHeuristic(turn, true, i, stone, -1000 - turn, 1000 + turn, turn); // 맨 처음은 AI가 두는 max이므로 true, turn과 stone은 parameter이고 i는 for문으로 0~6까지 돌려봄
	}

	int max = 0;
	for (int i = 0; i < BOARD_COLS; i++) { // 최고점수 찾기
		if (score[i] > score[max]) max = i;
		else if (score[i] == score[max]) {
			if (abs(3 - max) >= abs(3 - i)) max = i;
		}
	}

	return max; // column 리턴
}