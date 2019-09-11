#include <vector>

using namespace std;

typedef struct {
	int pos[4][2];
} node; // node 구조체 pos에서 앞의 4는 4개짜리 square, 뒤의 2는 각각 x, y좌표

void removeNode(vector<node>* target, vector<node>* temp, int x, int y) {
	for (vector<node>::iterator iter = (*target).begin(); iter != (*target).end(); ) {
		for (int i = 0; i < 4; i++) { // pos의 4개 square를 도는 for문
			if ((*iter).pos[i][0] == x && (*iter).pos[i][1] == y) { // 개중에 하나라도 x, y가 모두 일치한다면
				node n;
				for (int p = 0; p < 4; p++) {
					for (int q = 0; q < 2; q++) {
						n.pos[p][q] = (*iter).pos[p][q]; // node를 만들어서 데이터를 저장해주고
					}
				}
				(*temp).push_back(n); // 저장한 node는 temp(쓰레기통)에 넣고
				iter = (*target).erase(iter); // win에서 삭제한다.
											  //printf("DELETE COMPLETED : %d %d\n", x, y);
				break;
			}
			if (i == 3) ++iter; // 4개 square를 다 돌았는데 없다면 다음 node를 본다.
		}
	}
}

void recoverNode(vector<node>* target, vector<node>* temp) {
	while (!(*temp).empty()) { // temp(쓰레기통이 빌 때까지)
		(*target).push_back((*temp).back()); // 쓰레기통에서 맨 뒷부분을 win에 넣고
		(*temp).pop_back(); // 넣은 node는 쓰레기통에서 빼준다.
	}
}