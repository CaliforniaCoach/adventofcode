WIDTH = 25
HEIGHT = 6

layers = []
layer = []
row = []
with open("input.txt") as reader:
    for digit in reader.readline().strip():
        row.append(int(digit))
        if len(row) >= WIDTH:
            layer.append(row)
            row = []
            if len(layer) >= HEIGHT:
                layers.append(layer)
                layer = []

layer_fewest0 = min(layers, key=lambda layer: sum((row.count(0) for row in layer)))
print(sum(row.count(1) for row in layer_fewest0) * sum(row.count(2) for row in layer_fewest0))

result = layers[0]
for layer in layers[1:]:
    for irow in range(len(layer)):
        row = layer[irow]
        for ipix in range(len(row)):
            result[irow][ipix] = result[irow][ipix] if result[irow][ipix] != 2 else row[ipix]

for row in result:
    for pix in row:
        print("**" if pix == 1 else "  ", end="")

    print()
