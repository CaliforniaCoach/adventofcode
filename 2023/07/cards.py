import functools


def type(hand):
	hist = {}
	for c in hand:
		if c in hist:
			hist[c] += 1
		else:
			hist[c] = 1
	
	if len(hist) == 1:
		return 7
	
	if max(hist.values()) == 4:
		return 6
	
	if max(hist.values()) == 3 and min(hist.values()) == 2:
		return 5
	
	if max(hist.values()) == 3:
		return 4
	
	if max(hist.values()) == 2 and len(hist) == 3:
		return 3
	
	if max(hist.values()) == 2 and len(hist) >= 4:
		return 2
	
	if len(hist) == 5:
		return 1
	
	raise


def type2(hand):
	hist = {}
	for c in hand:
		if c in hist:
			hist[c] += 1
		else:
			hist[c] = 1

	jokers = hist.pop('J') if 'J' in hist else 0
	
	if len(hist) == 0 or max(hist.values()) + jokers == 5:
		return 7
	
	if max(hist.values()) + jokers == 4:
		return 6
	
	if max(hist.values()) + jokers == 3 and min(hist.values()) == 2:
		return 5
	
	if max(hist.values()) + jokers == 3:
		return 4
	
	if max(hist.values()) == 2 and len(hist) == 3:
		return 3
	
	if max(hist.values()) + jokers == 2:
		return 2
	
	if len(hist) == 5:
		return 1
	
	raise


def card_value(card, j_value):
	val = {'A': 14, 'K': 13, 'Q': 12, 'J': j_value, 'T': 10}
	return val[card] if card in val else int(card)



def compare(entry1, entry2, type_func, j_value):
	hand1 = entry1[0]
	hand2 = entry2[0]

	if type_func(hand1) > type_func(hand2):
		return 1
	
	if type_func(hand1) < type_func(hand2):
		return -1
	
	for i in range(len(hand1)):
		if card_value(hand1[i], j_value) > card_value(hand2[i], j_value):
			return 1
		if card_value(hand1[i], j_value) < card_value(hand2[i], j_value):
			return -1
		
	raise


def compare1(entry1, entry2):
	return compare(entry1, entry2, type, 11)


def compare2(entry1, entry2):
	return compare(entry1, entry2, type2, 1)


hands = []
with open('input.txt') as reader:
	for line in reader.readlines():
		hands.append(line.split())

hands.sort(key=functools.cmp_to_key(compare1))

total = 0
for i, hand in enumerate(hands):
	total += (i + 1) * int(hand[1])

print(total)


hands.sort(key=functools.cmp_to_key(compare2))

total = 0
for i, hand in enumerate(hands):
	total += (i + 1) * int(hand[1])

print(hands)
print(total)
