def blink():
    i = 0
    while i < len(stones):
        stone = stones[i]
        if stone == 0:
            stones[i] = 1
        elif len(str(stone)) % 2 == 0:
            str_stone = str(stone)
            stones[i] = int(str_stone[:len(str_stone) // 2])
            stones.insert(i + 1, int(str_stone[len(str_stone) // 2:]))
            i += 1
        else:
            stones[i] = stone * 2024
        i += 1

stones = []
with open("input.txt", encoding="utf-8") as reader:
    for line in reader.readlines():
        for tok in line.split():
            stones.append(int(tok))

for i in range(0, 10):
    blink()
    print(len(stones))
    print(stones)

print(len(stones))

for i in range(25, 75):
    blink()

print(len(stones))
