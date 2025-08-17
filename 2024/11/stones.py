
from functools import cache

@cache
def get_num_stones_after(start_stone, blinks):
    if blinks == 0:
        return 1
    stones = [start_stone]
    str_stone = str(stones[0])
    if stones[0] == 0:
        stones[0] = 1
    elif len(str_stone) % 2 == 0:
        stones[0] = int(str_stone[:len(str_stone) // 2])
        stones.append(int(str_stone[len(str_stone) // 2:]))
    else:
        stones[0] *= 2024

    return sum(get_num_stones_after(stone, blinks - 1) for stone in stones)



stones = []
with open("input.txt", encoding="utf-8") as reader:
    for line in reader.readlines():
        for tok in line.split():
            stones.append(int(tok))

print(f"Stones after 25 blinks: {sum(get_num_stones_after(stone, 25) for stone in stones)}")
print(f"Stones after 75 blinks: {sum(get_num_stones_after(stone, 75) for stone in stones)}")
