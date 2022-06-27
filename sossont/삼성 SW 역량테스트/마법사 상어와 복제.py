import heapq

# 1~8 번호의 방향 [x,y]
directions = [[0,-1],[-1,-1],[-1,0],[-1,1],[0,1],[1,1],[1,0],[1,-1]]
graph = [[[] for _ in range(4)] for _ in range(4)]
fishes = []
fish_smell = [[0 for _ in range(4)] for _ in range(4)]
# 상어의 이동 방법 사전 순 정렬
shark_moves = []
for x in range(1,5):
    for y in range(1,5):
        for z in range(1,5):
            shark_moves.append([x,y,z])

def check(x,y):
    return 0<=x<4 and 0<=y<4
# Input 부분
M, S = map(int,input().split())
for _ in range(M):
    x,y,d = map(int,input().split())
    graph[x-1][y-1].append(d-1)

shark_x, shark_y = map(int,input().split())
shark_x, shark_y = shark_x-1, shark_y-1


# 1. 복제 마법
# tmp에 물고기들을 복사해서 반환한다.
def copy():
    global graph
    tmp = [[[] for _ in range(4)] for _ in range(4)]
    for xx in range(4):
        for yy in range(4):
            if graph[xx][yy]:
                for fish in graph[xx][yy]:
                    tmp[xx][yy].append(fish)

    return tmp

# 2. 물고기 이동
def move_fish():
    global graph, shark_y, shark_x, fish_smell
    tmp = [[[] for _ in range(4)] for _ in range(4)]
    for xx in range(4):
        for yy in range(4):
            if graph[xx][yy]:
                for fish in graph[xx][yy]:
                    is_move = False
                    for d in range(8):
                        nd = (fish - d) % 8
                        nx = xx + directions[nd][0]
                        ny = yy + directions[nd][1]
                        # 격자 벗어나는 경우
                        if not check(nx,ny):
                            continue

                        # 상어가 있는 칸
                        if nx == shark_x and ny == shark_y:
                            continue

                        # 물고기의 냄새가 있는 칸
                        if fish_smell[nx][ny] != 0:
                            continue

                        # 그렇지 않은 경우 이동
                        tmp[nx][ny].append(nd)
                        is_move = True
                        break

                    if not is_move:
                        tmp[xx][yy].append(fish)

    for xx in range(4):
        for yy in range(4):
            graph[xx][yy] = tmp[xx][yy]


# 상 좌 하 우
delta = [[0],[-1,0], [0,-1],[1,0],[0,1]]

# 3. 상어가 3칸 이동
def move_shark():
    global shark_x, shark_y, graph
    q = []
    for m in range(len(shark_moves)):
        moves = shark_moves[m]
        ny = shark_y
        nx = shark_x
        isvisited = [[False for _ in range(4)] for _ in range(4)]
        eat = 0
        flag = True
        for move in moves:
            nx = nx + delta[move][0]
            ny = ny + delta[move][1]
            # 격자 범위를 벗어나는 경우
            if not check(nx,ny):
                flag = False
                break

            if isvisited[nx][ny]:
                continue

            isvisited[nx][ny] = True
            eat += len(graph[nx][ny])

        if flag:
            heapq.heappush(q, [-eat, m])
    _, case = heapq.heappop(q)

    for move in shark_moves[case]:
        shark_x = shark_x + delta[move][0]
        shark_y = shark_y + delta[move][1]
        if graph[shark_x][shark_y]:
            fish_smell[shark_x][shark_y] = 3
            graph[shark_x][shark_y].clear()


# 4. 냄새 사라진다
def remove_smell():
    global fish_smell
    for xx in range(4):
        for yy in range(4):
            if fish_smell[xx][yy] != 0:
                fish_smell[xx][yy] -= 1

# 5. 복제 마법 완료
def finish(tmp):
    global graph
    for xx in range(4):
        for yy in range(4):
            if tmp[xx][yy]:
                for fish in tmp[xx][yy]:
                    graph[xx][yy].append(fish)

for _ in range(S):
    cop = copy()
    move_fish()
    move_shark()
    remove_smell()
    finish(cop)

answer = 0
for xx in range(4):
    for yy in range(4):
        if graph[xx][yy]:
            answer += len(graph[xx][yy])

print(answer)