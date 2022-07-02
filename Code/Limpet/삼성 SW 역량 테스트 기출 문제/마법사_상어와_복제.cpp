#include<iostream>
#include<vector>
#include<queue>
using namespace std;

int board[9][4][4]; //0은 해당 칸 총 물고기 수, 나머지는 방향 별 물고기 수 x, y
int M, S;
int sx, sy;
int smell[4][4]; // -1은 냄새 x, 0이상은 냄새가 생긴 턴
int dir[8][2] = {
	{0,-1},{-1,-1},{-1,0}, {-1,1},{0,1}, {1,1}, {1,0}, {1,-1}
};//x, y

struct Node {
	int fish, mov;
	int x, y;
	vector<vector<bool>>isvisited; //본래 마지막 움직임을 기억하려 했으나 상하하로 위아래 물고기 먹을 수도 있음
};


int main(void) {

	int sum = 0;
	cin >> M >> S;

	for (int i = 0; i < 9; i++)
		for (int j = 4; j < 4; j++)
			fill(board[i][j], board[i][j] + 4, 0);

	for (int i = 0; i < 4; i++)
		fill(smell[i], smell[i] + 4, -100);

	for (int i = 0; i < M; i++) {
		int x, y, d;
		cin >> x >> y >> d;
		board[d][x-1][y-1]++;
		board[0][x-1][y-1]++;
	}

	cin >> sx >> sy;
	sx--;
	sy--;
	for(int s = 1; s<=S; s++) {
		// 복제
		int movement[9][4][4];
		for (int i = 0; i < 9; i++)
			for (int j = 0; j < 4; j++)
				fill(movement[i][j], movement[i][j] + 4, 0);

		//물고기 이동
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (board[0][i][j] == 0) continue; //해당 칸에 물고기 x
				
				//가능한 방향 탐색
			
				vector<int> pos(8, -1);
				bool isvalid = false;
				int pidx = -1;
				for (int r = 0; r < 8; r++) {
					int nextx = i + dir[r][0], nexty = j + dir[r][1];
					if (nextx < 0 || nextx >3 || nexty < 0 || nexty >3) continue;
					if (nextx == sx && nexty == sy) continue;
					if (smell[nextx][nexty] >= s - 2) continue;
					isvalid = true;
					pos[r] = r + 1;
				}
				if (!isvalid) {
					for (int r = 0; r < 9; r++)
						movement[r][i][j] += board[r][i][j];
					continue;
				}
				for (int r = 0; r < 7; r++) {
					if (pos[r] == -1) continue;
					if (pos[r + 1] == -1) pos[r + 1] = pos[r];
				}
				
				for (int r = 0; r <8; r++) {
					if (pos[r] == -1) pos[r] = pos[7];
					else break;
				}
				

				for (int r = 1; r <9; r++) {
					movement[pos[r-1]][i + dir[pos[r-1] - 1 ][0]][j + dir[pos[r-1]-1][1]] += board[r][i][j];
					movement[0][i + dir[pos[r-1] -1][0]][j + dir[pos[r-1]-1][1]] += board[r][i][j];					
				}
			}
		}
		
		//상어 이동
		queue<Node> q;
		Node start;
		start.fish = 0;
		start.mov = 0;
		start.y = sy;
		start.x = sx;
	
		vector<vector<bool>> v(4,vector<bool>(4,false));
		start.isvisited = v;

		q.push(start);
		int maxfish = -1, newy = -1, newx = -1;

		while (!q.empty()) {
			Node now = q.front(); q.pop();
            //처음 시작 위치에서 물고기를 먹고 시작하지 않음!!!!!!!!!!!!!!!!!!!!!!!
			if(!now.isvisited[now.x][now.y] && now.mov !=0)
				now.fish += movement[0][now.x][now.y];
			if(now.mov !=0)
				now.isvisited[now.x][now.y] = true;
			
			if (now.mov >= 3){ 
				if (maxfish < now.fish) {
					maxfish = now.fish;
						newy = now.y;
						newx = now.x;
						v = now.isvisited;
				}
				continue;
			}
			now.mov++;


			if (now.x > 0  ) {
				
				now.x--;
	
				q.push(now);
				now.x++;
			}
			if (now.y > 0 ) {
				
				now.y--;
				
				q.push(now);
			
				now.y++;
			}
			if (now.x < 3  ) {
			
				now.x++;
				
				q.push(now);
			
				now.x--;
			}
			if (now.y < 3) {
				
				now.y++;
			
				q.push(now);
			
				now.y--;
			}
			
		}
		sy = newy; sx = newx;
		//cout << sx << " " << sy << endl;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (v[i][j]) {
					if (movement[0][i][j] == 0) continue;
					for (int r = 0; r < 9; r++)movement[r][i][j] = 0;
					smell[i][j] = s;
				}
			}
		}
		// 물고기 복제
		for (int r = 0; r < 9; r++) {
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					board[r][i][j] += movement[r][i][j];
				}
			}
		}

	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			sum += board[0][i][j];
		}
	}
	cout << sum << endl;

	return 0;
	
}
