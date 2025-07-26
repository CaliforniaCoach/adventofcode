INPUT = "357253-892942"

def group_len(pw, index):
    char = pw[index]
    count = 1
    for i in range(index + 1, len(pw)):
        if pw[i] != char:
            break
        count += 1
    for i in range(index - 1, -1, -1):
        if pw[i] != char:
            break
        count += 1
    return count
    


def test(pw, part2):
    x = str(pw)
    double = False
    for i in range(len(x) - 1):
        double = double or (group_len(x, i) == 2 if part2 else group_len(x, i) >= 2)
        if x[i + 1] < x[i]:
            return False
    return double

min, max = map(int, INPUT.split("-"))

count1 = 0
count2 = 0
for pw in range(min, max + 1):
    if test(pw, False):
        count1 += 1
    if test(pw, True):
        count2 += 1

print(count1, count2)
