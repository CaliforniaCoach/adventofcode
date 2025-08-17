import sys
import re

rounds = int(sys.argv[2])
divide_by_3 = bool(sys.argv[3])

monkey = {}
monkey_items = {}
kgv = 1

def addition(old, x):
    return old + x

def multiplication(old, x):
    return old * x

def square(old, x):
    return old * old

with open(sys.argv[1], 'r') as reader:
    for line in reader:
        line = line.strip()
        word = line.split(' ')
        if word[0] == 'Monkey':
            monkey_no = int(word [1][0:-1])
            monkey[monkey_no] = [None, 0, 0, 0, 0]
        elif word[0] == 'Starting' and word[1] == 'items:':
            monkey_items[monkey_no] = [int(re.sub(',', '', item)) for item in word[2:]]
        elif word[0] == 'Operation:' and word[1] == "new" and word[2] == '=' and word[3] == 'old':
            if word[4] == '+':
                if word[5] == 'old':
                    monkey[monkey_no][0] = multiplication
                    monkey[monkey_no][1] = 2
                else:
                    monkey[monkey_no][0] = addition
                    monkey[monkey_no][1] = int(word[5])
            if word[4] == '*':
                if word[5] == 'old':
                    monkey[monkey_no][0] = square
                    monkey[monkey_no][1] = 99999
                else:
                    monkey[monkey_no][0] = multiplication
                    monkey[monkey_no][1] = int(word[5])
        elif line.startswith('Test: divisible by '):
            divisor = int(word[3])
            monkey[monkey_no][2] = divisor
            kgv *= divisor
        elif line.startswith('If true: throw to monkey '):
            monkey[monkey_no][3] = int(word[5])
        elif line.startswith('If false: throw to monkey '):
            monkey[monkey_no][4] = int(word[5])
        elif line != '':
            raise SyntaxError

print(monkey)
print(monkey_items)

monkey_inspections = [0] * len(monkey)

for round in range(rounds):
    for monkey_no, items in monkey_items.items():
        for worry_level in items:
            monkey_inspections[monkey_no] += 1
            func = monkey[monkey_no][0]
            arg = monkey[monkey_no][1]
            worry_level = func(worry_level, arg)
            if divide_by_3:
                worry_level //= 3
            worry_level %= kgv
            if worry_level % monkey[monkey_no][2] == 0:
                new_monkey = monkey[monkey_no][3]
            else:
                new_monkey = monkey[monkey_no][4]
            monkey_items[new_monkey].append(worry_level)
        monkey_items[monkey_no] = []

print(monkey_inspections)

monkey_inspections.sort()
print(monkey_inspections[-1] * monkey_inspections[-2])
