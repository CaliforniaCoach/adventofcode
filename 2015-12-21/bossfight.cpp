#include <iostream>
#include <limits>

using namespace std;


const int BOSS_HP = 100;
const int BOSS_DAMAGE = 8;
const int BOSS_ARMOR = 2;

const int PLAYER_HP = 100;



struct equipment {
	int weapon;  // mandatory
	int armor;   // optional
	int ring1;   // optional
	int ring2;   // optional
};

struct stats {
	int hp;
	int damage;
	int armor;
};

struct item {
	int cost;
	int damage;
	int armor;
};

struct cost {
	int min_cost_to_win;
	int max_cost_while_still_losing;
};

const struct item WEAPONS[] = {
	{8, 4, 0},
	{10, 5, 0},
	{25, 6, 0},
	{40, 7, 0},
	{74, 8, 0}
};

const struct item ARMORS[] = {
	{13, 0, 1},
	{31, 0, 2},
	{53, 0, 3},
	{75, 0, 4},
	{102, 0, 5}
};

const struct item RINGS[] = {
	{25, 1, 0},
	{50, 2, 0},
	{100, 3, 0},
	{20, 0, 1},
	{40, 0, 2},
	{80, 0, 3}
};

const struct stats boss_stats = {BOSS_HP, BOSS_DAMAGE, BOSS_ARMOR};


int hp_loss(int attacker_damage, int defender_armor) {
	int loss = attacker_damage - defender_armor;
	return loss >= 1 ? loss : 1;
}

int ceil_div(int divisor, int dividend) {
	int quotient = divisor / dividend;
	return quotient + (divisor % dividend ? 1 : 0);
}

int calc_required_damage(int player_armor) {
	int hp_loss_per_turn_player = hp_loss(BOSS_DAMAGE, player_armor);
	int turns_til_player_dead = (PLAYER_HP - 1) / hp_loss_per_turn_player + 1;
	int min_hp_loss_per_turn_boss = (BOSS_HP - 1) / turns_til_player_dead + 1;
	return min_hp_loss_per_turn_boss > 1 ? min_hp_loss_per_turn_boss + BOSS_ARMOR : 1;
}

struct stats calc_stats(struct equipment equip) {
	struct stats stats = {0, 0, 0};
	if (equip.weapon >= 0 && equip.weapon < (int) size(WEAPONS)) {
		stats.damage += WEAPONS[equip.weapon].damage;
	}
	if (equip.armor >= 0 && equip.armor < (int) size(ARMORS)) {
		stats.armor += ARMORS[equip.armor].armor;
	}
	if (equip.ring1 >= 0 && equip.ring1 < (int) size(RINGS)) {
		stats.damage += RINGS[equip.ring1].damage;
		stats.armor += RINGS[equip.ring1].armor;
	}
	if (equip.ring2 >= 0 && equip.ring2 < (int) size(RINGS)) {
		stats.damage += RINGS[equip.ring2].damage;
		stats.armor += RINGS[equip.ring2].armor;
	}
	return stats;
}

int calc_cost(struct equipment equip) {
	int cost = 0;
	if (equip.weapon >= 0 && equip.weapon < (int) size(WEAPONS)) {
		cost += WEAPONS[equip.weapon].cost;
	}
	if (equip.armor >= 0 && equip.armor < (int) size(ARMORS)) {
		cost += ARMORS[equip.armor].cost;
	}
	if (equip.ring1 >= 0 && equip.ring1 < (int) size(RINGS)) {
		cost += RINGS[equip.ring1].cost;
	}
	if (equip.ring2 >= 0 && equip.ring2 < (int) size(RINGS)) {
		cost += RINGS[equip.ring2].cost;
	}
	return cost;
}

int calc_required_weapon_index(struct equipment equip) {
	equip.weapon = -1;
	struct stats player_stats = calc_stats(equip);
	int required_total_damage = calc_required_damage(player_stats.armor);
	int required_weapon_damage = required_total_damage - player_stats.damage;
	for (int i = 0; i < (int) size(WEAPONS); i++) {
		struct item weapon = WEAPONS[i];
		if (weapon.damage >= required_weapon_damage) {
			return i;
		}
	}
	return -1;
}

void recalc_min_max_cost(struct cost& min_max_cost, struct equipment equip) {
	equip.weapon = -1;
	int required_weapon_index = calc_required_weapon_index(equip);
	int cost_without_weapon = calc_cost(equip);
	struct cost new_cost = min_max_cost;
	if (required_weapon_index < 0) {
		// We will always lose, even with most expensive weapon
		new_cost.max_cost_while_still_losing = cost_without_weapon + WEAPONS[size(WEAPONS)].cost;
	} else if (required_weapon_index == 0) {
		// We will always win, even with cheapest weapon
		new_cost.min_cost_to_win = cost_without_weapon + WEAPONS[0].cost;
	} else {
		// We will win with calculated weapon, but lose with one below calculated weapon
		new_cost.min_cost_to_win = cost_without_weapon + WEAPONS[required_weapon_index].cost;
		new_cost.max_cost_while_still_losing = cost_without_weapon +
			WEAPONS[required_weapon_index - 1].cost;
	}
	if (new_cost.min_cost_to_win < min_max_cost.min_cost_to_win) {
		min_max_cost.min_cost_to_win = new_cost.min_cost_to_win;
	}
	if (new_cost.max_cost_while_still_losing > min_max_cost.max_cost_while_still_losing) {
		min_max_cost.max_cost_while_still_losing = new_cost.max_cost_while_still_losing;
	}
}

int main() {
	struct equipment equip;
	struct cost min_max_cost = {numeric_limits<int>::max(), 0};
	for (int armor = -1; armor < (int) size(ARMORS); armor++) {
		for (int ring1 = -1; ring1 < (int) size(RINGS); ring1++) {
			// No 2nd ring
			recalc_min_max_cost(min_max_cost, {-1, armor, ring1, -1});
			if (ring1 >= 0) {
				// Buy 2nd ring
				for (int ring2 = ring1 + 1; ring2 < (int) size(RINGS); ring2++) {
					recalc_min_max_cost(min_max_cost, {-1, armor, ring1, ring2});
				}
			}
		}
	}
	cout << "Min cost to win:  " << min_max_cost.min_cost_to_win << endl;
	cout << "Max cost to lose: " << min_max_cost.max_cost_while_still_losing << endl;
}
