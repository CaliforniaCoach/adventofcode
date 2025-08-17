#include <iostream>
#include <limits>

using namespace std;


const int PLAYER_HP = 50;
const int PLAYER_MANA = 500;
const int BOSS_HP = 71;
const int BOSS_DAMAGE = 10;

/*
const int PLAYER_HP = 10;
const int PLAYER_MANA = 250;
const int BOSS_HP = 13;
const int BOSS_DAMAGE = 8;
*/


const int DRAIN_DAMAGE = 2;
const int SHIELD_ARMOR = 7;
const int POISON_DAMAGE = 3;
const int RECHARGE_MANA = 101;


struct state {
	int mana;
	int boss_hp;
	int player_hp;
	int shield_timer;
	int poison_timer;
	int recharge_timer;

	struct state& operator+=(const struct state& rhs) {
		mana += rhs.mana;
		boss_hp += rhs.boss_hp;
		player_hp += rhs.player_hp;
		shield_timer += rhs.shield_timer;
		poison_timer += rhs.poison_timer;
		recharge_timer += rhs.recharge_timer;
		return *this;
	}

	void decrease_timers() {
		if (shield_timer) shield_timer--;
		if (poison_timer) poison_timer--;
		if (recharge_timer) recharge_timer--;
	}

	void apply_timed_effects() {
		if (poison_timer) {
			boss_hp -= POISON_DAMAGE;
		}
		if (recharge_timer) {
			mana += RECHARGE_MANA;
		}
		decrease_timers();
	}
};


const struct state STATE_DELTA_SPELL[] = {
	{-53, -4, 0, 0, 0, 0},
	{-73, -2, +2, 0, 0, 0},
	{-113, 0, 0, 6, 0, 0},
	{-173, 0, 0, 0, 6, 0},
	{-229, 0, 0, 0, 0, 5},
};



int try_all_spells(struct state state, int total_mana_spent_so_far, int min_total_mana_spent, bool difficulty_hard);

int next_player_turn(struct state state, int spell, int total_mana_spent_so_far, int min_total_mana_spent,
					bool difficulty_hard) {
	// Player's turn
	if (difficulty_hard) {
		if (--state.player_hp <= 0) {
			return -1;
		}
	}
	state.apply_timed_effects();
	state += STATE_DELTA_SPELL[spell];
	total_mana_spent_so_far += -STATE_DELTA_SPELL[spell].mana;
	if (total_mana_spent_so_far >= min_total_mana_spent) {
		return -1;
	}
	if (state.mana < 0) {
		return -1;
	}
	if (state.boss_hp <= 0) {
		return min(min_total_mana_spent, total_mana_spent_so_far);
	}

	// Boss' turn
	state.apply_timed_effects();
	if (state.boss_hp <= 0) {
		return min(min_total_mana_spent, total_mana_spent_so_far);
	}
	state.player_hp -= max(BOSS_DAMAGE - (state.shield_timer ? SHIELD_ARMOR : 0), 1);
	if (state.player_hp <= 0) {
		return -1;
	}

	// Loop over all possible next player's turns
	return try_all_spells(state, total_mana_spent_so_far, min_total_mana_spent, difficulty_hard);
}

int try_all_spells(struct state state, int total_mana_spent_so_far, int min_total_mana_spent, bool difficulty_hard) {
	// Loop over all possible next player's turns, return "total mana spent" of best one
	int min_total_mana_spent_result = -1;
	for (int spell = 0; spell < size(STATE_DELTA_SPELL); spell++) {
		int retval = next_player_turn(state, spell, total_mana_spent_so_far, min_total_mana_spent, difficulty_hard);
		if (retval >= 0) {
			if (min_total_mana_spent_result < 0) {
				min_total_mana_spent_result = retval;
			} else {
				min_total_mana_spent_result = min(min_total_mana_spent_result, retval);
			}
		}
	}
	return min_total_mana_spent_result;
}

int main() {
	struct state initial_state = {PLAYER_MANA, BOSS_HP, PLAYER_HP, 0, 0, 0};
	cout << "Difficulty normal: " << try_all_spells(initial_state, 0, numeric_limits<int>::max(), false) << endl;
	cout << "Difficulty hard:   " << try_all_spells(initial_state, 0, numeric_limits<int>::max(), true) << endl;
}
