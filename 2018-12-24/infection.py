import re
import functools
import copy
from operator import attrgetter


FILENAME = 'input.txt'

armies = {}

@functools.total_ordering
class Group:
	def __init__(self, units, hitpoints, weaknesses, immunities, attackDamage, attackType,
			  initiative):
		self.units = units
		self.hitpoints = hitpoints
		self.weaknesses = weaknesses
		self.immunities = immunities
		self.attackDamage = attackDamage
		self.attackType = attackType
		self.initiative = initiative

		self.attacks = None
		self.attackedBy = None

	def effectivePower(self):
		return self.units * self.attackDamage
	
	def parentArmyName(self, armies):
		for (name, army) in armies.items():
			if self in army:
				return name
		return None
	
	def damageDealtTo(self, defender):
		if self.attackType in defender.immunities:
			return 0
		elif self.attackType in defender.weaknesses:
			return max(0, 2 * self.effectivePower())
		else:
			return max(0, self.effectivePower())

	def __eq__(self, other):
		return (self.effectivePower() == other.effectivePower()) and (self.initiative ==
															  other.initiative)

	def __gt__(self, other):
		if self.effectivePower() > other.effectivePower():
			return True
		elif self.effectivePower() < other.effectivePower():
			return False
		else:
			return self.initiative > other.initiative
		
	def __repr__(self):
		# return f'Group({self.__dict__})'
		return f'Group({self.units=})'


def fight(initialArmies, boost):
	armies = copy.deepcopy(initialArmies)
	for group in armies['Immune System']:
		group.attackDamage += boost

	while len(armies) > 1:
		allGroups = sum(armies.values(), [])

		# Target selection phase
		for group in sorted(allGroups, reverse=True):
			selectedDefendingGroup, attackDamage = (None, 0)
			for defendingGroup in allGroups:
				if defendingGroup is group:
					continue
				if defendingGroup.attackedBy is not None:
					continue
				if defendingGroup.parentArmyName(armies) == group.parentArmyName(armies):
					continue
				damageDealt = group.damageDealtTo(defendingGroup)
				if damageDealt <= 0:
					continue
				if selectedDefendingGroup:
					if damageDealt < attackDamage:
						continue
					elif damageDealt == attackDamage:
						if defendingGroup.effectivePower() < selectedDefendingGroup.effectivePower():
							continue
						elif defendingGroup.effectivePower() == selectedDefendingGroup.effectivePower():
							if defendingGroup.initiative < selectedDefendingGroup.initiative:
								continue
				selectedDefendingGroup = defendingGroup
				attackDamage = damageDealt
			if selectedDefendingGroup:
				group.attacks = selectedDefendingGroup
				selectedDefendingGroup.attackedBy = group

		# Attacking phase
		totalUnitsKilled = 0
		for group in sorted(allGroups, key=attrgetter('initiative'), reverse=True):
			defendingGroup = group.attacks
			if defendingGroup:
				damage = group.damageDealtTo(defendingGroup)
				unitsKilled = damage // defendingGroup.hitpoints
				assert unitsKilled >= 0
				defendingGroup.units -= unitsKilled
				totalUnitsKilled += unitsKilled
				if defendingGroup.units <= 0:
					army = defendingGroup.parentArmyName(armies)
					armies[army].remove(defendingGroup)
					if not armies[army]:
						del armies[army]
				group.attacks = None
				defendingGroup.attackedBy = None

		# print(list(armies.keys())[0], list(armies.values())[0][0].units)
		# print(list(armies.keys())[0], list(armies.values())[0][1].units)
		# print(list(armies.keys())[1], list(armies.values())[1][0].units)
		# print(list(armies.keys())[1], list(armies.values())[1][1].units)
		# print(armies)
		# print()

		if totalUnitsKilled == 0:
			break

	return {army: sum(group.units for group in groups) for army, groups in armies.items()}


with open(FILENAME, 'r') as reader:
	for line in reader:
		line = line.strip()
		if line in ('Immune System:', 'Infection:'):
			currentArmyName = line[:-1]
			currentArmy = []
			armies[currentArmyName] = currentArmy
		elif len(line) == 0:
			pass
		else:
			match = re.fullmatch(r'(\d+) units each with (\d+) hit points (\(.+\) )?with an attack that does (\d+) (.+) damage at initiative (\d+)', line)
			weaknesses = []
			immunities = []
			if match.group(3):
				weaknessesAndImmunities = match.group(3).strip()[1:-1].split('; ')
				for entry in weaknessesAndImmunities:
					if entry.startswith('weak to '):
						for weakness in entry[len('weak to '):].split(', '):
							weaknesses.append(weakness)
					if entry.startswith('immune to '):
						for immunity in entry[len('immune to '):].split(', '):
							immunities.append(immunity)
			currentArmy.append(Group(int(match.group(1)), int(match.group(2)), weaknesses,
							immunities, int(match.group(4)), match.group(5), int(match.group(6))))


print('Without boost (solution of part 1): ', fight(armies, 0))

boost = 1
while True:
	result = fight(armies, boost)
	print(f'With boost {boost}: {result}')
	if len(result) == 1 and list(result.keys())[0] != 'Infection':
		break
	boost += 1
