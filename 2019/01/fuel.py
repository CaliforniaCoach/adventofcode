total_fuel = 0
total_fuel2 = 0
with open("input.txt") as reader:
    for line in reader.readlines():
        new_fuel = int(line.strip()) // 3 - 2
        total_fuel += new_fuel
        while new_fuel > 0:
            total_fuel2 += new_fuel
            new_fuel = new_fuel // 3 - 2

print(total_fuel)
print(total_fuel2)
