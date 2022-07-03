N, K = map(int, input().split())
fishes = list(map(int, input().split()))
graph = [[0] * N for _ in range(N)]  # N * N 짜리 어항
for i in range(len(graph[0])):
    graph[0][i] = fishes[i]


# 가장 물고기 수 적은 어항에 물고기 채워넣기
# 그리고 가장 왼쪽 어항을 오른쪽 위에 있는 어항의 위에 올려 놓기
def add_fish():
    min_fish = min(graph[0])
    for i in range(len(graph[0])):
        if min_fish == graph[0][i]:
            graph[0][i] += 1

    graph[1][1] = graph[0][0]
    graph[0][0] = 0


init_x = 1  # 어항 시작 x값
height = 2  # 어항 최대 높이


def rotate1():
    # 첫 시작 : (0,1) , (1,1)
    global init_x
    global height
    init_x = 1
    height = 2  # 돌릴 어항의 최대 높이. 처음엔 2다.
    xlen = 1  # 돌릴 어항의 가로 길이. 처음엔 1이다

    count = 1  # count = 2가 되면 어항 높이 증가
    max_rotate = N - 1  # 어항 오른쪽에 닿는 걸 판정할 것

    # 회전하는 어항 높이 1*2 -> 2*2 -> 2*3 -> 3 -> 3-> 4
    # 회전해서 올린다.
    while True:
        for y in range(height):
            for x in range(init_x, init_x + xlen):
                graph[xlen - x + init_x][y + init_x + xlen] = graph[y][x]
                graph[y][x] = 0

        count += 1

        init_x += xlen

        if count == 2:
            xlen += 1
            count = 0
        else:
            height += 1

        if max_rotate < init_x + height + xlen - 1:  # 더는 못돌린다.
            break


##  물고기 수 조절
def adjust():
    global height
    global init_x

    ds = [(-1, 0), (1, 0), (0, 1), (0, -1)]  # 인접한 것
    q = []

    # 차이 찾아보기
    for y in range(height):
        for x in range(init_x, N):
            for d in ds:
                ny = y + d[0]
                nx = x + d[1]
                # 맵 밖을 벗어나면
                if ny < 0 or nx < 0 or ny > N - 1 or nx > N - 1:
                    continue

                new = graph[ny][nx]
                origin = graph[y][x]

                # 어항수가 0 이면 안된다.
                if new == 0 or origin == 0:
                    continue

                if new >= origin:
                    d = (new - origin) // 5
                    if d > 0:
                        q.append([d, y, x])
                        q.append([-d, ny, nx])  # y,x 좌표에 d마리 추가

    # 조절 완료
    while q:
        d, y, x = q.pop()
        graph[y][x] += d


# 어항 일렬로 놓기
def array():
    global init_x
    global height
    def_x = 0
    for x in range(init_x, N):
        for y in range(height):
            if graph[y][x] == 0:
                break

            graph[0][def_x] = graph[y][x]
            if y != 0:
                graph[y][x] = 0
            def_x += 1
    init_x = 0


def rotate2():
    global init_x
    global height
    init_x = 0  # 첫 시작은 N/2이다.
    height = 1
    count = 0
    n = 2
    while True:
        if count == 2:
            break
        for y in range(height):
            for x in range(init_x, init_x + N // n):
                graph[height + (height - (y + 1))][N // n + x] = graph[y][(init_x + N // n - (x - init_x + 1))]
                graph[y][(init_x + N // n - (x - init_x + 1))] = 0

        height *= 2
        init_x += N // n
        n = n * 2
        count += 1


answer = 0
while True:
    add_fish()

    rotate1()

    adjust()
    array()
    rotate2()
    adjust()
    array()

    answer += 1
    min_val = min(graph[0])
    max_val = max(graph[0])
    if max_val - min_val <= K:
        break

print(answer)
