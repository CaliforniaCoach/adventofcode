#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>


#define FILENAME "input.txt"

#define MAX_CARDS 50


int play(int cards[][MAX_CARDS], int num_cards[], int recursive_mode);

int calc_winner(int cards[][MAX_CARDS], int num_cards[], int recursive_mode) {
	if (!recursive_mode || cards[0][0] >= num_cards[0] || cards[1][0] >= num_cards[1]) {
		return cards[0][0] > cards[1][0] ? 0 : 1;
	} else {
		int sub_cards[2][MAX_CARDS];
		int num_sub_cards[2] = {cards[0][0], cards[1][0]};
		for (int player = 0; player < 2; player++) {
			for (int i = 0; i < num_sub_cards[player]; i++) {
				sub_cards[player][i] = cards[player][i + 1];
			}
		}
		return play(sub_cards, num_sub_cards, recursive_mode);
	}
}

void move_cards(int* cards_winner, int* num_cards_winner, int* cards_loser, int* num_cards_loser) {
	int temp = cards_winner[0];
	for (int i = 0; i < *num_cards_winner - 1; i++) {
		cards_winner[i] = cards_winner[i + 1];
	}
	cards_winner[*num_cards_winner - 1] = temp;
	cards_winner[*num_cards_winner] = cards_loser[0];
	(*num_cards_winner)++;

	for (int i = 0; i < *num_cards_loser - 1; i++) {
		cards_loser[i] = cards_loser[i + 1];
	}
	(*num_cards_loser)--;
}

#define COMBI_BUFFER_SIZE 10000

int play(int cards[][MAX_CARDS], int num_cards[], int recursive_mode) {
	int* card_combinations_met = malloc(COMBI_BUFFER_SIZE * 2 * MAX_CARDS * sizeof(int));
	int num_cards_met[COMBI_BUFFER_SIZE][2];
	int num_card_combinations = 0;
	while (num_cards[0] && num_cards[1]) {
		for (int combi = 0; combi < num_card_combinations; combi++) {
			int* card_combi_met = card_combinations_met + combi * 2 * MAX_CARDS;
			int combi_is_equal = 1;
			for (int player = 0; player < 2; player++) {
				if (num_cards[player] != num_cards_met[combi][player]) {
					combi_is_equal = 0;
					break;
				}
				for (int i = 0; i < num_cards[player] && combi_is_equal; i++) {
					if (cards[player][i] != card_combi_met[player * MAX_CARDS + i]) {
						combi_is_equal = 0;
						break;
					}
				}
			}
			if (combi_is_equal) {
				free(card_combinations_met);
				return 0;
			}
		}
		assert(num_card_combinations < COMBI_BUFFER_SIZE);
		memcpy(card_combinations_met + num_card_combinations * 2 * MAX_CARDS, cards, 2 * MAX_CARDS * sizeof(int));
		memcpy(num_cards_met + num_card_combinations, num_cards, 2 * sizeof(int));
		num_card_combinations++;

		int winner = calc_winner(cards, num_cards, recursive_mode);
		int loser = 1 - winner;
		move_cards(cards[winner], num_cards + winner, cards[loser], num_cards + loser);
	}
	free(card_combinations_met);
	return num_cards[0] ? 0 : 1;
}

int calc_score(int cards[], int num_cards) {
	int score = 0;
	for (int i = 0; i < num_cards; i++) {
		score += cards[i] * (num_cards - i);
	}
	return score;
}

int main() {
	int cards[2][MAX_CARDS];
	int num_cards[2];

	char line[1000];
	FILE* fp = fopen(FILENAME, "r");
	while (1) {
		fgets(line, 1000, fp);
		if (!strcmp(line, "Player 1:\n") || !strcmp(line, "\n")) {
			// do nothing
		} else if (!strcmp(line, "Player 2:\n")) {
			break;
		} else {
			int card = atoi(line);
			assert(card > 0);
			assert(num_cards[0] < MAX_CARDS);
			cards[0][num_cards[0]++] = card;
		}
	}
	while (1) {
		fgets(line, 1000, fp);
		if (feof(fp)) {
			break;
		} else {
			int card = atoi(line);
			assert(card > 0);
			assert(num_cards[0] + num_cards[1] < MAX_CARDS);
			cards[1][num_cards[1]++] = card;
		}
	}
	fclose(fp);

	int original_cards[2][MAX_CARDS];
	memcpy(original_cards, cards, sizeof(cards));
	int original_num_cards[2] = {num_cards[0], num_cards[1]};

	int winner = play(cards, num_cards, 0);
	printf("Score: %d\n", calc_score(cards[winner], num_cards[winner]));

	memcpy(cards, original_cards, sizeof(original_cards));
	memcpy(num_cards, original_num_cards, sizeof(original_num_cards));

	winner = play(cards, num_cards, 1);
	printf("Score: %d\n", calc_score(cards[winner], num_cards[winner]));
}
