import sys
import functools


packets = [[]]

def compare(element1, element2):
    if isinstance(element1, int):
        if isinstance(element2, int):
            if element1 < element2:
                return 1
            elif element1 > element2:
                return -1
            else:
                return 0
        elif isinstance(element2, list):
            return compare([element1], element2)
        else:
            raise ValueError('Second value is neither int nor list')
    elif isinstance(element1, list):
        if isinstance(element2, list):
            for i in range(min(len(element1), len(element2))):
                retval = compare(element1[i], element2[i])
                if retval <= -1:
                    return -1
                elif retval >= 1:
                    return 1
            if len(element1) < len(element2):
                return 1
            elif len(element1) > len(element2):
                return -1
            else:
                return 0
        elif isinstance(element2, int):
            return compare(element1, [element2])
        else:
            raise ValueError('Second value is neither int nor list')
    else:
        raise ValueError('First value is neither int nor list')


def main(file) -> int:

    with open(file, encoding='utf-8') as reader:
        for line in reader:
            line = line.strip()
            if line == '':
                packets.append([])
            else:
                packets[-1].append(eval(line))

    sum = 0
    xs = [[2],[6]]
    for i, packet_pair in enumerate(packets):
        if compare(packet_pair[0], packet_pair[1]) > 0:
            sum += i + 1
#            if len(xs) == 0:
#                xs.append(packet_pair[0])
#            for idx in range(0, len(xs)):
#                if compare(xs[idx], packet_pair[0]) > 0:
#                    xs.insert(0, packet_pair[0])
#                else:
#                    xs.append(packet_pair[0])
        else:
            pass

    print(sum)

    flatlist = [item for sublist in packets for item in sublist]
    flatlist.append([2])
    flatlist.append([6])
    flatlist.sort(key=functools.cmp_to_key(compare), reverse=True)
    print((flatlist.index([2]) + 1) * (flatlist.index([6]) + 1))

    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1]))
