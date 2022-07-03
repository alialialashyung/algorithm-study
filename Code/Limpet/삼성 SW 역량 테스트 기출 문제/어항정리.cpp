#include <iostream>
#include <vector>
#include <queue>
#include <memory.h>

using namespace std;

#define MIN 0
#define MAX 2e9
#define EMPTY 0

struct Node
{
	int x, y;
};

inline int min(int& A, int& B) { return A < B ? A : B; }
inline int max(int& A, int& B) { return A > B ? A : B; }
const int dx[] = { 0, 0, -1, 1 };
const int dy[] = { 1, -1, 0, 0 };

const int MAX_N = 100 + 5;
int N, K, ans;
int map[MAX_N][MAX_N];

void input()
{
	scanf("%d %d", &N, &K);

	// 가장 왼쪽에 있는 어항부터 물고기 수 입력
	for (int i = 1; i <= N; i++)
	{
		scanf("%d", &map[N][i]);
	}
}

void addFish()
{
	vector<Node> vec;
	int minVal = MAX;
	for (int x = 1; x <= N; x++)
	{
		for (int y = 1; y <= N; y++)
		{
			if (map[x][y] == EMPTY)
				continue;

			// 최소값에 해당하는 어항 찾기
			if (map[x][y] < minVal)
			{
				minVal = map[x][y];
				vec.clear();
				vec.push_back({ x, y });
			}

			// 같은 최소값을 가지고 있는 어항인 경우
			else if (map[x][y] == minVal)
			{
				vec.push_back({ x, y });
			}
		}
	}

	for (int i = 0; i < vec.size(); ++i)
	{
		map[vec[i].x][vec[i].y]++;
	}
}

void roll()
{
	int pivot, w, h;
	pivot = w = h = 1;

	for (int idx = 0; ; ++idx)
	{
		if (pivot - 1 + w + h > N)
			break;

		for (int c = pivot; c < pivot + w; c++)
		{
			for (int r = N; r > N - h; r--)
			{
				int nextR = (N - w) + (c - pivot);
				int nextC = (pivot + w) + (N - r);

				map[nextR][nextC] = map[r][c];
				map[r][c] = EMPTY;
			}
		}

		pivot += (idx / 2 + 1);
		idx % 2 ? w++ : h++;
	}
}

void controlFish()
{
	int cMap[MAX_N][MAX_N] = { 0 };

	for (int x = 1; x <= N; x++)
	{
		for (int y = 1; y <= N; y++)
		{
			if (map[x][y] != EMPTY)
			{
				cMap[x][y] += map[x][y];

				for (int d = 0; d < 4; d++)
				{
					int nextX = x + dx[d];
					int nextY = y + dy[d];

					if (map[nextX][nextY] == EMPTY)
						continue;

					// 물고기가 많은 경우
					if (map[x][y] > map[nextX][nextY]) 
					{
						int diff = (map[x][y] - map[nextX][nextY]) / 5;
						cMap[x][y] -= diff;
						cMap[nextX][nextY] += diff;
					}
				}
			}
		}
	}

	memcpy(map, cMap, sizeof(cMap));
}

void sortFish()
{
	queue<int> que;
	for (int y = 1; y <= N; y++)
	{
		// 비어있는 열(col)인 경우 skip
		if (map[N][y] == EMPTY) continue;

		for (int x = N; x >= 1; --x)
		{
			// 중간에 비어져 있다면 break
			if (map[x][y] == EMPTY)
				break;

			// vector에 순서대로 저장해주고 map[][]에서 비워준다.
			que.push(map[x][y]);
			map[x][y] = EMPTY;
		}
	}

	int col = 1;
	while (!que.empty())
	{
		map[N][col++] = que.front();
		que.pop();
	}
}

void fold()
{
	int quarterN = N / 4;
	int sCol[4] = {0, N,  N - quarterN + 1, N};
	int cDir[4] = {0, -1, 1, -1};
	
	int srcY = 1;
	for (int i = 1; i <= 3; ++i)
	{
		int col = sCol[i];
		for (int j = 0; j < quarterN; ++j)
		{
			map[N - i][col] = map[N][srcY];
			map[N][srcY] = EMPTY;

			col += cDir[i]; // 해당 row 진행 방향으로
			srcY++;	// 옮겨지는 대상
		}
	}
}

int getDiff()
{
	int maxVal = MIN;
	int minVal = MAX;

	for (int x = 1; x <= N; x++)
	{
		for (int y = 1; y <= N; y++)
		{
			if (map[x][y] == EMPTY)
				continue;

			maxVal = max(maxVal, map[x][y]);
			minVal = min(minVal, map[x][y]);
		}
	}

	return (maxVal - minVal);
}

int main()
{
	// freopen("input.txt", "r", stdin);
	input();
	while (1)
	{
		addFish();           // 1. 물고기 추가

		roll();              // 2. 어항 쌓기
		controlFish();       // 3. 물고기 수 조절
		sortFish();          // 4. 물고기 정렬

		fold();              // 5. 어항 접기
		controlFish();       // 6. 물고기 수 조절
		sortFish();          // 7. 물고기 정렬

		ans++;               // 8. 정리 횟수
		if (getDiff() <= K) break;
	}
	printf("%d\n", ans);
}
