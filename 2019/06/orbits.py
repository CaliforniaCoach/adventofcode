import copy

objects = {}
complete = set()

with open("input.txt") as reader:
    for line in reader.readlines():
        obj1, obj2 = line.strip().split(")")
        objects[obj1] = objects.get(obj1, set()) | {obj2}

objects_direct = copy.deepcopy(objects)

def get_orbit_objects_recursive(objects, center_object):
    if center_object in objects:
        orbit_objects = set(objects[center_object])
        if center_object not in complete:
            for orbit_object in objects[center_object]:
                if orbit_object not in complete:
                    orbit_objects.update(get_orbit_objects_recursive(objects, orbit_object))
                else:
                    orbit_objects.update(objects[orbit_object])
        return orbit_objects
    else:
        return set()

for center_object, orbit_objects in objects.items():
    orbit_objects.update(get_orbit_objects_recursive(objects, center_object))
    complete.add(center_object)

print(sum([len(x) for x in objects.values()]))

candidates = [x for x, y in objects.items() if "YOU" in y and "SAN" in y]
common_ancestor = min(candidates, key=lambda object: len(objects[object]))

def steps_recursive(start, target):
    if target in objects_direct[start]:
        return 0
    else:
        for direct_orbit_object in objects_direct[start]:
            if target in objects[direct_orbit_object]:
                return steps_recursive(direct_orbit_object, target) + 1

print("Common ancestor:", common_ancestor)

print(steps_recursive(common_ancestor, "SAN") + steps_recursive(common_ancestor, "YOU"))
