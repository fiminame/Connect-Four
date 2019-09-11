#include <vector>

using namespace std;

typedef struct {
	int pos[4][2];
} node; // node ����ü pos���� ���� 4�� 4��¥�� square, ���� 2�� ���� x, y��ǥ

void removeNode(vector<node>* target, vector<node>* temp, int x, int y) {
	for (vector<node>::iterator iter = (*target).begin(); iter != (*target).end(); ) {
		for (int i = 0; i < 4; i++) { // pos�� 4�� square�� ���� for��
			if ((*iter).pos[i][0] == x && (*iter).pos[i][1] == y) { // ���߿� �ϳ��� x, y�� ��� ��ġ�Ѵٸ�
				node n;
				for (int p = 0; p < 4; p++) {
					for (int q = 0; q < 2; q++) {
						n.pos[p][q] = (*iter).pos[p][q]; // node�� ���� �����͸� �������ְ�
					}
				}
				(*temp).push_back(n); // ������ node�� temp(��������)�� �ְ�
				iter = (*target).erase(iter); // win���� �����Ѵ�.
											  //printf("DELETE COMPLETED : %d %d\n", x, y);
				break;
			}
			if (i == 3) ++iter; // 4�� square�� �� ���Ҵµ� ���ٸ� ���� node�� ����.
		}
	}
}

void recoverNode(vector<node>* target, vector<node>* temp) {
	while (!(*temp).empty()) { // temp(���������� �� ������)
		(*target).push_back((*temp).back()); // �������뿡�� �� �޺κ��� win�� �ְ�
		(*temp).pop_back(); // ���� node�� �������뿡�� ���ش�.
	}
}